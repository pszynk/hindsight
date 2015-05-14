
#ifndef CONFIGURE_HPP_LGANSEK9
#define CONFIGURE_HPP_LGANSEK9

#include "hisi/core/match/GlobalMatchAlgo.hpp"
#include "hisi/core/pos/PositionRuleFunc.hpp"

#include "DataInfo.hpp"

#include "make_match.hpp"
#include "make_eval.hpp"
#include "make_init.hpp"
#include "make_data.hpp"
#include "make_help.hpp"

#include <tokext/SuffixContext.hpp>
#include <tokext/utils/extraction.hpp>

#include <eoVector.h>
#include <utils/eoParser.h>
#include <utils/eoState.h>


namespace
{

using Fitness = double;

template<typename T>
using IndiContainer = eoVector<Fitness, T>;


}

namespace hisi
{

template<class RuleFunc>
void configure(int argv ,char* argc[])
{
    using ContentType = typename RuleFunc::ContentType;
    using Indi = IndiContainer<ContentType>;

    { // validation
        eoParser parser(argv, argc);
        eoState state;
        make::DataInfo info;

        // make match algorithm
        TokenPool<ContentType> tokens;

        GlobalMatchAlgo<decltype(tokens), RuleFunc>
            match_algo = make::make_match<decltype(tokens), RuleFunc>(parser, state, tokens);

        //eoEvalFunc<Indi>& eval = make::make_eval(parser, state, match_algo, tokens, Indi());

        //eoInit<Indi>& init = make::make_init<Indi>(parser, state, info);

        //eoGenOp<Indi>& op = make::make_op<Indi>(parser, state, info);

        //eoPop<Indi>& pop = make_pop(parser, state, init);

        //eoContinue<Indi>& stop_crit = make_continue(parser, state, eval);

        //eoCheckPoint<Indi>& checkpoint = make_checkpoint(parser, state, eval, stop_crit);

        // fake make_data
    } // end of validation, discard state
    // TODO make algo

    //try {
    //    tokext::SuffixContext scx = make::make_data(parser);
    //    // !!! put scx to GlobalMatchAlgo !!!
    //    match_algo.context(&scx);
    //    //tokens = make::make_tokens(parser, scx);

    //} catch (std::invalid_argument iex) {
    //    std::cerr << "ERROR:" << std::endl;
    //    std::cerr << "No folow data loaded!" << std::endl;
    //    make_help(parser);
    //    return;
    //}


    //make_help(parser);

    //run

}

} /* hisi */

#endif /* end of include guard: CONFIGURE_HPP_LGANSEK9 */
