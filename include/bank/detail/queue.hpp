#ifndef BANK_DETAIL_QUEUE_HPP
#define BANK_DETAIL_QUEUE_HPP

#include <new>

#include <cstdlib>

#include <bank/error.hpp>

namespace bank {
namespace detail {

/* Used by the collector to actually remove members
 * Works like a circular buffer of sorts, but is specialized for size_t types
 */
class queue
{
    public:
        explicit queue(void) throw(error);
        virtual ~queue(void);

        size_t size(void) const;

        void push(size_t address);
        size_t pop(void);

        bool empty(void) const;
        bool full(void) const;

    private:
        size_t* start;  // Points to the true start of the memory buffer
        size_t* end;    // Points to the true end of the memory buffer

        size_t* first;  // returned by queue::front()
        size_t* last;   // returned by queue::back()
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_QUEUE_HPP */
