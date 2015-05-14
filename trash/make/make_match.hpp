
#ifndef MAKE_MATCH_HPP_CUTJXFOE
#define MAKE_MATCH_HPP_CUTJXFOE

#include "hisi/core/match/GlobalMatchAlgo.hpp"

#include "make_filter.hpp"

#include <utils/eoParser.h>
#include <utils/eoState.h>

namespace hisi
{
    namespace make
    {

template<class Mapper, class RuleFunc>
GlobalMatchAlgo<Mapper, RuleFunc> make_match(
        eoParser& parser, eoState& state, Mapper mapper)
{
    GlobalMatchAlgo<Mapper, RuleFunc> match_algo(mapper);
    match_algo.filter(&make_filter(parser, state));
    return std::move(match_algo);
}
    } /* make */
} /* hisi */

#endif /* end of include guard: MAKE_MATCH_HPP_CUTJXFOE */
