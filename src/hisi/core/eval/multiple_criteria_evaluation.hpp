
#ifndef MULTIPLE_CRITERIA_EVALUATION_HPP_6ZSRGGJJ
#define MULTIPLE_CRITERIA_EVALUATION_HPP_6ZSRGGJJ

#include "SignatureEvaluator.hpp"

#include <vector>

namespace hisi
{

template<typename EOT>
struct MulitCriteriaEvaluator : public SignatureEvaluator<EOT>
{
    using Fitness = typename EOT::Fitness;

    using Item = SignatureEvaluator<EOT>;
    using Data = std::vector<std::pair<Item*, double>>;

    virtual void init(Data crits) = 0;
    virtual void add(Item& crit, double ratio) = 0;
};


template<typename EOT>
class LinearEvaluator : public MulitCriteriaEvaluator<EOT>
{
public:
    using Fitness = typename EOT::Fitness;

    using Item = typename MulitCriteriaEvaluator<EOT>::Item;
    using Data = typename MulitCriteriaEvaluator<EOT>::Data;

    LinearEvaluator() {}

    LinearEvaluator(
            Item& crit,
            double ratio):
            crits_({{&crit, ratio}})
    {}

    Fitness operator()(const EOT& eo) override
    {
        double val = 0.0;
        for(auto& c : crits_) {
            val += c.second * c.first->operator()(eo);
        }

        return val;
    }

    void add(Item& crit, double ratio) override {
        crits_.push_back({&crit, ratio});
    }

    void init(Data crits) override
    {
        crits_ = crits;
    }


private:
    Data crits_;
};


} /* hisi */

#endif /* end of include guard: MULTIPLE_CRITERIA_EVALUATION_HPP_6ZSRGGJJ */
