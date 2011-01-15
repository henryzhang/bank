#ifndef BANK_ALLOCATOR_HPP
#define BANK_ALLOCATOR_HPP

/* Used for the C++ STL Types */

#include <limits>
#include <new>

#include <cstdlib>

#include <bank/bank.hpp>

namespace bank {

template <typename T>
class allocator
{
    public:
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef T value_type;

        typedef const T& const_reference;
        typedef const T* const_pointer;

        typedef T& reference;
        typedef T* pointer;

        template <typename U> struct rebind { typedef allocator<U> other; };

        template <typename U> inline allocator(const allocator<U>& copy) { }
        inline allocator(const allocator<T>& copy) { }
        inline allocator(void) { }

        inline virtual ~allocator(void) { }

        inline void construct(pointer ptr, const_reference value)
        {
            new(static_cast<void*>(ptr)) value_type(value);
        }

        inline void construct(pointer ptr) { new(static_cast<void*>(ptr)) value_type(); }
        inline void destroy(pointer ptr) { ptr->~value_type(); }

        inline const_pointer address(const_reference ref) const { return &ref; }
        inline pointer address(reference ref) const { return &ref; }

        inline pointer allocate(size_type size) { return reinterpret_cast<pointer>(bank::alloc(size)); }
        
        inline void deallocate(pointer p, size_type s) { this->deallocate(static_cast<void*>(p), s); }
        inline void deallocate(void* p, size_type) { bank::free(p); }

        inline size_type max_size(void) const { return std::numeric_limits<size_type>::max() / sizeof(T); }

        template <typename U> inline bool operator !=(const allocator<U>&) { return false; }
        template <typename U> inline bool operator ==(const allocator<U>&) { return true; }
};

} /* namespace bank */


#endif /* BANK_ALLOCATOR_HPP */
