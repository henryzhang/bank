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
    explicit deallocator(const size_t& address) : address(address) { }
    void operator ()(const T& chunk) const { const_cast<T&>(chunk).deallocate(address); }

    const size_t& address;
};

} /* namespace */

namespace bank {
namespace detail {

collector::collector(void) : thread(&collector::run, this) { this->thread.start(); }

collector::~collector(void)
{
    this->thread.wait();
    if (this->mutex.is_locked()) { this->mutex.unlock(); }
}

void collector::remove(const size_t& address)
{
    std::cout << address << std::endl;
    this->objects.push(address);
    //TODO: Don't make this a magic number >:(
    this->condition.signal();
    //if (this->objects.size() > 20) { this->condition.signal(); }
}

void collector::operator delete(void* pointer) { std::free(pointer); pointer = NULL; }
void* collector::operator new(size_t size) { return std::malloc(size); }

void collector::run(void* actual)
{
    collector* self = static_cast<collector*>(actual);
    self->mutex.lock();
    pool::chunk_list& instance = manager::instance().memory->list;
    while (self->objects.empty())
    {
        self->condition.wait(self->mutex);
        while (!self->objects.empty())
        {
            synk::parallel_for_each(instance.begin(), instance.end(), deallocator(self->objects.front()));
            self->objects.pop();
        }
    }
    self->mutex.unlock();
}

}} /* namespace bank::detail */
