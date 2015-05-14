
#ifndef STATEMATCHER_HPP_TMOGL8U3
#define STATEMATCHER_HPP_TMOGL8U3

#include "MatchStats.hpp"
#include "pos/PositionStateFactory.hpp"
#include "tools/AcceptAllFlowFilter.hpp"

#include <tokext/SuffixContext.hpp>
//std
#include <iterator>

#include <iostream>

namespace hisi
{

template<class StateFactoryT>
class StateMatcher
{
private:
    StateFactoryT factory_;

public:
    StateMatcher(
            StateFactoryT factory):
        factory_(factory)
    {}

    template<typename IterT>
    //MatchStats match(IterT fromIt, IterT toIt) const
    auto match(IterT fromIt, IterT toIt)
        -> decltype(this->factory_.getResult(this->factory_.createState())) const
    {
        assert(fromIt != toIt);

        auto state = factory_.initiateState((*fromIt));

        //std::cout << state << std::endl;

        for (auto it = std::next(fromIt); it != toIt; ++it) {
            state = factory_.nextState(state, (*it));
        //std::cout << state << std::endl;
        }

        return factory_.getResult(state);
    }

};

template<
    typename ArgT,
    class RecordT,
    class TokenMapperT,
    class FlowFilterT = AcceptAllFlowFilter>
StateMatcher<
    pos_::PositionStateFactory<
        ArgT,
        RecordT,
        TokenMapperT,
        FlowFilterT
    >
>
make_PositionMatcher(
        const tokext::SuffixContext& scx,
        const TokenMapperT& tokenMapper,
        /*const*/ FlowFilterT& flowFilter)
{
    using FactoryT =
        pos_::PositionStateFactory<ArgT, RecordT, TokenMapperT, FlowFilterT>;
    return StateMatcher<FactoryT>(FactoryT(scx, tokenMapper, flowFilter));
}

} /* hisi */

#endif /* end of include guard: STATEMATCHER_HPP_TMOGL8U3 */
