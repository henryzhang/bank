#include <bank/detail/collector.hpp>
#include <bank/detail/callable.hpp>
#include <bank/detail/utility.hpp>
#include <bank/detail/thread.hpp>
#include <bank/detail/array.hpp>
#include <bank/detail/chunk.hpp>

#include <cstdlib>

#include <iostream>

namespace {

struct deallocator : public bank::detail::callable
{
    typedef bank::detail::chunk chunk;
    explicit deallocator(size_t address) : address(address) { }
    inline void operator ()(chunk& item) { item.deallocate(this->address); }
    size_t address;
};

} /* namespace */

namespace bank {
namespace detail {

collector::collector(array& memory) : scanner(&collector::run, this), memory(memory), destruct(0), started(0)
{
    this->scanner.start();
    while (!this->started) { thread::yield(); }
}

collector::~collector(void)
{
    std::cout << "signal to destruct sent" << std::endl;
    this->destruct = true;
    std::cout << "waiting" << std::endl;
    this->scanner.wait();
}

void collector::remove(const size_t& address)
{
    std::cout << std::hex << address << std::endl;
    this->objects.push(address);
    if (this->objects.full()) { thread::yield(); }
}

void collector::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* collector::operator new(size_t size) { return std::malloc(size); }

void collector::run(void* actual)
{
    collector* self = static_cast<collector*>(actual);
    self->started = true;
    std::cout << "collector thread is now running" << std::endl;

    /* Possibly the worst construct ever, but it works so... :/ */
    while (!self->destruct)
    {
        while(self->objects.empty() && !self->destruct) { thread::yield(); }
        while(!self->objects.empty())
        {
            for_each(&self->memory, self->memory.size(), deallocator(self->objects.pop()));
        }
    }
}

}} /* namespace bank::detail */
