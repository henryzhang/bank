#ifndef BANK_POINTER_HPP
#define BANK_POINTER_HPP

#include <bank/object.hpp>
#include <bank/new.hpp>

namespace bank {

template <typename T>
class pointer : public object
{
    public:
        inline explicit pointer(T* ptr) : ptr(ptr) { }
        inline explicit pointer(void) : ptr(NULL) { }
        inline virtual ~pointer(void) { if (this->ptr) { delete this->ptr; } }

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
