#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>

namespace bank {
namespace detail {

void for_each(array& start, size_t end, callable& functor)
{
    for (size_t idx = 0; idx < (end + 1); ++idx)
    {
        functor(start.at(idx));
    }
}

}} /* namespace bank::detail */
