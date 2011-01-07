#include <bank/object.hpp>
#include <bank/new.hpp>

namespace bank {

void object::operator delete(void* pointer) { free(pointer); }
void* object::operator new(size_t size) { return alloc(size); }

} /* namespace bank */
