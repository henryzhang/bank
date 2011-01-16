#include <bank/detail/tracker.hpp>
#include <bank/detail/types.hpp>
#include <bank/error.hpp>

#include <cstdlib>

namespace bank {
namespace detail {

tracker::tracker(const size_t& size) throw(error) : index(0), start(NULL)
{
    this->start = static_cast<size_t*>(std::calloc(size, sizeof(size_t)));
    if (this->start == NULL) { throw error("Could not allocate memory for internal allocation tracker"); }
}

tracker::~tracker(void)
{
    do
    {
        void* buffer = reinterpret_cast<void*>(this->start[this->index]);
        if (buffer == NULL) { --this->index; continue; }
        std::free(buffer);
        --this->index;
    } while (this->index > 0);

    void* buffer = reinterpret_cast<void*>(this->start[0]);
    if (buffer != NULL) { std::free(buffer); }
    if (this->start) { std::free(this->start); }
}

void tracker::push(void* pointer) { this->start[index] = reinterpret_cast<size_t>(pointer); ++this->index; }

}} /* namespace bank::detail */
