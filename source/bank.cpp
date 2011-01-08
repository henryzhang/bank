#include <bank/detail/manager.hpp>
#include <bank/bank.hpp>

#include <cstdlib>

namespace bank {

void open(size_t chunks) { detail::manager::instance().initialize(chunks); }

} /* namespace bank */
