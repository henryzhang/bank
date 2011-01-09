#include <bank/detail/collector.hpp>
#include <bank/detail/callable.hpp>
#include <bank/detail/manager.hpp>
#include <bank/detail/chunk.hpp>
#include <bank/detail/pool.hpp>

#include <synk/condition.hpp>
#include <synk/thread.hpp>

#include <algorithm>
#include <utility>
#include <vector>

#include <cstdlib>

#include <iostream>

#include <pthread.h>

namespace {

struct deallocator : public callable
{
    explicit deallocator(size_t address) : address(address) { }
    void operator ()(chunk& item) { if (this->address == 0) { return; } item.deallocate(address); }
    size_t address;
};

} /* namespace */

namespace bank {
namespace detail {

collector::collector(void) : thread(&collector::run, this), destruct(false), started(false)
{
    this->thread.start();
    while (!this->started) { pthread_yield_np(); }
}

collector::~collector(void)
{
    std::cout << "Signaling thread" << std::endl;
    this->destruct = true;
    std::cout << "Waiting on thread" << std::endl;
    this->thread.wait();
    std::cout << this << std::endl;
}

void collector::remove(const size_t& address)
{
    std::cout << std::hex << address << std::endl;
    this->objects.push(address);
    if (this->objects.full()) { /* place a yield here, or something to slow down the delete thread */ }
}

void collector::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* collector::operator new(size_t size) { return std::malloc(size); }

/* Yes, this function runs on its own thread, otherwise we wouldn't have a concurrent collector :) */
void collector::run(void* actual)
{
    collector* self = static_cast<collector*>(actual);
    self->started = true;
    std::cout << "Collector thread is running, and the mutex is locked" << std::endl;
    pool::array& instance = manager::instance().memory->cluster;

    /* The "worst" while loop construction I've ever made, but it replicates a spinlock of sorts :P */
    while (!self->destruct) {
        while (self->objects.empty() && !self->destruct) { /* Place a yield here */ }
        while (!self->objects.empty())
        {
            for_each(&instance[0], instance.size(), deallocator(self->objects.pop()));
//            synk::parallel_for_each(instance.begin(), instance.end(), deallocator(self->objects.pop()));
        }
    }
}

}} /* namespace bank::detail */
