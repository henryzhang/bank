#include <bank/detail/collector.hpp>
#include <bank/detail/manager.hpp>
#include <bank/detail/pool.hpp>
#include <bank/bank.hpp>

#include <cstdlib>

namespace bank {

void open(size_t chunks) { detail::manager::instance().initialize(chunks); }

void* alloc(size_t size) { return detail::manager::instance().memory->allocate(size); }
void free(void* pointer) { detail::manager::instance().waste->remove(reinterpret_cast<size_t>(pointer)); }

} /* namespace bank */
