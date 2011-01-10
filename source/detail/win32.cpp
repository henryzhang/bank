#include <bank/detail/platform.hpp>
#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/array.hpp>

#include <Windows.h>

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
