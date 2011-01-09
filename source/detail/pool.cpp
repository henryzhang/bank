#include <bank/detail/allocator/reserve.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/chunk.hpp>
#include <bank/detail/types.hpp>
#include <bank/detail/pool.hpp>
#include <bank/error.hpp>

#include <synk/utility.hpp>

#include <algorithm>
#include <iterator>
#include <vector>
#include <limits>
#include <new>

#include <cstdlib>

#include <iostream>

namespace {

const bank::uint32_t _64KB = std::numeric_limits<bank::uint16_t>::max() + 1;

namespace chunk {

struct setter
{
    explicit inline setter(void* buffer) : address(reinterpret_cast<size_t>(buffer)) { }
    template <typename T> inline void operator ()(T& item)
    {
        item.set(this->address, _64KB);
        this->address += (_64KB + 1);
    }

    size_t address;
};

struct combiner
{
    typedef bank::detail::chunk& block;
    explicit inline combiner(block chunk) : chunk(chunk) { }
    template <typename T> inline void operator ()(T& item) { this->chunk.combine(item); }
    block chunk;
};

/* No lock is used because if the operator () finds two free chunks, it does not matter as to which one is
 * set. We just want a free chunk is all :)
 */
struct finder
{
    typedef const bank::detail::pool::iterator& iterator;
    explicit inline finder(size_t& index, finder::iterator start) : index(index), found(false), start(start) { }
    template <typename T> inline void operator ()(const T& item) const
    {
        if (found) { return; }
        if (item.is_free())
        {
            this->found = true;
            const_cast<size_t&>(this->index) = (&item - &*this->start) / sizeof(bank::detail::chunk); // Am I cheating? :P
        }
    }

    const size_t& index;
    mutable bool found;
    iterator start;
};

} /* namespace chunk */

inline double max_chunks(void) { return static_cast<double>(bank::detail::get_memory_size()) / _64KB; }

/* Returns the amount of memory to use to track the chunks -- in bytes.
 * Technically it is a bit more than should it be, but this is done "just in case"
 * If you want to get those few extra kilobytes back, this is where you should change the algorithm
 */
inline size_t get_reserved(void)
{
    double reserved = max_chunks() * sizeof(bank::detail::chunk);
    return (static_cast<size_t>(reserved) / _64KB + 1) * _64KB;
}

size_t alloc_and_set(size_t& begin, const size_t& size, bank::detail::pool::chunk_list& list)
{
    typedef bank::detail::pool::chunk_list::iterator iterator;
    
    void* buffer = std::malloc(size * _64KB);
    if (buffer == NULL) { throw bank::error("Could not allocate and set memory chunks"); }
    
    iterator start = list.begin();
    std::advance(start, begin);

    iterator end = start;
    std::advance(end, size + 1);

    std::for_each(start, end, ::chunk::setter(buffer));

    size_t idx = begin;
    begin += size + 1;
    return idx;
}

} /* internal namespace */

namespace bank {
namespace detail {

pool::pool(const size_t& chunks) throw(error) : list(max_chunks(), chunk(), allocator::reserve<chunk>(get_reserved())), index(0)
{
    std::cout << "In pool ctor" << std::endl;
    void* buffer = std::malloc(chunks * _64KB);
    if (buffer == NULL) { throw error("Could not allocate initial memory chunks in pool"); }
    std::for_each(this->list.begin(), this->list.begin() + (chunks + 1), ::chunk::setter(buffer));
    this->size = chunks;
}

pool::~pool(void) { }

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
            /* If the index has not changed at all, then we need to allocate 10 new chunks */
            if (this->index == idx) { idx = alloc_and_set(this->size, 10, this->list); }
            this->index = idx; // Update the current index
            return this->get_current().allocate(size);
        }
        else { return this->list.at((++this->index)).allocate(size); }
    }
    return buffer;
}

chunk& pool::get_index(const size_t& index) { return this->list.at(index); }
chunk& pool::get_current(void) { return this->list.at(this->index); }
size_t pool::get_size(void) const { return this->list.size(); }

chunk& pool::operator [](const size_t& index) { return this->get_index(index); }
void pool::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* pool::operator new(size_t size) { return std::malloc(size); }

}} /* namespace bank::detail */
