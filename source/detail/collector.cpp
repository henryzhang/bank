#include <bank/detail/collector.hpp>
#include <bank/detail/manager.hpp>
#include <bank/detail/chunk.hpp>
#include <bank/detail/pool.hpp>

#include <synk/condition.hpp>
#include <synk/utility.hpp>
#include <synk/thread.hpp>

#include <algorithm>
#include <utility>
#include <vector>

#include <cstdlib>

#include <iostream>

namespace {

struct deallocator
{
    typedef typename bank::detail::pool::iterator::value_type T;
    explicit deallocator(size_t address) : address(address) { }
    void operator ()(const T& chunk) const { const_cast<T&>(chunk).deallocate(address); }

    size_t address;
};

} /* namespace */

namespace bank {
namespace detail {

collector::collector(void) : thread(&collector::run, this), destruct(false) { this->thread.start(); }

collector::~collector(void)
{
    this->destruct = true;
    std::cout << "Signaling thread" << std::endl;
    this->condition.signal();
    std::cout << "Waiting on thread" << std::endl;
    this->thread.wait();
    std::cout << this << std::endl;
}

void collector::remove(const size_t& address)
{
    std::cout << std::hex << address << std::endl;
    this->objects.push(address);
    this->condition.signal();
    //if (this->objects.full()) { this->condition.signal(); }
}

void collector::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* collector::operator new(size_t size) { return std::malloc(size); }

void collector::run(void* actual)
{
    collector* self = static_cast<collector*>(actual);
    self->mutex.lock();
    std::cout << "Collector thread is running, and the mutex is locked" << std::endl;
    pool::chunk_list& instance = manager::instance().memory->list;
    while (self->objects.empty())
    {
        if (self->destruct) { break; }
        self->condition.wait(self->mutex);
        std::cout << "signaled" << std::endl;
        while (!self->objects.empty())
        {
            synk::parallel_for_each(instance.begin(), instance.end(), deallocator(self->objects.pop()));
        }
        std::cout << "Collection completed" << std::endl;
        if (self->destruct) { break; }
    }
    self->mutex.unlock();
}

}} /* namespace bank::detail */
