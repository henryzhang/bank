#ifndef BANK_POINTER_HPP
#define BANK_POINTER_HPP

#include <bank/object.hpp>
#include <bank/bank.hpp>

namespace bank {

template <typename T>
class pointer : public object
{
    public:
        template <typename U> inline explicit pointer(const U& value) : ptr(new(alloc(sizeof(T))) ptr(value)) { }
        inline explicit pointer(const T& copy) : ptr(new(alloc(sizeof(T))) ptr(copy)) { }
        inline explicit pointer(T* ptr) : ptr(ptr) { }
        inline explicit pointer(void) : ptr(new(alloc(sizeof(T))) ptr()) { }
        inline virtual ~pointer(void) { this->ptr->~T(); bank::free(this->ptr); }

        inline T* get(void) const { return this->ptr; }

        inline T* operator ->(void) const { return this->get(); }
        inline T& operator *(void) const { return *this->get(); }

        inline bool operator ==(const pointer<T>& right) { return *this == right; }
        inline bool operator !=(const pointer<T>& right) { return *this != right; }

        inline bool operator ==(const T& right) { return *this->ptr == right; }
        inline bool operator !=(const T& right) { return *this->ptr != right; }

        inline bool operator ==(const T* right) { return this->ptr == right; }
        inline bool operator !=(const T* right) { return this->ptr != right; }

    private:
        T* ptr;
};

} /* namespace bank */

#endif /* BANK_POINTER_HPP */
