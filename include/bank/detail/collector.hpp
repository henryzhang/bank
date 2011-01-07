#ifndef BANK_DETAIL_COLLECTOR_HPP
#define BANK_DETAIL_COLLECTOR_HPP

#include <queue>

#include <cstdlib>

#include <synk/detail/platform.hpp>
#include <synk/condition.hpp>
#include <synk/thread.hpp>
#include <synk/mutex.hpp>

#include <bank/detail/allocator/queue.hpp>
#include <bank/detail/ring.hpp>

namespace bank {
namespace detail {

class collector
{
    public:
        typedef std::queue<size_t, ring<size_t, allocator::queue<size_t>>> delete_queue; // Used for the consumer/producer stuff
        explicit collector(void);
        virtual ~collector(void);

        void remove(const size_t& address); // Adds the given address to the delete_queue

        void operator delete(void*);
        void* operator new(size_t);

    private:
        explicit collector(const collector& copy);
        static void run(void*);

        synk::condition condition;
        synk::thread thread;
        synk::mutex mutex;

        delete_queue objects;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_COLLECTOR_HPP */
