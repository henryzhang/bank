#ifndef BANK_DETAIL_ARRAY_HPP
#define BANK_DETAIL_ARRAY_HPP

#include <cstdlib>

#include <bank/detail/iterator.hpp>

namespace bank {
namespace detail {

class chunk;

/* Specialized container for the memory pool whih houses all the chunks */
class array
{
    public:
        explicit array(const size_t& size);
        virtual ~array(void);

        chunk& at(size_t index);

        iterator begin(void);
        iterator end(void);

        bool operator ==(const array& right);
        bool operator !=(const array& right);

        chunk& operator [](size_t index);

    private:
        chunk* start;
        size_t size;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_ARRAY_HPP */
