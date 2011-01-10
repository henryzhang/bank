#include <bank/detail/platform.hpp>
#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>

#include <unistd.h>

namespace bank {
namespace detail {

size_t get_memory_size(void) { return size_t(sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE)); }

}} /* namespace bank::detail */
