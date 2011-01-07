#ifndef BANK_DETAIL_ALLOCATOR_BASE_HPP
#define BANK_DETAIL_ALLOCATOR_BASE_HPP

#include <cstdlib>

namespace bank {
namespace detail {
namespace allocator {

template <typename T>
class base
{
    public:
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef T value_type;

        typedef const T& const_reference;
        typedef const T* const_pointer;

        typedef T& reference;
        typedef T* pointer;

        template <typename U> struct rebind { typedef base<U> other; };

        inline explicit base(const base<T>& copy) { }
        inline explicit base(void) { }
        inline virtual ~base(void) { }

        inline void construct(pointer ptr, const_reference value) { new(static_cast<void*>(ptr)) T(value); }
        inline void construct(pointer ptr) { new(static_cast<void*>(ptr)) value_type(); }
        inline void destroy(pointer ptr) { ptr->~value_type(); }

        inline const_pointer address(const_reference ref) const { return &ref; }
        inline pointer address(reference ref) const { return &ref; }

        inline void deallocate(pointer p, size_type s) { this->deallocate(static_cast<void*>(p), s); }
        inline virtual void deallocate(void*, size_type) { }
        inline virtual pointer allocate(size_type) { return NULL; }

        inline virtual size_type max_size(void) { return 0; }

        template <typename U> inline bool operator !=(const base<U>&) { return false; }
        template <typename U> inline bool operator ==(const base<U>&) { return true; }
};

}}} /* namespace bank::detail::allocator */

#endif /* BANK_DETAIL_ALLOCATOR_BASE_HPP */
