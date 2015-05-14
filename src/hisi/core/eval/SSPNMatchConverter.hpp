
#ifndef SSPNMATCHCONVERTER_HPP_ZOBIUINX
#define SSPNMATCHCONVERTER_HPP_ZOBIUINX

#include "MatchConverter.hpp"

#include <eoObject.h>

namespace hisi
{


class SSPNMatchConverter :
    public MatchConverter<double>,
    public eoObject
{
public:

    using ReturnType = double;

    SSPNMatchConverter(
            bool sp_p = 1.0, bool se_p = 1.0,
            bool ppv_p = 1.0, bool npv_p = 1.0) :
        sp_p_(sp_p), se_p_(se_p), ppv_p_(ppv_p), npv_p_(npv_p)
    {};

    double operator()(const MatchResult& stats) override;
    std::string className() const  override {return "SSPNMatchConverter";};

private:
    bool sp_p_, se_p_, ppv_p_, npv_p_;
};

} /* hisi */


#endif /* end of include guard: SSPNMATCHCONVERTER_HPP_ZOBIUINX */
