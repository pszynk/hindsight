
#ifndef SIGNATUREEVALUATOR_HPP_4WABX8NG
#define SIGNATUREEVALUATOR_HPP_4WABX8NG

#include <eoFunctor.h>

namespace hisi
{

template<class EOT>
struct SignatureEvaluator : public eoUF<const EOT&, typename EOT::Fitness>
{
    using Fitness = typename EOT::Fitness;
};

//template<class EOT, typename Fit = typename EOT::Fitness>
//struct SignatureEvaluator : public eoUF<const EOT&, Fit>
//{
//    using Fitness = Fit;
//};

} /* hisi */

#endif /* end of include guard: SIGNATUREEVALUATOR_HPP_4WABX8NG */
