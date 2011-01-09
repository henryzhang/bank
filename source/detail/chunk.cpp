#include <bank/detail/chunk.hpp>
#include <bank/detail/types.hpp>

#include <limits>

#include <cstdlib>

namespace { const bank::uint32_t _64KB = std::numeric_limits<bank::uint16_t>::max() + 1; } /* internal namespace */

namespace bank {
namespace detail {

chunk::chunk(const chunk& copy) : start(copy.start), end(copy.end),
    allocated(copy.allocated), next(copy.next), combined(copy.combined) { }
chunk::chunk(void) : start(0), end(0), allocated(0), next(0), combined(false) { }
chunk::~chunk(void) { }

void chunk::deallocate(const size_t& address)
{
    if (this->allocated == 0) { return; } // Waste of a this->has otherwise, as it means we are probably combined, or at the very least unused
    if (this->has(address)) { --this->allocated; }
    if (this->allocated == 0) { this->next = 0; } // However, We do a second check because a deallocation might get us back down to 0
}

void* chunk::allocate(const size_t& size)
{
    if (this->start + this->next + size > this->end) { return NULL; } // Ring will perform NULL check, and then switch to next chunk
    ++this->allocated;
    this->next += size;
    return reinterpret_cast<void*>(this->start + this->next - size);
}

void chunk::decouple(const chunk& other)
{
    if (this->end == other.end)
    {
        this->end = other.start - 1;
        const_cast<chunk&>(other).next = const_cast<chunk&>(other).allocated = 0;
        this->combined = (this->get_size() == _64KB);
        const_cast<chunk&>(other).combined = (other.get_size() == _64KB);
    }
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

chunk& chunk::operator =(const chunk& right)
{
    this->start = right.start;
    this->end = right.end;
    this->allocated = right.allocated;
    this->next = right.next;
    this->combined = right.combined;
    return *this;
}

bool chunk::operator ==(const chunk& right) const { return this->start == right.start; }
bool chunk::operator !=(const chunk& right) const { return this->start != right.start; }
bool chunk::operator >=(const chunk& right) const { return this->start >= right.end; }
bool chunk::operator <=(const chunk& right) const { return this->end <= right.start; }

bool chunk::operator >(const chunk& right) const { return this->start > right.end; }
bool chunk::operator <(const chunk& right) const { return this->end > right.start; }

}} /* namespace bank::detail */
