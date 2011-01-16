#include <bank/detail/queue.hpp>
#include <bank/detail/types.hpp>
#include <bank/error.hpp>

#include <limits>

#include <cstdlib>

namespace bank {
namespace detail {

queue::queue(void) throw(error) : start(NULL), end(NULL), first(NULL), last(NULL)
{
    void* buffer = std::calloc(1, std::numeric_limits<uint16_t>::max() + 1);
    if (buffer == NULL) { throw error("Could not allocate memory for removal queue"); }
    this->start = static_cast<size_t*>(buffer);
    this->last = this->first = this->start;
    this->end = this->start + std::numeric_limits<uint16_t>::max() + 1;
}

queue::~queue(void)
{
    if (this->start)
    {
        std::free(static_cast<void*>(this->start));
        this->first = this->last = this->start = this->end = NULL;
    }
}

size_t queue::size(void) const
{
    if (this->first > this->last) { return this->last - this->start + this->end - this->first; }
    return this->last - this->first;
}

void queue::push(size_t address)
{
    if ((++this->last) == this->first) { /* we're about to start overwriting the queue. What do we do? :/ */ }
    if (this->last > this->end) { this->last = this->start; }
    *this->last = address;
    ++this->last;
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
