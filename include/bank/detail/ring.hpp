#ifndef BANK_DETAIL_RING_HPP
#define BANK_DETAIL_RING_HPP

namespace bank {
namespace detail {

/* specialized circular buffer, used for the collector's delete queue
 * Due to its specialization, it shouldn't be used outside of bank
 * As it holds a static amount of memory (alloc.max_size()), and simply "shifts"
 * about as the ring moves the "current front". A pop_front, simply moves the current index N to N + 1
 */
template <typename T, typename alloc>
class ring
{
    public:
        typedef typename alloc::difference_type difference_type;
        typedef typename alloc::value_type value_type;
        typedef typename alloc::size_type size_type;

        typedef typename alloc::const_reference const_reference;
        typedef typename alloc::const_pointer const_pointer;

        typedef typename alloc::reference reference;
        typedef typename alloc::pointer pointer;

        typedef typename alloc::allocator_type;

        inline explicit ring(const allocator_type& alloc = allocator_type());
        inline virtual ~ring(void);

        inline allocator_type  get_allocator(void) const;
        inline allocator_type& get_allocator(void);

        inline const_reference at(size_type index) const;
        inline reference at(size_type index);

        inline reference front(void);
        inline reference back(void);

        inline void push_back(void);
        inline void pop_front(void);

        size_type size(void) const;

        inline bool empty(void) const;
        inline bool full(void) const;
        
        inline const_reference operator [](size_type index) const;
        inline reference operator [](size_type index);

        inline bool operator ==(const ring<T, alloc>& right);
        inline bool operator !=(const ring<T, alloc>& right);
        inline bool operator <=(const ring<T, alloc>& right);
        inline bool operator >=(const ring<T, alloc>& right);
        inline bool operator <(const ring<T, alloc>& right);
        inline bool operator >(const ring<T, alloc>& right);

        inline ring<T, alloc>& operator =(const ring<T, alloc>& right);
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_RING_HPP */
