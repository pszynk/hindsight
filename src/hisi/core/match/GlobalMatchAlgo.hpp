
#ifndef GLOBALMATCHALGO_HPP_GSVPUCIT
#define GLOBALMATCHALGO_HPP_GSVPUCIT

#include "hisi/core/tools/AcceptAllFlowFilter.hpp"
#include "hisi/core/tools/TokenPool.hpp"
#include "MatchResult.hpp"
#include "PositionIndex.hpp"
#include "MatchingState.hpp"


#include <tokext/SuffixContext.hpp>
#include <tokext/Token.hpp>

namespace hisi
{


template<class PosRuleFunc, class TokenMapper>
class GlobalMatchAlgo
{
public:
    GlobalMatchAlgo(
            const tokext::SuffixContext *const scx,
            const TokenMapper& mapper,
            double threshold = 0.0):
        scx_(scx), mapper_(mapper), threshold_(threshold)
    {};

    GlobalMatchAlgo(
            const TokenMapper& mapper,
            double threshold = 0.0):
        mapper_(mapper), threshold_(threshold)
    {};

    void context(const tokext::SuffixContext&  scx) {scx_ = &scx;}
    void filter(FlowFilter& filter) {filter_ = &filter;}
    void threshold(double d = 0.0) {threshold_ = d;}

    template<typename IterT>
    MatchResult match(IterT begContIt, IterT endContIt);

    using detailed_t = std::pair<MatchResult, std::vector<tokext::Position::idx_t>>;
    template<typename IterT>
    detailed_t detailed(IterT begContIt, IterT endContIt);


private:
    const tokext::SuffixContext *scx_ = nullptr;
    const TokenMapper& mapper_;

    FlowFilter *filter_ = nullptr;

    double threshold_;

    bool noStart_ = true;

    //---------------------------------

    template<typename IterT>
    using helper_t_ = std::vector<std::pair<IterT, int>>;

    template<typename IterT>
    using precomp_t_ = std::tuple<PositionIndex, MatchResult, helper_t_<IterT>>;

    template<typename IterT>
    precomp_t_<IterT> precompute_(IterT begContIt, IterT endContIt);

    bool isUnderThreshold_(const PositionIndex& index,
            const MatchResult& result, double threshold);

    using record_t = PositionIndex::record_t;
    using pos_t = PositionIndex::pos_t;
    using PosMState = MatchingState<pos_t>;



