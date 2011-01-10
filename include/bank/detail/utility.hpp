#ifndef BANK_DETAIL_UTILITY_HPP
#define BANK_DETAIL_UTILITY_HPP

#include <cstdlib>

namespace bank {
namespace detail {

class callable;
class array;

void for_each(array* start, size_t end, callable& functor);
size_t get_memory_size(void);

namespace atomic {

void increment(size_t& value);
void decrement(size_t& value);

void set_false(bool& value);
void set_true(bool& value);

bool compare(size_t& left, size_t& right);
bool compare(bool left, bool is);

} /* namespace atomic */

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_UTILITY_HPP */
