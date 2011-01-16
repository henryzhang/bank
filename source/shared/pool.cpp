#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>
#include <bank/detail/chunk.hpp>
#include <bank/detail/types.hpp>
#include <bank/detail/pool.hpp>
#include <bank/error.hpp>

#include <limits>

#include <cstdlib>

namespace {

/* An extremely weird bug manifests if we don't cast uint32_t::max to a 64-bit integer first */
const bank::uint32_t _64KB = std::numeric_limits<bank::uint16_t>::max() + 1;
const bank::uint64_t _4GB = static_cast<uint64_t>(std::numeric_limits<bank::uint32_t>::max()) + 1;

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

inline size_t find_single(bank::detail::array& range, const size_t& end, const size_t& size)
{
    for (size_t idx = 0; idx < end; ++idx) { if (range.at(idx).is_free(size)) { return idx; } }
    return max_chunks() + 1;
}

} /* namespace */

namespace bank {
namespace detail {

pool::pool(const size_t& chunks) throw(error) : allocs(max_allocs()), list(max_chunks()), index(0), size(chunks)
{
    if (chunks > max_chunks()) { throw error("Requested number of chunks is larger than system maximum"); }
    void* buffer = std::calloc(chunks, _64KB);
    if (buffer == NULL) { throw error("Could not allocate initial memory chunks in pool"); }
    size_t address = reinterpret_cast<size_t>(buffer);
    for (size_t idx = 0; idx < chunks; ++idx)
    {
        this->list.at(idx).set(address);
        address += (_64KB + 1);
    }
    this->allocs.push(buffer);
}

pool::~pool(void) { }

void pool::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* pool::operator new(size_t size) { return std::malloc(size); }

/* Effectively, when it comes to allocating memory, this is where the magic happens :)
 * This is the largest function in the entire library.
 */

// If there were any optimizations to be done, many would probably go here. :/
void* pool::allocate(const size_t& size)
{
    if (size > _4GB) { return NULL; } // What could you possibly be doing? Enjoy your NULL pointer.
    if (size >= _64KB) // Is bigger than a "normal" alloc, so we need to do some special work
    {
        this->index = find_single(this->list, this->size, size);
        if (this->index == max_chunks() + 1)
        {
            size_t required_chunks = (size / _64KB);
            if (required_chunks < 10) { required_chunks = 10; }
            else
            {
                // I *could* divide, I suppose, but I'll leave it for now, if we ever need to wildly deviate.
                size_t multiple = 0;
                do { required_chunks -= 10; ++multiple; } while (required_chunks > 10);
                ++multiple;
                required_chunks = multiple * 10; // Might be optimizable by bit shifting 3 and 1 and adding them. We'll trust the compiler for now.
            }

            void* buffer = std::calloc(required_chunks, _64KB); // Allocate in multiples of 10
            if (buffer == NULL) { throw error("Could not allocate and set new memory chunks"); }
            this->allocs.push(buffer);

            size_t address = reinterpret_cast<size_t>(buffer);
            for (this->index = this->size; this->size < required_chunks; ++this->size)
            {
                this->list.at(this->size).set(address);
                address += (_64KB + 1);
            }

            chunk& combined = this->list.at(this->index);
            for (size_t idx = this->index; idx < required_chunks; ++idx) { combined.combine(this->list.at(idx)); }
            return combined.allocate(size); // If this is ever null, we done boned it up :/
        }
    }

    void* buffer = this->list.at(this->index).allocate(size);
    if (buffer == NULL)
    {
        if (this->index + 1 > this->size || !this->list.at(this->index + 1).is_free(size))
        {
            this->index = find_single(this->list, this->size, size);
            if (this->index == max_chunks() + 1)
            {
                void* buffer = std::calloc(10, _64KB); // 640KB ought to be enough for anybody
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
