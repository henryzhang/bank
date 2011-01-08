#include <bank/error.hpp>

namespace bank {

error::error(const char* message) throw() : message(message) { }
error::~error(void) throw() { }

const char* error::what(void) const throw() { return this->message; }

} /* namespace bank */
