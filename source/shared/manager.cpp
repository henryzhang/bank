#include <bank/detail/collector.hpp>
#include <bank/detail/manager.hpp>
#include <bank/detail/types.hpp>
#include <bank/detail/pool.hpp>

#include <cstdlib>

namespace bank {
namespace detail {

manager::manager(void) : waste(NULL), memory(NULL), initialized(false) { }
manager::~manager(void) { }

void manager::initialize(const size_t& chunks)
{
    if (this->initialized) { return; }

    this->memory = new pool(chunks);
    this->waste = new collector(*this->memory);
    this->initialized = true;
}

void manager::shutdown(void)
{
    if (this->initialized)
    {
        delete this->waste;
        delete this->memory;
    }
}

manager& manager::instance(void)
{
    static manager man;
    return man;
}

}} /* namespace bank::detail */
