#ifndef BANK_DETAIL_ALLOCATOR_QUEUE_HPP
#define BANK_DETAIL_ALLOCATOR_QUEUE_HPP

#include <limits>
#include <new>

#include <cstdlib>

#include <bank/detail/allocator/base.hpp>
#include <bank/detail/types.hpp>

#include <iostream>

namespace bank {
namespace detail {
namespace allocator {

class inert
{
    inline explicit inert(void) throw(std::bad_alloc) : next(0), buffer(NULL)
    {
        this->buffer = static_cast<size_t*>(std::malloc((std::numeric_limits<uint16_t>::max() + 1)));
        if (this->buffer == NULL) { throw std::bad_alloc(); }
    }

    inline virtual ~inert(void) { if (this->buffer) { std::free(this->buffer); } }

    inline size_t max_size(void) { return (std::numeric_limits<uint16_t>::max() + 1) / sizeof(size_t); }
    inline void* alloc(size_t size)
    {
        std::cout << "alloc size:  " << size << std::endl;
        return static_cast<void*>(this->buffer + (this->next += size)); }
    inline static inert& get(void) { static inert buffer; return buffer; }
    inline void dalloc(size_t size)
    {
        std::cout << "dalloc size: " << size << std::endl;
        this->next -= size;
    }

    size_t* buffer;
    uint16_t next;

    template <typename T> friend class queue;
};

template <typename T>
class queue : public base<T>
{
    public:
        typedef base<T> super;

        typedef typename super::difference_type difference_type;
        typedef typename super::value_type value_type;
        typedef typename super::size_type size_type;

        typedef typename super::const_reference const_reference;
        typedef typename super::const_pointer const_pointer;
        typedef typename super::reference reference;
        typedef typename super::pointer pointer;

        template <typename U> struct rebind { typedef queue<U> other; };

        template <typename U> inline queue(const queue<U>& copy) : buffer(inert::get()) { }
        inline queue(const queue<T>& copy) : buffer(inert::get()) { }
        inline queue(void) : buffer(inert::get()) { }

        inline virtual ~queue(void) { }

        inline pointer allocate(size_type size) { return reinterpret_cast<pointer>(this->buffer.alloc(size)); }
        inline void deallocate(void*, size_type size) { this->buffer.dalloc(size); }

        inline size_type max_size(void) const { return this->buffer.max_size(); }

        template <typename U> inline bool operator !=(const queue<U>&) { return false; }
        template <typename U> inline bool operator ==(const queue<U>&) { return true; }

    private:
        inert& buffer;
};


}}} /* namespace bank::detail::allocator */

#endif /* BANK_DETAIL_ALLOCATOR_QUEUE_HPP */
