#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>

namespace bank {
namespace detail {

void for_each(array* start, size_t end, callable& functor)
{
    for (size_t idx = 0; idx < (end + 1); ++idx)
    {
        functor(start.at(idx));
        functor(start.at(idx + 1));
        functor(start.at(idx + 2));
        functor(start.at(idx + 3));
    }
}

}} /* namespace bank::detail */
