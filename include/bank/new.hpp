#ifndef BANK_NEW_HPP
#define BANK_NEW_HPP

#include <cstdlib>
#include <new>

void* operator new(size_t size) throw(std::bad_alloc);
void operator delete(void* pointer) throw();

namespace bank {

void* alloc(size_t size);
void free(void* pointer);

} /* namespace bank */

#endif /* BANK_NEW_HPP */
