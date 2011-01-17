#ifndef BANK_DETAIL_CHUNK_HPP
#define BANK_DETAIL_CHUNK_HPP

#include <cstdlib>

#include <bank/detail/types.hpp>

namespace bank {
namespace detail {

class chunk
{
    public:
        explicit chunk(void);
        virtual ~chunk(void);

        void deallocate(const size_t& address);
        void* allocate(const size_t& address);

        void combine(const chunk& other);

        bool has(const size_t& address) const;
        void set(const size_t& address, size_t length);

        bool is_combined(void) const;
        bool is_free(const size_t& size) const;

        uint32_t get_size(void) const;

        chunk& operator =(const chunk& right);

        bool operator ==(const chunk& right) const;
        bool operator !=(const chunk& right) const;
        bool operator >=(const chunk& right) const;
        bool operator <=(const chunk& right) const;

        bool operator >(const chunk& right) const;
        bool operator <(const chunk& right) const;

    private:
        size_t start;
        size_t end;

        uint32_t allocated;
        uint32_t next;

        bool combined;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_CHUNK_HPP */
