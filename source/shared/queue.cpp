#include <bank/detail/queue.hpp>
#include <bank/detail/types.hpp>
#include <bank/error.hpp>

#include <limits>

#include <cstdlib>

namespace bank {
namespace detail {

queue::queue(void) throw(error) : start(NULL), end(NULL), first(NULL), last(NULL)
{
    void* buffer = std::malloc(std::numeric_limits<uint16_t>::max() + 1);
    if (buffer == NULL) { throw error("Could not allocate memory for removal queue"); }
    const_cast<size_t*>(this->start) = static_cast<size_t*>(buffer);
    this->last = this->first = this->start;
    const_cast<size_t*>(this->end) = this->start + std::numeric_limits<uint16_t>::max() + 1;
}

queue::~queue(void)
{
    if (this->start)
    {
        std::free(static_cast<void*>(const_cast<size_t*>(this->start)));
        const_cast<size_t*>(this->start) = const_cast<size_t*>(this->end) = this->first = this->last = NULL;
    }
}

size_t queue::size(void) const
{
    if (this->first > this->last) { return this->last - this->start + this->end - this->first; }
    return this->last - this->first;
}

void queue::push(size_t address)
{
    if (this->empty()) { *this->last = address; return; }
    if ((++this->last) == this->first) { /* we're about to start overwriting the queue. What do we do? :/ */ }
    if (this->last > this->end) { this->last = this->start; }
    *this->last = address;
}

size_t queue::pop(void)
{
    size_t value = *this->first;
    *this->first = 0;
    if (++this->first > this->end) { this->first = this->start; }
    return value;
}

bool queue::empty(void) const { return this->first == this->last; }
bool queue::full(void) const { return this->first == (this->last - 1); }

}} /* namespace bank::detail */
