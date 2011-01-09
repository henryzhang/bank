#include <bank/detail/queue.hpp>
#include <bank/detail/types.hpp>
#include <bank/error.hpp>

#include <limits>
#include <new>

#include <cstdlib>

namespace bank {
namespace detail {

queue::queue(void) throw(error) : start(NULL), end(NULL), first(NULL), last(NULL)
{
    this->start = static_cast<size_t*>(std::malloc((std::numeric_limits<uint16_t>::max() + 1)) * 2);
    if (this->start == NULL) { throw error("Could not allocate memory for collector::queue"); }
    this->last = this->first = this->start;
    this->end = this->start + (std::numeric_limits<uint16_t>::max() + 1) * 2;
}

queue::~queue(void)
{
    if (this->start)
    {
        std::free(static_cast<void*>(start));
        this->start = this->end = this->first = this->last = NULL;
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
    if ((++this->last) == this->first) { /* We're about to start overwriting the queue completely, need to handle this somehow, so we don't get any memory leaks */ }
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
