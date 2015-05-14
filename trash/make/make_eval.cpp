
#include "make_eval.hpp"

#include "hisi/core/eval/SSPNMatchConverter.hpp"

namespace hisi
{
    namespace help_
    {

MatchConverter<double>* readConversion(const eoParamParamType& param)
{
    using std::stod;
    if(param.first == std::string("SSPN")) {
        double p0 = 1.0, p1 = 1.0, p2 = 1.0, p3 = 1.0;
        try {
            switch (param.second.size()) {
                default : throw make::InputParamError("SSPN converter, wrong number of parameters");
                case 0  : return new SSPNMatchConverter();
                case 4  : p3 = stod(param.second[3]);
                case 3  : p2 = stod(param.second[2]);
                case 2  : p1 = stod(param.second[1]);
                case 1  : p0 = stod(param.second[0]);
            }

            return new SSPNMatchConverter(p0, p1, p2, p3);

        } catch (std::out_of_range r) {
            throw make::InputParamError(
                    std::string("SSPN parameter out of range : ") +  r.what());

        } catch (std::invalid_argument i) {
            throw make::InputParamError(
                    std::string("SSPN parameter should be floating point : ") +  i.what());
        }
    }
    throw make::InputParamError("unknown converter");
}

MatchConverter<double>* readPenalty(const eoParamParamType& param)
{
    if(param.first.empty()) {
        return nullptr;
    }

    throw make::InputParamError("unknown penalty");
}

    } /* help_ */
} /* hisi */
