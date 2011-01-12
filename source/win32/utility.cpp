#include <bank/detail/utility.hpp>

namespace bank {
namespace detail {

size_t get_memory_size(void)
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
}

}} /* namespace bank::detail */
