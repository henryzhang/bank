#include <bank/detail/collector.hpp>
#include <bank/detail/thread.hpp>
#include <bank/detail/array.hpp>
#include <bank/detail/chunk.hpp>
#include <bank/detail/pool.hpp>

#include <cstdlib>

namespace bank {
namespace detail {


#pragma warning(disable: 4355)
collector::collector(pool& memory) : scanner(&collector::run, this), memory(memory), destruct(0), started(0)
{
    this->scanner.start();
    while (!this->started) { thread::yield(); }
}
#pragma warning(default: 4355)

collector::~collector(void)
{
    this->destruct = true;
    this->scanner.wait();
}

void collector::remove(const size_t& address)
{
    while (this->objects.full()) { thread::yield(); } // Hopefully by yielding, we can give the collector enough time to remove some objects.
    this->objects.push(address);
}

void collector::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* collector::operator new(size_t size) { return std::malloc(size); }

void collector::run(void* actual)
{
    collector* self = static_cast<collector*>(actual);
    self->started = true;

    /* Possibly the worst construct ever, but it works so... :/ */
    while (!self->destruct)
    {
        while(self->objects.empty() && !self->destruct) { thread::yield(); }
        while(!self->objects.empty())
        {
            size_t address = self->objects.pop();
            if (address == 0) { continue; }
            for (size_t idx = 0; idx < self->memory.size; idx++)
            {
                self->memory.list.at(idx).deallocate(address);
            }
        }
    }
}

}} /* namespace bank::detail */
