
#ifndef LOADINGERROREXCEPTION_HPP_JGL67IVF
#define LOADINGERROREXCEPTION_HPP_JGL67IVF

#include <stdexcept>
#include <string>

namespace hisi
{
    namespace load
    {


namespace
{
std::string ERROR_PREFIX_ = "Loading error: ";
}

class LoadingErrorException : public std::runtime_error
{
public:
    LoadingErrorException(const std::string& what_arg):
        std::runtime_error(ERROR_PREFIX_+what_arg)
    {}
};


    } /* load */
} /* hisi */

#endif /* end of include guard: LOADINGERROREXCEPTION_HPP_JGL67IVF */
