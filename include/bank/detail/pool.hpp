#ifndef BANK_DETAIL_POOL_HPP
#define BANK_DETAIL_POOL_HPP

#include <cstdlib>

#include <bank/detail/tracker.hpp>
#include <bank/detail/array.hpp>
#include <bank/error.hpp>

namespace bank {
namespace detail {

class pool
{
    public:
        explicit pool(const size_t& chunks) throw(error);
        virtual ~pool(void);

        void* allocate(const size_t& size);

        bool has(const size_t& address) const;
        bool has(void* pointer) const;

        void operator delete(void*);
        void* operator new(size_t);

    private:
        explicit pool(const pool& copy);

        friend class manager;

        tracker allocs;
        array list;

        size_t index;
        size_t size;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_POOL_HPP */
