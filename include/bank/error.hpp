#ifndef BANK_ERROR_HPP
#define BANK_ERROR_HPP

#include <exception>

namespace bank {

class error : public std::exception
{
    public:
        explicit error(const char* message) throw();
        virtual ~error(void) throw();

        virtual const char* what(void) const throw();
    private:
        const char* message;
};

} /* namespace bank */

#endif /* BANK_ERROR_HPP */
