#include <bank/detail/platform.hpp>
#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>

#include <sys/sysctl.h>

namespace bank {
namespace detail {

size_t get_memory_zie(void)
{
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    size_t size = 0;
    size_t name_length = sizeof(mib) / sizeof(mib[0]);
    size_t length = sizeof(size);
    sysctl(mib, name_length, &size, &length, NULL, 0);
    return size;
}

}} /* namespace bank::detail */
