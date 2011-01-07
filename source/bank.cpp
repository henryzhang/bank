#include <bank/detail/manager.hpp>
#include <bank/bank.hpp>

#include <cstdlib>

namespace bank {

void open(const size_t& chunks) { detail::manager::instance().initialize(chunks); }
void open(void) { open(256); }

void bailout(void) { }

} /* namespace bank */
