#include <bank/object.hpp>
#include <bank/bank.hpp>

namespace bank {

void object::operator delete(void* pointer) { bank::free(pointer); }
void* object::operator new(size_t size) { return bank::alloc(size); }

} /* namespace bank */
