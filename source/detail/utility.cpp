#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>

namespace bank {
namespace detail {

/* A bit of an unrolled loop */
void for_each(array* start, size_t end, callable& functor)
{
    //TODO: Check to get the modulus of end % 4
    for (size_t idx; idx < (end + 1); ++idx)
    {
        functor(start.at(idx));
        functor(start.at(idx + 1));
        functor(start.at(idx + 2));
        functor(start.at(idx + 3));
    }
}

}} /* namespace bank::detail */
