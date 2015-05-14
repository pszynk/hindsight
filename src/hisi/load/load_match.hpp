
#ifndef LOAD_MATCH_HPP_VMBLQELW
#define LOAD_MATCH_HPP_VMBLQELW

#include "hisi/core/match/GlobalMatchAlgo.hpp"

#include <utils/eoState.h>

#include <libconfig.h++>

namespace hisi
{
    namespace load
    {

template<class RuleFunc, class Mapper>
GlobalMatchAlgo<RuleFunc, Mapper> load_match_algo(
    const libconfig::Config& /*cfg*/, eoState& /*state*/, const Mapper& mapper)
{
    GlobalMatchAlgo<RuleFunc, Mapper> match_algo(mapper);

    //TODO: filter [(18/02/15 19:19:43) - pawel]

    return std::move(match_algo);
}

    } /* load */
} /* hisi */

#endif /* end of include guard: LOAD_MATCH_HPP_VMBLQELW */
