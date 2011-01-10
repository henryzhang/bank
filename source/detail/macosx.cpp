#include <bank/detail/platform.hpp>
#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>
#include <bank/detail/types.hpp>

#include <libkern/OSAtomic.h>
#include <sys/sysctl.h>

namespace bank {
namespace detail {

size_t get_memory_size(void)
{
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    size_t size = 0;
    size_t name_length = sizeof(mib) / sizeof(mib[0]);
    size_t length = sizeof(size);
    sysctl(mib, name_length, &size, &length, NULL, 0);
    return size;
}

/* The nonbarrier version of the OSAtomic* functions are used due to this library targeting Intel Macs only
 * Because of this, we don't need the barrier, as it causes an unnecessary cache flush.
 */
namespace atomic {

void set_false(bool& value)
{
    if (!value) { return; }
    OSAtomicIncrement32(reinterpret_cast<int32_t*>(&value));
}

void set_true(bool& value)
{
    if (value) { return; }
    OSAtomicDecrement32(reinterpret_cast<int32_t*>(&value));
}

bool compare(bool value, bool is)
{
    OSAtomicCompareAndSwapInt(value, value, reinterpret_cast<int*>(&is));
    return is;
}

} /* namespace atomic */

}} /* namespace bank::detail */
