#include <bank/detail/collector.hpp>
#include <bank/detail/thread.hpp>
#include <bank/detail/array.hpp>
#include <bank/detail/chunk.hpp>

#include <cstdlib>

#include <iostream>

namespace {

struct deallocator
{
    typedef bank::detail::chunk chunk;
    inline explicit deallocator(void) : address(0) { }
    inline void operator ()(chunk& item) { item.deallocate(this->address); }
    inline void operator =(const size_t& address) { this->address = address; }
    size_t address;
};

} /* namespace */

namespace bank {
namespace detail {


#pragma warning(disable: 4355)
collector::collector(array& memory) : scanner(&collector::run, this), memory(memory), destruct(0), started(0)
{
    this->scanner.start();
    while (!this->started) { thread::yield(); }
}
#pragma warning(default: 4355)

collector::~collector(void)
{
    std::cout << "signal to destruct sent" << std::endl;
    this->destruct = true;
    std::cout << "waiting" << std::endl;
    this->scanner.wait();
}

void collector::remove(const size_t& address)
{
    while (this->objects.full()) { thread::yield(); } // Hopefully by yielding, we can give the collector enough time to remove some objects.
    this->objects.push(address);
    std::cout << "Deferring removal of address: " << address << std::endl;
}

void collector::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* collector::operator new(size_t size) { return std::malloc(size); }

void collector::run(void* actual)
{
    collector* self = static_cast<collector*>(actual);
    deallocator functor;
    self->started = true;
    std::cout << "collector thread is now running" << std::endl;

    /* Possibly the worst construct ever, but it works so... :/ */
    while (!self->destruct)
    {
        while(self->objects.empty() && !self->destruct) { thread::yield(); }
        while(!self->objects.empty())
        {
            functor = self->objects.pop();
            for (size_t idx = 0; idx < self->memory.get_size(); idx++) { functor(self->memory.at(idx)); }
        }
    }
}

}} /* namespace bank::detail */
