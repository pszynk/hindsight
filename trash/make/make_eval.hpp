
#ifndef MAKE_EVAL_HPP_1LTXA0ZA
#define MAKE_EVAL_HPP_1LTXA0ZA

#include "param_config.hpp"
#include "InputParamError.hpp"

#include "hisi/core/eval/MatchConverter.hpp"
#include "hisi/core/eval/RewardAndPenaltyConverter.hpp"

#include "hisi/core/eval/MatchEvaluator.hpp"
#include "hisi/core/eval/LengthEvaluator.hpp"
#include "hisi/core/eval/multiple_criteria_evaluation.hpp"

#include "hisi/core/eval/EvaluatorWrapper.hpp"

#include <utils/eoParser.h>
#include <utils/eoState.h>
#include <eoEvalFunc.h>

#include <vector>
#include <algorithm>


namespace hisi
{
    namespace help_
    {

MatchConverter<double>* readConversion(const eoParamParamType& param);
MatchConverter<double>* readPenalty(const eoParamParamType& param);


template<typename EOT>
MulitCriteriaEvaluator<EOT>* readMulitCriteriaEvaluator(const eoParamParamType& param)
{
    if(param.first.empty()) {
        return nullptr;
    }

    if(param.first == std::string("Linear")) {
        if(param.second.size() == 0) {
            return new LinearEvaluator<EOT>();
        }
    }

    throw make::InputParamError("unknown criteria scalarization");
}

    } /* help_ */

    namespace make
    {

template<class EOT, class Mapper, class MatchAlgo>
eoEvalFunc<EOT>& make_eval(
        eoParser& parser, eoState& state, MatchAlgo& match_algo, Mapper& mapper, EOT)
{
    using namespace make_param_;
    using namespace help_;

    using Gene = typename EOT::AtomType;


    // ------ conversion -----------
    // conversion
    MatchConverter<double>* conv_ptr;
    eoParamParamType reward =
        MATCH_CONVERSION.doGetORcreateParam(parser).value();
    auto reward_ptr = readConversion(reward);

    // penalty
    eoParamParamType penalty =
        MATCH_PENALTY.doGetORcreateParam(parser).value();
    auto penalty_ptr = readPenalty(penalty);

    if(penalty_ptr) {
        state.storeFunctor(penalty_ptr);
        conv_ptr = new RewardAndPenaltyConverter<double>(*reward_ptr, *penalty_ptr);
    } else {
        conv_ptr = reward_ptr;
    }
    state.storeFunctor(conv_ptr);

    // threshold
    double threshold =
        MATCH_PENALTY_THRESHOLD.doGetORcreateParam(parser).value();

    if (!penalty_ptr && threshold != 0.0) {
        std::cerr << "threshold value ignored!, no penalty function!" << std::endl;
    }
    else if (penalty_ptr && (threshold <= 0 || threshold >= 1)) {
        throw InputParamError("threshold must be from range (0, 1), when penalty"
                " function is set");
    }

    match_algo.threshold(threshold);

    // ------- evaluation -----------

    using MultiData = typename MulitCriteriaEvaluator<EOT>::Data;
    MultiData evaluators;

    MatchEvaluator<EOT, MatchAlgo>* match_eval_ptr =
        new MatchEvaluator<EOT, MatchAlgo>(match_algo, *conv_ptr);
    //state.storeFunctor(match_eval_ptr);


    double match_crit_ratio =
        MATCH_CRITERION_RATIO.doGetORcreateParam(parser).value();
    if (match_crit_ratio < 0 || match_crit_ratio > 1) {
        throw make::InputParamError("criterion ratio must be from range [0, 1]");
    }
    evaluators.push_back({match_eval_ptr, match_crit_ratio});

    double len_crit_ratio =
        LENGTH_CRITERION_RATIO.doGetORcreateParam(parser).value();
    if (len_crit_ratio < 0 || len_crit_ratio > 1) {
        throw make::InputParamError("criterion ratio must be from range [0, 1]");
    }
    evaluators.push_back({new LengthEvaluator<EOT, Mapper>(mapper), len_crit_ratio});

    eoParamParamType multi =
        SCALARIZATION_FUNCTION.doGetORcreateParam(parser).value();
    MulitCriteriaEvaluator<EOT>* multi_ptr = readMulitCriteriaEvaluator<EOT>(multi);

    // remove zero ratios
    evaluators.erase(
            std::remove_if(evaluators.begin(), evaluators.end(),
            [&state](const typename MultiData::value_type& p) {
            if(p.second == 0.0) {
                delete p.first;
                return false;
            }
            state.storeFunctor(p.first);
            return true;}),
            evaluators.end()
            );

    if(evaluators.size() == 0) {
        throw make::InputParamError("all criterion have 0.0 ratio!");
    }

    if(evaluators.size() == 1) {
        EvaluatorWrapper<EOT>* wrapper = new EvaluatorWrapper<EOT>(*evaluators[0].first);
        //state.storeFunctor(evaluators[0]);
        state.storeFunctor(wrapper);
        return *wrapper;
    }

    multi_ptr->init(evaluators);
    EvaluatorWrapper<EOT>* wrapper = new EvaluatorWrapper<EOT>(*multi_ptr);
    state.storeFunctor(wrapper);

    return *wrapper;
}

    } /* make
         */
} /* hisi */

#endif /* end of include guard: MAKE_EVAL_HPP_1LTXA0ZA */
