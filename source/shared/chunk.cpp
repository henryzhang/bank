#include <bank/detail/chunk.hpp>
#include <bank/detail/types.hpp>

#include <limits>

#include <cstdlib>

namespace bank {
namespace detail {

chunk::chunk(void) : start(0), end(0), allocated(0), next(0), combined(false) { }
chunk::~chunk(void) { }

void* chunk::allocate(const size_t& size)
{
    if (this->start + this->next + size > this->end) { return NULL; }
    ++this->allocated;
    this->next += size;
    return reinterpret_cast<void*>(this->start + this->next - size);
}

void chunk::deallocate(const size_t& address)
{
    if (this->allocated == 0) { return; }
    if (this->has(address)) { --this->allocated; }
    if (this->allocated == 0) { this->next = 0; }
}

void chunk::combine(const chunk& other)
{
    if ((this->end + 1 == other.start) && (other.next == 0) && (other.allocated == 0))
    {
        this->end = other.end;
        this->combined = const_cast<chunk&>(other).combined = true;
    }
}

void chunk::set(const size_t& address, const size_t& size)
{
    this->start = address;
    this->end = this->start + size;
}

bool chunk::has(const size_t& address) const { return address > this->start && address < this->end; }

bool chunk::is_combined(void) const { return this->combined; }
bool chunk::is_free(void) const { return this->allocated == 0; }

uint32_t chunk::get_size(void) const { return this->end - this->start; }

bool chunk::operator ==(const chunk& right) const { return this->start == right.start; }
bool chunk::operator !=(const chunk& right) const { return this->start != right.start; }
bool chunk::operator >=(const chunk& right) const { return this->start >= right.end; }
bool chunk::operator <=(const chunk& right) const { return this->start <= right.start; }

bool chunk::operator >(const chunk& right) const { return this->start > right.end; }
bool chunk::operator <(const chunk& right) const { return this->end < right.start; }

}} /* namespace bank::detail */
