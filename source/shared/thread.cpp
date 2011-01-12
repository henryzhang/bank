#include <bank/detail/thread.hpp>

namespace bank {
namespace detail {

thread::thread(const callback& function, void* data) : function(function), data(data) { }
thread::run(void) { if (this->func) { this->func(this->data); } }

}} /* namespace bank::detail */

