#include <bank/detail/tracker.hpp>
#include <bank/detail/types.hpp>
#include <bank/error.hpp>

#include <cstdlib>

namespace bank {
namespace detail {

tracker::tracker(const size_t& size) throw(error) : index(0), start(NULL)
{
    this->start = static_cast<size_t*>(std::malloc(size * sizeof(size_t)));
    if (this->start == NULL) { throw error("Could not allocate memory for internal allocation tracker"); }
    //TODO: Possibly memset?
}

tracker::~tracker(void)
{
    do
    {
        std::free(reinterpret_cast<void*>(this->start[this->index]));
        --this->index;
    } while (this->index != 0); 
    if (this->start) { std::free(this->start); }
}

void tracker::push(void* pointer) { this->start[index] = reinterpret_cast<size_t>(pointer); ++this->index; }

}} /* namespace bank::detail */
