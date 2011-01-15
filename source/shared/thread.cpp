#include <bank/detail/thread.hpp>

namespace bank {
namespace detail {

thread::thread(const callback& function, void* data) : function(function), data(data) { }
void thread::run(void) { if (this->function) { this->function(this->data); } }

}} /* namespace bank::detail */

