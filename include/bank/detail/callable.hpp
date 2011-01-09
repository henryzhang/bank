#ifndef BANK_DETAIL_CALLABLE_HPP
#define BANK_DETAIL_CALLABLE_HPP

namespace bank {
namespace detail {

class chunk;

/* All functors for the for_each function MUST inherit from this type */
struct callable { virtual void operator ()(chunk& item) = 0; };

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_CALLABLE_HPP */
