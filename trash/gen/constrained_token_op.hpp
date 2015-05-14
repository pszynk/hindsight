
#ifndef CONSTRAINED_TOKEN_OP_HPP_UFO9HD1H
#define CONSTRAINED_TOKEN_OP_HPP_UFO9HD1H

#include "ConstrainedToken.hpp"

#include <eoInit.h>

namespace hisi
{


template<typename T>
class ConstrainedTokenInit: public eoInit<ConstrainedToken<T>>
{
public:
    using TokenIdxT = typename ConstrainedToken<T>::TokenIdxT;
    using PredArgT = typename ConstrainedToken<T>::PredArgT;
    ConstrainedTokenInit(
            eoInit<TokenIdxT>& tokenIdxInit,
            eoInit<PredArgT>& predArgInit):
        tokenIdxInit_(tokenIdxInit), predArgInit_(predArgInit)
    {}

    void operator()(ConstrainedToken<T>& constToken) override {
        tokenIdxInit_(constToken.tokenIdxRef());
        predArgInit_(constToken.predArgRef());
    }

private:
    eoInit<TokenIdxT>& tokenIdxInit_;
    eoInit<PredArgT>& predArgInit_;
};

template<typename T>
class ConstrainedTokenMutation: public eoMonOp<ConstrainedToken<T>>
{
public:
    using TokenIdxT = typename ConstrainedToken<T>::TokenIdxT;
    using PredArgT = typename ConstrainedToken<T>::PredArgT;
    ConstrainedTokenMutation(
            eoMonOp<TokenIdxT>& tokenIdxMutation,
            eoMonOp<PredArgT>& predArgMutation):
        tokenIdxMutation_(tokenIdxMutation), predArgMutation_(predArgMutation)
    {}

    bool operator()(ConstrainedToken<T>& constToken) override {
        return tokenIdxMutation_(constToken.tokenIdxRef()) |
            predArgMutation_(constToken.predArgRef());
    }

private:
    eoMonOp<TokenIdxT>& tokenIdxMutation_;
    eoMonOp<PredArgT>& predArgMutation_;
};
} /* hisi */

#endif /* end of include guard: CONSTRAINED_TOKEN_OP_HPP_UFO9HD1H */
