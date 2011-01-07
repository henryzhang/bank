#include <synk/detail/platform.hpp>
#include <bank/detail/utility.hpp>

namespace {

#if defined(SYNK_WINDOWS_PLATFORM)
inline size_t get_memory(void)
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
}

#elif defined(SYNK_MACOSX_PLATFORM) || defined(SYNK_FREEBSD_PLATFORM)
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

#else
#include <unistd.h>
inline size_t get_memory(void) { return static_cast<size_t>(sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE)); }

#endif /* SYNK_*_PLATFORM */

} /* internal namespace */

namespace bank {
namespace detail {

size_t get_total_memory(void) { return ::get_memory(); }

}} /* namespace bank::detail */
