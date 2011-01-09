#ifndef BANK_DETAIL_COLLECTOR_HPP
#define BANK_DETAIL_COLLECTOR_HPP

#include <cstdlib>

#include <synk/detail/platform.hpp>
#include <synk/condition.hpp>
#include <synk/thread.hpp>
#include <synk/mutex.hpp>

#include <bank/detail/queue.hpp>

namespace bank {
namespace detail {

class collector
{
    public:
        explicit collector(void);
        virtual ~collector(void);

        void remove(const size_t& address); // Adds the given address to the queue

        void operator delete(void*);
        void* operator new(size_t);

    private:
        explicit collector(const collector& copy);
        static void run(void*);

        synk::condition condition;
        synk::thread thread;
        synk::mutex mutex;
        //thread scanner;
        queue objects;
        bool destruct;
        bool started;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_COLLECTOR_HPP */
