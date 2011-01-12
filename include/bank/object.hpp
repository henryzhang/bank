#ifndef BANK_OBJECT_HPP
#define BANK_OBJECT_HPP

#include <cstdlib>

namespace bank {

class object
{
    public:
        void operator delete(void*);
        void operator new(size_t);
};

} /* namespace bank */

#endif /* BANK_OBJECT_HPP */
