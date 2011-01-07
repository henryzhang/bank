#ifndef BANK_DETAIL_POOL_HPP
#define BANK_DETAIL_POOL_HPP

#include <vector>
#include <new>

#include <cstdlib>

#include <bank/detail/allocator/reserve.hpp>
#include <bank/detail/chunk.hpp>

namespace bank {
namespace detail {

class chunk;

class pool
{
    public:
        typedef std::vector<chunk, allocator::reserve<chunk>> chunk_list;
        typedef chunk_list::iterator iterator;

        explicit pool(const size_t& chunks) throw(std::bad_alloc);
        virtual ~pool(void);

        void* allocate(const size_t& size);

        chunk& get_index(const size_t& index);
        chunk& get_current(void);

        size_t get_size(void) const;

        bool has(const size_t& address) const;
        bool has(void* pointer) const;

        chunk& operator [](const size_t& index);
        void operator delete(void*);
        void* operator new(size_t);

    private:
        explicit pool(const pool& copy);

        friend class collector;

        chunk_list list;
        size_t index;
        size_t size;
};

}} /* namespace bank::detail */

#endif /* BANK_DETAIL_POOL_HPP */
