
#ifndef POSITIONSTATEFACTORY_HPP_H8XCMZAO
#define POSITIONSTATEFACTORY_HPP_H8XCMZAO

#include "PositionIndex.hpp"
#include "../MatchStats.hpp"
#include "../tools/AcceptAllFlowFilter.hpp"

#include <tokext/Token.hpp>
#include <tokext/Flow.hpp>
#include <tokext/Position.hpp>
#include <tokext/SuffixContext.hpp>

#include <iostream>

namespace hisi
{
    namespace pos_
    {

template<
    typename ArgT_,
    class RecordT,
    class TokenMapperT,
    class FlowFilterT = AcceptAllFlowFilter>
class PositionStateFactory
{
public:
    using ArgT = ArgT_;
    using StateT = PositionIndex<RecordT>;
    using ResultT = MatchStats;

    PositionStateFactory (
            const tokext::SuffixContext& scx,
            const TokenMapperT& tokenMapper,
            /*const*/ FlowFilterT& flowFilter):
        scx_(scx), tokenMapper_(tokenMapper), flowFilter_(flowFilter)
    {};

    StateT createState() const
    {
        return StateT();
    }

    StateT initiateState(
            const ArgT& arg) const
    {
        tokext::Token token = tokenMapper_.token(arg);
        StateT state;
        using tokext::Position;
        // for all positions of token
        for(auto pidx = token.firstSufIdx(); pidx <= token.lastSufIdx(); ++pidx) {
            Position pos = scx_.prefixPosition(pidx);
            // if position in filtered out flow
            if (!flowFilter_(pos.flow())) {continue;};

            auto recPtr = state.find(pos.flow());
            if(recPtr) {
                recPtr->initiate(token, pos);
            } else {
                state.put(pos.flow(), RecordT(arg, token, pos));
            }
        }
        return state;
    }

    StateT nextState(
            const StateT& prev,
            const ArgT& arg) const
    {
        tokext::Token token = tokenMapper_.token(arg);
        StateT state;
        using tokext::Position;
        // for all positions of token
        for(auto pidx = token.firstSufIdx(); pidx <= token.lastSufIdx(); ++pidx) {
            Position pos = scx_.prefixPosition(pidx);

            auto prevRecPtr = prev.find(pos.flow());
            if(!prevRecPtr) {continue;};

            if(auto recPtr = state.find(pos.flow())) {
                recPtr->update((*prevRecPtr), token, pos);
            } else {
                state.put(pos.flow(), RecordT(arg, (*prevRecPtr), token, pos));
            }
        }
        return state;
    }

    ResultT getResult(const StateT& state) const
    {
        MatchStats stats(
                {
                    state.suspicious(),
                    flowFilter_.acceptedSuspicious(),
                    flowFilter_.ignoredSuspicious()
                },
                {
                    state.normal(),
                    flowFilter_.acceptedNormal(),
                    flowFilter_.ignoredNormal()
                });
        return stats;
    }
private:
    const tokext::SuffixContext& scx_;
    const TokenMapperT& tokenMapper_;
    /*const*/ FlowFilterT& flowFilter_;

};

    } /* pos_ */
} /* hisi */

#endif /* end of include guard: POSITIONSTATEFACTORY_HPP_H8XCMZAO */
