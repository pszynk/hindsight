
#ifndef INPUTPARAMERROR_HPP_YXITMN2O
#define INPUTPARAMERROR_HPP_YXITMN2O

#include <stdexcept>
#include <string>

namespace hisi
{
    namespace make
    {

namespace
{
std::string ERROR_PREFIX_ = "Invalid input: ";
}

class InputParamError : public std::runtime_error
{
public:
    InputParamError(const std::string& what_arg):
        std::runtime_error(ERROR_PREFIX_+what_arg)
    {}
};

    } /* make */
} /* hisi */
#endif /* end of include guard: INPUTPARAMERROR_HPP_YXITMN2O */
