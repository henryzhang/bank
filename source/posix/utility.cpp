#include <bank/detail/platform.hpp>
#include <bank/detail/utility.hpp>

#include <sys/sysctl.h>
#include <unistd.h>


namespace bank {
namespace detail {

size_t get_memory_size(void)
{
    #if defined(BANK_GENBSD_PLATFORM)
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    size_t size = 0;
    size_t name_length = sizeof(mib) / sizeof(mib[0]);
    size_t length = sizeof(size);
    sysctl(mib, name_length, &size, &length, NULL, 0);
    return size
    #elif defined(BANK_LINUX_PLATFORM)
    return size_t(sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE));
    #else
        #error "Unknown Platform :("
    #endif /* BANK_GENBSD_PLATFORM */
}

}} /* namespace bank::detail */
