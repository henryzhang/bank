#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>
#include <bank/detail/chunk.hpp>
#include <bank/detail/types.hpp>
#include <bank/detail/pool.hpp>
#include <bank/error.hpp>

#include <limits>

#include <cstdlib>

namespace {

const bank::uint32_t _64KB = std::numeric_limits<bank::uint16_t>::max() + 1;

inline size_t max_chunks(void) { return bank::detail::get_memory_size() / _64KB; }

/* Returns the amount of memory to use to track the chunks -- in bytes.
 * Technically it is a bit more than should it be, but this is done "just in case"
 * If you want to get those few extra kilobytes back, this is where you should change the algorithm
 */
inline size_t allocate_with_set(bank::detail::array& list, size_t start, size_t size)
{
    void* buffer = std::malloc(size * _64KB);
    if (buffer == NULL) { throw bank::error("Could not allocate and set memory chunks"); }
//    for_each();
}

struct setter : public bank::detail::callable
{
    typedef bank::detail::chunk chunk;
    inline explicit setter(void* buffer) : address(reinterpret_cast<size_t>(buffer)) { }
    inline void operator ()(chunk& item) { item.set(this->address, _64KB); this->address += (_64KB + 1); }
    size_t address;
};

struct combiner : public bank::detail::callable
{
    typedef bank::detail::chunk chunk;
    inline explicit combiner(chunk& chunk) : chunk(chunk) { }
    inline void operator ()(chunk& item) { this->chunk.combine(item); }
    chunk& chunk;
};

struct finder : public bank::detail::callable
{
    typedef bank::detail::chunk chunk;
    inline explicit finder(size_t& index) : index(index), found(false) { }
    inline void operator ()(chunk& item)
    {
        if (found) { return; }
        if (item.is_free()) { this->found = true; return; }
        ++this->index;
    }
    size_t& index;
    bool found;
};

} /* namespace */

namespace bank {
namespace detail {

pool::pool(const size_t& chunks) throw(error) : list(max_chunks()), index(0), size(chunks)
{
    std::cout << "in pool ctor" << std::endl;
    if (chunks > max_chunks()) { throw error("Requested number of chunks is larger than maximum count"); }
    void* buffer = std::malloc(chunks * _64KB);
    if (buffer == NULL) { throw error("Could not allocate initial memory chunks in pool"); }
    for_each(this->list, chunks, setter(buffer));
}

pool::~pool(void) { while (!this->allocs.empty()) { std::free(this->allocs.pop()); } }

void pool::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* pool::operator new(size_t size) { return std::malloc(size); }

/* If the given size is larger than a chunk size, we must then perform a check to see if we currently
 * have enough memory left from where we are, to the end of the currently tracked number of chunks,
 * as `this->list.size()` is (hopefully) never equal to `this->size`.
 *
 * If there is not enough space left in the current number of chunks, we allocate enough chunks, in bytes,
 * set them to chunks, then combine those chunks, so that behavior for the collector stays consistent.
 *
 * If there is enough space, then we simply combine said chunks, and then return the memory allocated from
 * these chunks.
 *
 * Possible issues are if the requested size is equal to `(this->size - this->index) * _64KB + 1`, as it would
 * mean the rest of the chunk is nearly wasted, though the gc should eventually use the rest of it.
 *
 * For the "normal" allocation call, we allocate from the current chunk. If the current chunk returns NULL
 * then we move to the next chunk in the ring. If the current chunk is the "last" chunk, we check to see
 * if there are any free chunks in the ring. If there are, we set the index to that, allocate, and then move
 * on, otherwise we allocate 10 new chunks, move to the first, then allocate and return.
 * If the current chunk is not the last chunk, then a move forward is performed, until a given chunk is
 * found to be free. If none can be found, then the previously mentioned allocation is performed, and the
 * same results are given. This *can* cause an issue where there are large rings (as the indexing forward
 * is performed in worst-case O(n)), so a few optimizations are performed. Specifically use of the
 * synk::parallel_for_each function, though this may cause a slowdown for systems with small amounts of memory
 * 
 * Effectively, when it comes to allocating memory, this is where the magic happens :)
 * I'm also fairly certain this is the largest function in the entire library.
 */

void* pool::allocate(const size_t& size)
{
    if (size > _64KB)
    {
        size_t idx = this->index;
        if ((this->size - this->index) * _64KB < size)
        {
            idx = alloc_and_set(this->size, size / _64KB + 1, this->list);
        }
        iterator start = this->list.begin();
        std::advance(start, idx + 1);

        iterator end = this->list.begin();
        std::advance(end, this->size);

        std::for_each(start, end, ::chunk::combiner(this->list.at(idx)));
        return this->list.at(idx).allocate(size);
    }

    void* buffer = this->get_current().allocate(size);
    if (buffer == NULL)
    {
        if (this->index + 1 >= this->size || !this->list.at(this->index + 1).is_free())
        {
            size_t idx = this->index;
            const iterator& end = this->list.begin() + this->size;
            synk::parallel_for_each(this->list.begin(), end, ::chunk::finder(idx, this->list.begin()));
            * If the index has not changed at all, then we need to allocate 10 new chunks *
            if (this->index == idx) { idx = alloc_and_set(this->size, 10, this->list); }
            this->index = idx; // Update the current index
            return this->get_current().allocate(size);
        }
        else { return this->list.at((++this->index)).allocate(size); }
    }
    return buffer;
}


}} /* namespace bank::detail */
