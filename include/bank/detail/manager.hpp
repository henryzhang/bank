#ifndef BANK_DETAIL_MANAGER_HPP
#define BANK_DETAIL_MANAGER_HPP

#include <cstdlib>

namespace bank {

extern void* alloc(size_t);
extern void free(void*);

extern void open(const size_t&);

namespace detail {

class collector;
class pool;

class manager
{
    public:
        virtual ~manager(void);

    private:
        explicit manager(const manager& copy);
        explicit manager(void);

        void initialize(const size_t& chunks);
        static manager& instance(void);

        friend class detail::collector;

        friend void* bank::alloc(size_t);
        friend void bank::free(void*);

        friend void bank::open(const size_t&);

        collector* waste;
        pool* memory;

        bool initialized;
};

}} /* namespace bank:detail */

#endif /* BANK_DETAIL_MANAGER_HPP */
