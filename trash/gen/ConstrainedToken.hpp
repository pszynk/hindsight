
#ifndef CONSTRAINEDTOKEN_HPP_W87LXP3Y
#define CONSTRAINEDTOKEN_HPP_W87LXP3Y


#include "Interval.hpp"
#include "ConstrainedToken.hpp"

#include <functional>
#include <cstddef>

namespace hisi
{

template<typename T>
class ConstrainedToken
{
public:
    using TokenIdxT = std::size_t;
    using PredArgT = T;

    ConstrainedToken (TokenIdxT tokenIdx = 0, PredArgT predArg = {}):
        tokenIdx_(tokenIdx), predArg_(predArg)
    {};


    explicit operator TokenIdxT() const
    { return tokenIdx_; }

    TokenIdxT tokenIdx() const
    { return tokenIdx_; }

    TokenIdxT& tokenIdxRef()
    { return &tokenIdx_; }

    explicit operator PredArgT() const
    { return predArg_; }

    PredArgT predArg() const
    { return predArg_; }

    PredArgT& predArgRef()
    { return &predArg_; }

    void tokenIdx(TokenIdxT tokenIdx)
    { tokenIdx_ = tokenIdx; }

    void predArg(PredArgT predArg)
    { predArg_= predArg; }

private:
    TokenIdxT tokenIdx_;
    PredArgT predArg_;
};

template<typename PredFuncT>
class ConstrainedTokenPredicate
{
public:
    using PredArgT = typename PredFuncT::first_argument_type;
    ConstrainedTokenPredicate(
            const ConstrainedToken<PredArgT>& bounded,
            PredFuncT pred = {}):
        predArg_(bounded.predArg()),
        pred_(pred)
    {}

    bool check(unsigned pos)
    {
        return pred(predArg_, pos);
    }

private:
    PredArgT predArg_;
    PredFuncT pred_;
};

typedef ConstrainedTokenPredicate<InInterval> IntervalConstrainedTokenPredicate;
typedef ConstrainedTokenPredicate<std::less_equal<std::size_t>> LowerConstrainedTokenPredicate;
typedef ConstrainedTokenPredicate<std::greater_equal<std::size_t>> UpperConstrainedTokenPredicate;

} /* hisi */

#endif /* end of include guard: CONSTRAINEDTOKEN_HPP_W87LXP3Y */
