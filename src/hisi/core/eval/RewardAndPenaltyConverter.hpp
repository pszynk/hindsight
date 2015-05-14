
#ifndef REWARDANDPENALTYCONVERTER_HPP_LSCBA4QX
#define REWARDANDPENALTYCONVERTER_HPP_LSCBA4QX

#include "MatchConverter.hpp"

namespace hisi
{

template<typename T>
class RewardAndPenaltyConverter : public MatchConverter<T>
{
public:
    using ReturnType = T;

    RewardAndPenaltyConverter(MatchConverter<T>& reward, MatchConverter<T>& penalty):
        reward_(reward), penalty_(penalty)
    {}

    ReturnType operator()(const MatchResult& stats) override
    {
        if(stats.interrupt()) {
            return penalty_(stats);
        }
        return reward_(stats);
    }

private:
    MatchConverter<T>& reward_;
    MatchConverter<T>& penalty_;
};
} /* hisi */

#endif /* end of include guard: REWARDANDPENALTYCONVERTER_HPP_LSCBA4QX */
