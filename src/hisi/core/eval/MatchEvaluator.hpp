
#ifndef MATCHEVALUATOR_HPP_3RBMDZ6S
#define MATCHEVALUATOR_HPP_3RBMDZ6S


#include "SignatureEvaluator.hpp"

#include "MatchConverter.hpp"

namespace hisi
{

template<class EOT, class MatcherT>
class MatchEvaluator : public SignatureEvaluator<EOT>
{
public:
    using Fitness = typename EOT::Fitness;

    MatchEvaluator (
            MatcherT& matcher,
            MatchConverter<Fitness>& converter):
        matcher_(matcher),
        converter_(converter)
    {}

    Fitness operator()(const EOT& eo) override
    {
        return converter_(matcher_.match(eo.begin(), eo.end()));
    }

private:
    MatcherT& matcher_;
    MatchConverter<Fitness>& converter_;
};


} /* hisi */

#endif /* end of include guard: MATCHEVALUATOR_HPP_3RBMDZ6S */
