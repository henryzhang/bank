#ifndef BANK_ALLOCATOR_HPP
#define BANK_ALLOCATOR_HPP

#include <limits>

#include <bank/detail/allocator/base.hpp>
#include <bank/new.hpp>

namespace bank {

template <typename T>
class allocator : public detail::allocator::base<T>
{
    public:
        typedef detail::allocator::base<T> super;

        typedef typename super::difference_type difference_type;
        typedef typename super::value_type value_type;
        typedef typename super::size_type size_type;

        typedef typename super::const_reference const_reference;
        typedef typename super::const_pointer const_pointer;
        typedef typename super::reference reference;
        typedef typename super::pointer pointer;

        template <typename U> struct rebind { typedef allocator<U> other; };

        template <typename U> inline allocator(const allocator<U>& copy) { }
        inline allocator(const allocator<T>& copy) { }
        inline allocator(void) { }

        inline virtual ~allocator(void) { }

        inline pointer allocate(size_type size) { return reinterpret_cast<pointer>(alloc(size)); }
        inline void deallocate(void* p, size_type) { bank::free(p); }

        inline size_type max_size(void) const { return std::numeric_limits<size_type>::max() / sizeof(T); }

        template <typename U> inline bool operator !=(const allocator<U>&) { return false; }
        template <typename U> inline bool operator ==(const allocator<U>&) { return true; }
};

} /* namespace bank */

#endif /* BANK_ALLOCATOR_HPP */
