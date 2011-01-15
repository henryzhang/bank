#include <bank/detail/tracker.hpp>
#include <bank/detail/types.hpp>
#include <bank/error.hpp>

#include <cstdlib>

#include <iostream>

namespace bank {
namespace detail {

tracker::tracker(const size_t& size) throw(error) : index(0), start(NULL)
{
    this->start = static_cast<size_t*>(std::calloc(size, sizeof(size_t)));
    std::cout << "tracker:" << this->start << std::endl;
    if (this->start == NULL) { throw error("Could not allocate memory for internal allocation tracker"); }
    //TODO: Possibly memset?
}

tracker::~tracker(void)
{
    std::cout << "Deleting trackers" << std::endl;
    do
    {
        void* buffer = reinterpret_cast<void*>(this->start[this->index]);
        if (buffer == NULL) { --this->index; continue; }
        std::cout << "freeing: " << this->start[this->index] << std::endl;
        std::free(buffer);
        --this->index;
    } while (this->index > 0);
    std::cout << "Loop finished!" << std::endl;
    //if (this->start) { std::free(this->start); }
}

void tracker::push(void* pointer) { std::cout << "tracker pushed: " << pointer << std::endl; this->start[index] = reinterpret_cast<size_t>(pointer); ++this->index; }

}} /* namespace bank::detail */
