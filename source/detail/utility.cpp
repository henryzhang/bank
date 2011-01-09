#include <bank/detail/platform.hpp>
#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>

namespace {

#if defined(BANK_WINDOWS_PLATFORM)
#include <Windows.h>
inline size_t get_memory(void)
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
}

#elif defined(BANK_LINUX_PLATFORM)
#include <unistd.h>
inline size_t get_memory(void) { return size_t(sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE)); }

#elif defined(BANK_MACOSX_PLATFORM) || defined(BANK_FREEBSD_PLATFORM)
#include <sys/sysctl.h>
inline size_t get_memory(void)
{
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    size_t size = 0;
    size_t name_length = sizeof(mib) / sizeof(mib[0]);
    size_t length = sizeof(size);
    sysctl(mib, name_length, &size, &length, NULL, 0);
    return size;
}


} /* internal namespace */

namespace bank {
namespace detail {

void for_each(array* start, size_t end, callable& functor)
{
    for (size_t idx; idx < (end + 1); ++idx) { functor(start.at(idx)); }
}

size_t get_memory_size(void) { return ::get_memory(); }

}} /* namespace bank::detail */
