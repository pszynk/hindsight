
#ifndef LOADER_HPP_KB32PURO
#define LOADER_HPP_KB32PURO

#include "hisi/core/tools/TokenPool.hpp"
#include "hisi/core/match/GlobalMatchAlgo.hpp"
#include "hisi/core/pos/PositionRuleFunc.hpp"

#include "DataInfo.hpp"

#include "load_match.hpp"
#include "load_eval.hpp"
#include "load_init.hpp"
#include "load_op.hpp"

#include "hisi/utils/sig_gen.hpp"

#include "hisi/make/make_data.hpp"
#include "hisi/make/make_tokens.hpp"

#include <tokext/SuffixContext.hpp>

#include <eoVector.h>
#include <utils/eoState.h>

#include <do/make_pop.h>
#include <do/make_continue.h>
#include <do/make_checkpoint.h>
#include <do/make_algo_scalar.h>
#include <do/make_run.h>

#include <libconfig.h++>

namespace hisi
{
    namespace load
    {

template<class RuleFunc>
class Loader
{
public:
    Loader (const libconfig::Config& cfg): cfg_(cfg) {}

    int run(eoParser& parser);

private:
    const libconfig::Config& cfg_;

    using Fitness = double;

    template<typename T>
    using IndiContainer = eoVector<Fitness, T>;

    using ContentType = typename RuleFunc::ContentType;
    using Indi = IndiContainer<ContentType>;
    using Atom = ContentType;

};

struct bla {
    int a;
    static int bbb();
};


template<class RuleFunc>
int Loader<RuleFunc>::run(eoParser& parser)
{
    try {
        eoState state;
        load::DataInfo info;

        // load match algorithm
        TokenPool<ContentType> tokens;


        unsigned maxLen = 0;
        tokext::InputData idata = make::make_data(parser, maxLen);


        std::cout << "Wczytano: " << idata.suspiciousDataSize() / 1024
            << " kb podejrzanych" << std::endl;
        std::cout << "Wczytano: " << idata.normalDataSize() / 1024
            << " kb normalnych" << std::endl;

        tokext::SuffixContext scx(std::move(idata));
        scx.freeNormalData();

        tokens = make::make_tokens<decltype(tokens)>(parser, scx);

        info.maxFlowLength = maxLen;
        info.tokenCount = tokens.size();


        GlobalMatchAlgo<RuleFunc, decltype(tokens)>
            match_algo = load_match_algo<RuleFunc, decltype(tokens)>(cfg_, state, tokens);
        eoEvalFunc<Indi>& eval = load::load_eval(cfg_, state, match_algo, tokens, Indi());

        match_algo.context(scx);


        eoEvalFuncCounter<Indi> evalCounter(eval);


        eoInit<Indi>* indiInit = nullptr;
        eoInit<Atom>* atomInit = nullptr;
        std::tie(indiInit, atomInit) = load::load_init<Indi>(cfg_, state, info);

        eoGenOp<Indi>& op = load::load_op<Indi>(cfg_, state, *atomInit, info);

        eoPop<Indi>& pop = do_make_pop(parser, state, *indiInit);

        eoContinue<Indi>& stop_crit = do_make_continue(parser, state, evalCounter);

        eoCheckPoint<Indi>& checkpoint = do_make_checkpoint(parser, state, evalCounter, stop_crit);

        eoAlgo<Indi>& ga = do_make_algo_scalar(parser, state, eval, checkpoint, op);


        make_help(parser);

        apply<Indi>(eval, pop);

        do_run(ga, pop);

        auto best = pop.best_element();
        auto worse = pop.worse_element();
	std::cout << "BEST -------------------------" << std::endl;
        std::cout << best << std::endl;
        std::cout << utils::generate_sig(best, tokens, scx) << std::endl;
        std::cout << match_algo.match(best.begin(), best.end()) << std::endl << std::endl;

	std::cout << "WORST ------------------------" << std::endl;
        std::cout << worse << std::endl;
        std::cout << utils::generate_sig(worse, tokens, scx) << std::endl;
        std::cout << match_algo.match(worse.begin(), worse.end()) << std::endl;

        //eval(best)


    } catch (make::InputParamError ipe) {
        std::cerr << "Error reading command line: " << ipe.what() << std::endl;
        return -1;
    } catch (load::LoadingErrorException lee) {
        std::cerr << "Error loading data from config file: " << lee.what() << std::endl;
        return -1;
    }
    return 0;
}


//#include "Loader.ipp"

    } /* load */
} /* hisi */

#endif /* end of include guard: LOADER_HPP_KB32PURO */
