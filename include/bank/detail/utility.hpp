#ifndef BANK_DETAIL_UTILITY_HPP
#define BANK_DETAIL_UTILITY_HPP

#include <cstdlib>

namespace bank {
namespace detail {

class callable;
class array;

void for_each(array* start, size_t end, callable& functor);
size_t get_memory_size(void);

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_UTILITY_HPP */
