#ifndef BANK_DETAIL_ALLOCATOR_RESERVE_HPP
#define BANK_DETAIL_ALLOCATOR_RESERVE_HPP

#include <cstdlib>

#include <bank/detail/allocator/base.hpp>

namespace bank {
namespace detail {
namespace allocator {

template <typename T>
class reserve : public base<T>
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

        template <typename U> struct rebind { typedef reserve<U> other; };

        template <typename U> inline reserve(const reserve<U>& copy) : address(copy.address), buffer(copy.buffer), size(copy.size) { }
        inline reserve(const reserve<T>& copy) : address(copy.address), buffer(copy.buffer), size(copy.size) { }
        inline reserve(const size_t& size) : address(0), buffer(reserve<T>::get(size)), size(size) { }
        inline virtual ~reserve(void) { }

        inline pointer allocate(size_type size)
        {
            this->address += size;
            return reinterpret_cast<pointer>(static_cast<size_t*>(this->buffer) + this->address - size);
        }

        inline size_type max_size(void) const { return this->size / sizeof(value_type); }

        template <typename U> inline bool operator !=(const reserve<U>&) { return false; }
        template <typename U> inline bool operator ==(const reserve<U>&) { return true; }

    private:
        struct memory_block
        {
            inline explicit memory_block(void) : buffer(NULL) { }
            inline virtual ~memory_block(void) { if (this->buffer) { std::free(this->buffer); } }
            void* operator ()(const size_t& size)
            {
                if (this->buffer == NULL) { this->buffer = std::malloc(size); }
                return this->buffer;
            }
            void* buffer;
        };

        static void* get(const size_t& size)
        {
            static memory_block block;
            return block(size);
        }

        size_t address;
        void* buffer;
        size_t size;
};

}}} /* namespace bank::detail::allocator */

#endif /* BANK_DETAIL_ALLOCATOR_RESERVE_HPP */
