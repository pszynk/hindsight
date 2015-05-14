
#ifndef LOAD_EVAL_HPP_R93RSCLE
#define LOAD_EVAL_HPP_R93RSCLE

#include "hisi/core/eval/SSPNMatchConverter.hpp"
#include "hisi/core/eval/MatchConverter.hpp"
#include "hisi/core/eval/RewardAndPenaltyConverter.hpp"

#include "hisi/core/eval/MatchEvaluator.hpp"
#include "hisi/core/eval/LengthEvaluator.hpp"
#include "hisi/core/eval/multiple_criteria_evaluation.hpp"

#include "hisi/core/eval/EvaluatorWrapper.hpp"

#include "LoadingErrorException.hpp"
#include "DataInfo.hpp"
#include "hisi/config.hpp"

#include <utils/eoState.h>
#include <eoEvalFuncCounter.h>

#include <libconfig.h++>

namespace hisi
{
    namespace load
    {
        namespace
        {


MatchConverter<double>* load_conversion(const libconfig::Setting& stg)
{
    using namespace libconfig;
    using std::string;
    try {
        const Setting& conv = stg["convert"];
        string type = conv["type"];

        if (type == string("SSPN")) {
            const Setting& args = conv["args"];
            switch (args.getLength()) {
                case 0:  return new SSPNMatchConverter();
                case 1:  return new SSPNMatchConverter(bool(args[0]));
                case 2:  return new SSPNMatchConverter(bool(args[0]), bool(args[1]));
                case 3:  return new SSPNMatchConverter(bool(args[0]), bool(args[1]), bool(args[2]));
                default: return new SSPNMatchConverter(bool(args[0]), bool(args[1]), bool(args[2]), bool(args[3]));
            }
        } else {
            throw LoadingErrorException("Error loading conversion function: "
                    + std::string(conv.getPath()) + ", unknown type: " + type);
        }
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading conversion function: " +
                std::string(sx.getPath()));
    }
}

MatchConverter<double>* load_penalty(const libconfig::Setting& stg)
{
    using namespace libconfig;
    using std::string;
    try {

        Setting* pen = nullptr;
        try {
            pen = &stg["penalty"];
        } catch (SettingNotFoundException nfx) {
            std::cerr << "no penalty function in config, none will be used" << std::endl;
            return nullptr;
        }
        string type = (*pen)["type"];

        if (type == string("none")) {
            return nullptr;
        } else {
            throw LoadingErrorException("Error loading penalty function: "
                    + std::string(pen->getPath()) + ", unknown type: " + type);
        }
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading penalty function: " +
                std::string(sx.getPath()));
    }
}

template<typename Indi, typename Mapper>
SignatureEvaluator<Indi>* load_second_crit(const libconfig::Setting& stg,
        const Mapper& tokens)
{
    using namespace libconfig;
    using std::string;
    try {

        Setting* crit = nullptr;
        try {
            crit = &stg["second_crit"];
        } catch (SettingNotFoundException nfx) {
            std::cerr << "no second crit function in config, none will be used" << std::endl;
            return nullptr;
        }

        string type = (*crit)["type"];

        if (type == string("none")) {
            return nullptr;
        } else if (type == string("length")) {
            unsigned max_ = (*crit)["max"];
            return new LengthEvaluator<Indi, Mapper>(tokens, max_);
        } else {
            throw LoadingErrorException("Error loading second crit function: "
                    + std::string(crit->getPath()) + ", unknown type: " + type);
        }
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading second crit function: " +
                std::string(sx.getPath()));
    }
}

template<typename Indi>
MulitCriteriaEvaluator<Indi>* load_scalar(const libconfig::Setting& stg,
    SignatureEvaluator<Indi>* eval1, SignatureEvaluator<Indi>* eval2)
{
    using namespace libconfig;
    using std::string;
    try {

        Setting* scal = nullptr;

        try {
            scal = &stg["scalar_func"];
        } catch (SettingNotFoundException nfx) {
            std::cerr << "no second crit function in config, none will be used" << std::endl;
            return nullptr;
        }

        string type = (*scal)["type"];

        if (type == string("none")) {
            return nullptr;
        } else if (type == string("linear")) {
            if (eval1 == nullptr || eval2 == nullptr) {
                std::cerr << "Scalarization set, but only one criteria, scalar. ignored!" << std::endl;
                return nullptr;
            }
            double rate = (*scal)["rate"];
            LinearEvaluator<Indi>* lin =  new LinearEvaluator<Indi>();
            lin->add(*eval1, rate);
            lin->add(*eval2, 1.0-rate);
            return lin;
        } else {
            throw LoadingErrorException("Error loading scalar function: "
                    + std::string((*scal).getPath()) + ", unknown type: " + type);
        }
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading scalar function: " +
                std::string(sx.getPath()));
    }
}
        } // anonymus

template<class Indi, class MatchAlgo, class Mapper>
eoEvalFunc<Indi>& load_eval(
         const libconfig::Config &cfg, eoState& state,
         MatchAlgo& match_algo, const Mapper& mapper, const Indi&)
{
    //using Atom = typename Indi::AtomType;
    //using Fitness = typename Indi::Fitness;
    using Convers = MatchConverter<double>;

    using namespace libconfig;

    const Setting& eval = cfg.lookup(config::path::EVAL_A);
    // 1. get conversion
    Convers* conversion = load_conversion(eval);
    state.storeFunctor(conversion);
    // 2. try get penalty, and threshold
    Convers* penalty = load_penalty(eval);
    if (penalty) {
        if (eval.exists("threshold")) {
            // 3. create full conversion
            state.storeFunctor(penalty);
            conversion = new RewardAndPenaltyConverter<double>(*conversion, *penalty);
            state.storeFunctor(conversion);
        } else {
            delete penalty;
            throw LoadingErrorException("Loading conversion - penalty function"
                    " given, but no threshold!");
        }
    }
    if (eval.exists("threshold")) {
        std::cerr << "Threshold set, but no penalty function!, threshold ignored" << std::endl;
    }
    SignatureEvaluator<Indi>* matchEval = new MatchEvaluator<Indi, MatchAlgo>(match_algo, *conversion);
    state.storeFunctor(matchEval);

    SignatureEvaluator<Indi>* secondEval = load_second_crit<Indi, Mapper>(eval, mapper);

    SignatureEvaluator<Indi>* scalar = load_scalar<Indi>(eval, matchEval, secondEval);

    EvaluatorWrapper<Indi>* wrap = nullptr;
    if (secondEval && !scalar) {
        delete secondEval;
        throw LoadingErrorException("Loading evaluation - second criterion"
                " given, but no scalarization function!");
    } else if (scalar) {
        state.storeFunctor(secondEval);
        state.storeFunctor(scalar);
        wrap = new EvaluatorWrapper<Indi>(*scalar);
    } else {
        wrap = new EvaluatorWrapper<Indi>(*matchEval);
    }
    state.storeFunctor(wrap);
    eoEvalFuncCounter<Indi>* func = new eoEvalFuncCounter<Indi>(*wrap);
    state.storeFunctor(func);
    return *func;
}

    } /* load */
} /* hisi */

#endif /* end of include guard: LOAD_EVAL_HPP_R93RSCLE */
