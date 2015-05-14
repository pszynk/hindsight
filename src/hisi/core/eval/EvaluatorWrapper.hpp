
#ifndef EVALUATORWRAPPER_HPP_NT12NLEE
#define EVALUATORWRAPPER_HPP_NT12NLEE

#include "SignatureEvaluator.hpp"

#include <eoEvalFunc.h>

namespace hisi
{


template<typename EOT>
class EvaluatorWrapper : public eoEvalFunc<EOT>
{
public:
    EvaluatorWrapper(SignatureEvaluator<EOT>& evaluator):
        evaluator_(evaluator)
    {}

    void operator()(EOT& eo) override
    {
        if (eo.invalid()) {
            eo.fitness(evaluator_(eo));
        }
    }

private:
    SignatureEvaluator<EOT>& evaluator_;
};


} /* hisi */

#endif /* end of include guard: EVALUATORWRAPPER_HPP_NT12NLEE */
