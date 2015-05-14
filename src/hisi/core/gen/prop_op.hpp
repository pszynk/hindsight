
#ifndef PROP_OP_HPP_80UOSOT5
#define PROP_OP_HPP_80UOSOT5

#include <eoProportionalCombinedOp.h>

namespace hisi
{


template<typename EOT>
using PropCombinedMut = eoPropCombinedMonOp<EOT>;

template<typename EOT>
using PropCombinedCross = eoPropCombinedQuadOp<EOT>;
} /* hisi */

#endif /* end of include guard: PROP_OP_HPP_80UOSOT5 */
