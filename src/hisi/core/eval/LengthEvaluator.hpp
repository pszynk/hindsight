
#ifndef LENGTHEVALUATOR_HPP_8NEXMEID
#define LENGTHEVALUATOR_HPP_8NEXMEID

#include "SignatureEvaluator.hpp"

#include <cstdint>
#include <limits>

namespace hisi
{

template<class EOT, class Mapper>
class LengthEvaluator : public SignatureEvaluator<EOT>
{
public:
    using Fitness = typename EOT::Fitness;

    LengthEvaluator(const Mapper &tokenContainer, unsigned divisor =
            std::numeric_limits<uint16_t>::max()):
        mapper_(tokenContainer), divisor_(divisor)
    {}

    Fitness operator()(const EOT& eo) override
    {
        Fitness length = 0.0;
        for(const auto& gene : eo) {
            length += mapper_[gene].length();
        }
        return length / divisor_;
    }

private:
    const Mapper& mapper_;
    unsigned divisor_;
};


} /* hisi */
#endif /* end of include guard: LENGTHEVALUATOR_HPP_8NEXMEID */
