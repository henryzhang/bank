#ifndef BANK_DETAIL_TRACKER_HPP
#define BANK_DETAIL_TRACKER_HPP

#include <cstdlib>

#include <bank/error.hpp>

/* Used by the memory pool to track the malloc allocation points.
 * Works like a LIFO stack, but is specialized for malloc'd memory pointers
 * These are freed on tracker destruction
 */
namespace bank {
namespace detail {

class tracker
{
        typedef void* buffer;
    public:
        explicit tracker(void) throw(error);
        virtual ~tracker(void);

        void push(buffer pointer);

    private:
        size_t index;
        buffer* start;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_TRACKER_HPP */
