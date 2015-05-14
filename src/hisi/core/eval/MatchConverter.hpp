
#ifndef MATCHCONVERTER_HPP_VEILYWPF
#define MATCHCONVERTER_HPP_VEILYWPF

#include "hisi/core/match/MatchResult.hpp"

#include <eoFunctor.h>

namespace hisi
{

template <typename RetT>
    struct MatchConverter : public eoUF<const MatchResult&, RetT>
{
    using ReturnType = RetT;
};


} /* hisi */

#endif /* end of include guard: MATCHCONVERTER_HPP_VEILYWPF */
