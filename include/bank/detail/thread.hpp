#ifndef BANK_DETAIL_THREAD_HPP
#define BANK_DETAIL_THREAD_HPP

/* Used to run the actual collection cycle, and as there is only ever one internally, the platform
 * specific struct is not defined here, nor is it modifiable
 * Instead, the given thread is static
 * The actual thread is not started on the system until thread::start is called, rather than on creation.
 */

#include <cstdlib>

namespace bank {
namespace detail {

class thread
{
    public:
        typedef void (*callback)(void*);
        explicit thread(const callback& function, void* data = NULL);
        ~thread(void);

        static void yield(void);
        void start(void);
        void wait(void);
        void run(void);

    private:
        callback function;
        void* data;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_THREAD_HPP */