    //TODO: Można zrobić tak żeby nie zwracał bool, ale najlepszy
    // MatchingState, i z takiego MS zrobić jakąś ocene z większą liczbą opci niż
    // fail/success [(06/02/15 15:19:24) - pawel]
    template<typename IterT>
    bool recursiveMatch_(
        IterT begContLenIt, IterT endContIt,
        record_t::const_iterator recIt,
        const PosMState& state) const;
};

template<class R, class M>
template<typename IterT>
//MatchResult GMA<R, M>::
MatchResult GlobalMatchAlgo<R, M>::
match(IterT begContIt, IterT endContIt)
{
    PositionIndex index;
    MatchResult result;
    helper_t_<IterT> helper;
    std::tie(index, result, helper) = precompute_(begContIt, endContIt);
    // now we have prepared index!
    //
    //std::cout << index << std::endl << std::endl;
    // state has last, and next and valid ev(size)
    for(const auto& recPair : index.suspicious()) {
        PosMState mstate;
        if(recursiveMatch_(
                    helper.begin(),
                    helper.end(),
                    recPair.second.begin(),
                    mstate))
        {
            result.suspicious().incMatched();
        }
    }

    for(const auto& recPair : index.normal()) {
        PosMState mstate;
        if(recursiveMatch_(
                    helper.begin(),
                    helper.end(),
                    recPair.second.begin(),
                    mstate))
        {
            result.normal().incMatched();
        }
    }
    return std::move(result);
}

template<class R, class M>
template<typename IterT>
auto GlobalMatchAlgo<R, M>::
detailed(IterT begContIt, IterT endContIt) -> detailed_t
{
    PositionIndex index;
    MatchResult result;
    helper_t_<IterT> helper;
    std::tie(index, result, helper) = precompute_(begContIt, endContIt);
    // now we have prepared index!

    std::vector<tokext::Position::idx_t> flows;
    //std::cout << index << std::endl << std::endl;
    // state has last, and next and valid ev(size)
    for(const auto& recPair : index.suspicious()) {
        PosMState mstate;
        if(recursiveMatch_(
                    helper.begin(),
                    helper.end(),
                    recPair.second.begin(),
                    mstate))
        {
            result.suspicious().incMatched();
            flows.push_back(recPair.first);
        }
    }

    for(const auto& recPair : index.normal()) {
        PosMState mstate;
        if(recursiveMatch_(
                    helper.begin(),
                    helper.end(),
                    recPair.second.begin(),
                    mstate))
        {
            result.normal().incMatched();
            flows.push_back(recPair.first);
        }
    }
    return std::make_pair(std::move(result), std::move(flows));
}

template<class R, class M>
template<typename IterT>
auto GlobalMatchAlgo<R, M>::
precompute_(IterT begContIt, IterT endContIt) -> precomp_t_<IterT>
{
    if(begContIt == endContIt) {
        throw std::runtime_error("GlobalMatchAlgo: iterators - first == last!");
    }

    PositionIndex index;
    MatchResult result(
            {scx_->suspiciousFlowCount(),
            scx_->suspiciousFlowCount()},
            {scx_->normalFlowCount(),
            scx_->normalFlowCount()});

    helper_t_<IterT> helper;

    if(filter_) {
        filter_->randomize(scx_->suspiciousFlowCount(),
                scx_->normalFlowCount());
    }

    // start with first one!
    auto& token = mapper_[(*begContIt)];
    // add to helper what iter points to and length of token
    helper.push_back(std::make_pair(begContIt, (int)token.length())); // TODO token len should be u16
    // set index to next level
    index.nextLevel();
    // iterate over token positions
    for(auto pidx = token.firstSufIdx();
            pidx <= token.lastSufIdx(); ++pidx) {
        tokext::Position pos = scx_->prefixPosition(pidx);

        // put pos on index
        auto place = index.contains(pos);

        // index has this flow already
        if(place.first) {
            index.put(place.second, pos);
            continue;
        }

        // if this is first time we see this flow, check if it will be ignored
        if(filter_ && !filter_->operator()(pos.flow())) {
        // increment considered counter
            result.stats(pos.flow().isSuspicious()).decConsidered();
            continue;
        }

        // record position if not ignored!
        index.put(pos);

    }

    // check threshold
    if(isUnderThreshold_(index, result, threshold_)) {
        result.interrupt() = {index.level(), (unsigned)std::distance(begContIt, endContIt)};
    }
    //std::cout << index << std::endl << std::endl;

    // iterate over contents
    for(IterT it = std::next(begContIt); it != endContIt; ++it) {
        auto& token = mapper_[(*it)];
        // add to helper what iter points to and length of token
        helper.push_back(std::make_pair(it, token.length()));
        // set index to next level
        index.nextLevel();
        // iterate over token positions
        for(auto pidx = token.firstSufIdx();
                pidx <= token.lastSufIdx(); ++pidx) {
            tokext::Position pos = scx_->prefixPosition(pidx);

            // put pos on index
            index.put(pos);
        }
        //std::cout << index << std::endl << std::endl;

        // check threshold
        if(isUnderThreshold_(index, result, threshold_)) {
            //TODO: PRZERWAĆ!!!! [(22/02/15 19:04:07) - pawel]
            result.interrupt() = {index.level(), (unsigned)std::distance(begContIt, endContIt)};
        }
    }
    index.nextLevel();
    return std::make_tuple(std::move(index), std::move(result), std::move(helper));
}

template<class PosRuleFunc, class M>
template<typename IterT>
bool GlobalMatchAlgo<PosRuleFunc, M>::
recursiveMatch_(
        IterT begContLenIt, IterT endContLenIt,
        record_t::const_iterator recIt,
        const PosMState& state) const
{
    if (begContLenIt == endContLenIt) {return true;}

    // for each position in this record level;
    for(auto posIt = recIt->begin(); posIt != recIt->end(); ++posIt) {

        // check if position fulfils the rule!
        if ((this->noStart_ && state.size() == 0) ||
                PosRuleFunc::check({
                    state.point(),           // point from where start check (0 if first token)
                    (*posIt)},               // starting position of token
                    //begContLenIt->second},   // length of the token
                    *begContLenIt->first))   // whole ContT object to extract rule parameters from
        {
            PosMState newState = state.next({(pos_t)(*posIt), (pos_t)begContLenIt->second});
            if (newState) {
                if (recursiveMatch_(std::next(begContLenIt), endContLenIt, std::next(recIt), newState)) {
                    return true;
                }
            }
        }
    }
    return false;
}


template<class R, class M>
bool GlobalMatchAlgo<R, M>::
isUnderThreshold_(const PositionIndex& index,
        const MatchResult& result, double threshold)
{
    if(result.suspicious().considered() == 0) {
        return false;
    }

    return (double(index.suspiciousCount()) /
            result.suspicious().considered()) < threshold;

}

} /* hisi */

#endif /* end of include guard: GLOBALMATCHALGO_HPP_GSVPUCIT */
