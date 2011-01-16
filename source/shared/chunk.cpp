#include <bank/detail/chunk.hpp>
#include <bank/detail/types.hpp>

#include <limits>

#include <cstdlib>

namespace { const bank::uint32_t _64KB = std::numeric_limits<uint16_t>::max() + 1; } /* namespace  */

namespace bank {
namespace detail {

chunk::chunk(void) : start(0), end(0), allocated(0), next(0), combined(false) { }
chunk::~chunk(void) { }

/* Luckily these are ignored by clang and gcc */
#pragma warning(disable: 4267)
void* chunk::allocate(const size_t& size)
{
    if (this->start + this->next + size > this->end) { return NULL; }
    ++this->allocated;
    this->next += size;
    return reinterpret_cast<void*>(this->start + this->next - size);
}
#pragma warning(default: 4267)

void chunk::deallocate(const size_t& address)
{
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

bool chunk::has(const size_t& address) const { return address > this->start && address < this->end; }

void chunk::set(const size_t& address)
{
    this->start = address;
    this->end = this->start + _64KB;
}

bool chunk::is_combined(void) const { return this->combined; }

bool chunk::is_free(const size_t& size) const
{
    return this->allocated == 0 || (this->start + this->next + size) <= this->end;
}

#pragma warning(disable: 4267)
uint32_t chunk::get_size(void) const { return this->end - this->start; }
#pragma warning(default: 4267)

bool chunk::operator ==(const chunk& right) const { return this->start == right.start; }
bool chunk::operator !=(const chunk& right) const { return this->start != right.start; }
bool chunk::operator >=(const chunk& right) const { return this->start >= right.end; }
bool chunk::operator <=(const chunk& right) const { return this->start <= right.start; }

bool chunk::operator >(const chunk& right) const { return this->start > right.end; }
bool chunk::operator <(const chunk& right) const { return this->end < right.start; }

}} /* namespace bank::detail */
