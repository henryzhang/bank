#include <bank/detail/thread.hpp>
#include <bank/detail/types.hpp>
#include <bank/error.hpp>

#include <limits>

#include <Windows.h>

namespace {

struct thread
{
    static HANDLE& get(void) { static HANDLE handle; return handle; }
    static DWORD WINAPI execute(void* data) { static_cast<bank::detail::thread*>(data)->run(); return 0; }
};

}

namespace bank {
namespace detail {

thread::~thread(void) { CloseHandle(thread::get()); }

void thread::yield(void) { Sleep(0); }
void thread::wait(void) { WaitForSingleObject(thread::get(), INFINITE); }

void thread::start(void)
{
    thread::get() = CreateThread(NULL, (std::numeric_limits<uint16_t>::max() + 1) * 2,
                                 thread::execute, this, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL);
    if (thread::get() == NULL) { throw error("Could not create collector thread"); }
}


}} /* namespace bank::detail */
