#include <bank/detail/collector.hpp>
#include <bank/detail/manager.hpp>
#include <bank/detail/pool.hpp>
#include <bank/new.hpp>

#include <cstdlib>

//void* operator new(size_t size) throw(std::bad_alloc) { return bank::alloc(size); }
//void operator delete(void* pointer) throw() { bank::free(pointer); }

namespace bank {

void* alloc(size_t size) { return detail::manager::instance().memory->allocate(size); }
void free(void* pointer) { detail::manager::instance().waste->remove(reinterpret_cast<size_t>(pointer)); }

} /* namespace bank */
