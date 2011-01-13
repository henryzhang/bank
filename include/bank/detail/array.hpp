#ifndef BANK_DETAIL_ARRAY_HPP
#define BANK_DETAIL_ARRAY_HPP

#include <cstdlib>

namespace bank {
namespace detail {

class chunk;

class array
{
    public:
        explicit array(const size_t& size);
        virtual ~array(void);

        chunk& at(size_t index);
        
        size_t get_size(void) const;

        bool operator ==(const array& right);
        bool operator !=(const array& right);

    private:
        chunk* start;
        size_t size;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_ARRAY_HPP */
