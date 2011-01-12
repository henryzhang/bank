#include <bank/detail/platform.hpp>
#include <bank/detail/thread.hpp>
#include <bank/detail/types.hpp>
#include <bank/error.hpp>

#include <cstdlib>

#include <pthread.h>
#include <errno.h>

#if defined(BANK_GENBSD_PLATFORM)
    #define pthread_yield pthread_yield_np
#endif 

namespace {

struct thread
{
    static pthread_t& get(void) { static pthread_t handle; return handle; }
    static void* execute(void* data) { static_cast<bank::detail::thread*>(data)->run(); return NULL; }
};

} /* internal namespace */

namespace bank {
namespace detail {

thread::~thread(void) { pthread_detach(&thread::get()); }
void thread::yield(void) { pthread_yield(); }

void thread::wait(void)
{
    if (pthread_join(thread::get(), NULL) != 0)
    {
        switch (errno)
        {
            case EDEADLK: throw error("The collector has deadlocked"); break;
            case EINVAL: throw error("The collector is no long joinable"); break;
            case ESRCH: throw error("The collector can not be found"); break;
        }
    }
}

/* This takes its time, because if something goes wrong, we need to know */
void thread::start(void)
{
    pthread_attr_t attr;
    if ((pthread_attr_init(&attr) != 0) || (pthread_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) != 0))
    {
        switch (errno)
        {
            case ENOMEM: throw error("Insufficient memory to allocate collector thread"); break;
            case EINVAL: throw error("Invalid attribute for collector thread"); break;
        }
    }

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if (pthread_create(&thread::get(), &attr, thread::execute, this) != 0)
    {
        switch (errno)
        {
            case EAGAIN: throw error("The collector thread exceeds the maximum number of available"); break;
            case EINVAL: throw error("The collector thread has an invalid attribute"); break;
        }
    }
    pthread_attr_destroy(&attr);
}

}} /* namespace bank::detail */
