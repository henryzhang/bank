#ifndef BANK_HPP
#define BANK_HPP

#include <cstdlib>

namespace bank {

void open(size_t chunks=256);
void close(void);

void* alloc(size_t size);
void free(void* pointer);

} /* namespace bank */

#endif /* BANK_HPP */
