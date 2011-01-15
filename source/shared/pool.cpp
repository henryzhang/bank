#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>
#include <bank/detail/chunk.hpp>
#include <bank/detail/types.hpp>
#include <bank/detail/pool.hpp>
#include <bank/error.hpp>

#include <limits>

#include <cstdlib>
#include <iostream>

namespace {

const bank::uint32_t _64KB = std::numeric_limits<bank::uint16_t>::max() + 1;

/* maximum number of chunks, followed by the number of times the pool can allocate from the system
 * These values are memoized at runtime, to save any future calls to them, at the expense of some memory.
 * In my opinion it is better to store the variable in memory, than to make the system calls every time.
 */
inline size_t max_chunks(void)
{
    static const size_t chunks = bank::detail::get_memory_size() / _64KB;
    return chunks;
}
inline size_t max_allocs(void)
{
    static const size_t allocs = (max_chunks() - 256) / 10 + 1;
    return allocs;
}

inline size_t find_single(bank::detail::array& range, const size_t& size)
{
    for (size_t idx = 0; idx < size; ++idx) { if (range.at(idx).is_free()) { return idx; } }
    return max_chunks() + 1;
}

inline size_t find_range(bank::detail::array& range, const size_t& size, const size_t& count)
{
    size_t index, current = 0;

    for (size_t idx = 0; idx < size; ++idx)
    {
        bank::detail::chunk& ref = range.at(idx);
//TODO: Fix to make sure we can actually combine all of these.
//if (ref.next_to(range.at(idx + 1)) && ref.is_free())
//          && ref.next_to(range.at(idx + 1)) ? ++current : current = 0;
        if (current == 0) { ++index; }
        if (current == count) { return index; }
    }

    return max_chunks() + 1;
}

} /* namespace */

namespace bank {
namespace detail {

pool::pool(const size_t& chunks) throw(error) : allocs(max_allocs()), list(max_chunks()), index(0), size(chunks)
{
    if (chunks > max_chunks()) { throw error("Requested number of chunks is larger than system maximum"); }
    void* buffer = std::malloc(chunks * _64KB);
    if (buffer == NULL) { throw error("Could not allocate initial memory chunks in pool"); }
    size_t address = reinterpret_cast<size_t>(buffer);
    for (size_t idx = 0; idx < chunks; ++idx)
    {
        this->list.at(idx).set(address);
        address += (_64KB + 1);
    }
}

pool::~pool(void) { }

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
 * This is the largest function in the entire library.
 */

// If there were any optimizations to be done, many would probably go here. :/
void* pool::allocate(const size_t& size)
{
    if (size > _64KB) // Is bigger than a "normal" alloc, so we need to do some special work
    {
        size_t required_chunks = (size / _64KB) + 1;
        this->index = find_range(this->list, this->size, required_chunks);
        if (this->index == max_chunks() + 1)
        {
            if (required_chunks < 10) { required_chunks = 10; }
            else
            {
                size_t multiple = 0;
                do { required_chunks -= 10; ++multiple; } while (required_chunks > 10);
                ++multiple;
                required_chunks = multiple * 10;
            }

            void* buffer = std::malloc(required_chunks * _64KB);
            if (buffer == NULL) { throw error("Could not allocate and set new memory chunks"); }
            this->allocs.push(buffer);

            size_t address = reinterpret_cast<size_t>(buffer);
            for (this->index = this->size; this->size < required_chunks; ++this->size)
            {
                this->list.at(this->size).set(address);
                address += (_64KB + 1);
            }
        }

        chunk& combined = this->list.at(this->index);
        for (size_t idx = this->index; idx < required_chunks; ++idx) { combined.combine(this->list.at(idx)); }
        return combined.allocate(size); // If this is ever null, we done boned it up :/
    }

    void* buffer = this->list.at(this->index).allocate(size);
    if (buffer == NULL)
    {
        if (this->index + 1 >= this->size || !this->list.at(this->index + 1).is_free())
        {
            size_t idx = this->index;
            this->index = find_single(this->list, this->size);
            if (this->index == max_chunks() + 1)
            {
                void* buffer = std::malloc(10 * _64KB); // 640KB should be all that anyone ever needs
                if (buffer == NULL) { throw error("Could not allocate and set new memory chunks"); }
                this->allocs.push(buffer);

                size_t address = reinterpret_cast<size_t>(buffer);
                for (this->index = this->size; this->size != (this->size + 11); ++this->size)
                {
                    this->list.at(this->size).set(address);
                    address += (_64KB + 1);
                }
            }
            return this->list.at(this->index).allocate(size);
        }
        else { return this->list.at((++this->index)).allocate(size); }
    }
    return buffer;
}


}} /* namespace bank::detail */
