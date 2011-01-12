#ifndef BANK_DETAIL_COLLECTOR_HPP
#define BANK_DETAIL_COLLECTOR_HPP

#include <cstdlib>

#include <bank/detail/thread.hpp>
#include <bank/detail/queue.hpp>

namespace bank {
namespace detail {

class array;

class collector
{
    public:
        explicit collector(array& memory);
        virtual ~collector(void);

        void remove(const size_t& address);

        void operator delete(void*);
        void* operator new(size_t);

    private:
        explicit collector(const collector& copy);
        static void run(void*);

        thread scanner;
        queue objects;
        array& memory;
        bool destruct;
        bool started;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_COLLECTOR_HPP */
