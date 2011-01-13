#include <bank/detail/array.hpp>
#include <bank/detail/chunk.hpp>

#include <cstdlib>

namespace bank {
namespace detail {

array::array(const size_t& size) : start(new chunk[size]), size(size) { }
array::~array(void) { delete[] this->start; }

chunk& array::at(size_t index)
{
    if (index >= size) { return *this->start; }
    return this->start[index];
}

size_t array::get_size(void) const { return this->size; }

bool array::operator ==(const array& right)
{
    return (this->start == right.start) && (this->size == right.size);
}

bool array::operator !=(const array& right)
{
    return (this->start != right.start) && (this->size != right.size);
}

}} /* namespace bank */
