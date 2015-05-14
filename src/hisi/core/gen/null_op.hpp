
#ifndef NULL_OP_HPP_91VUMNFZ
#define NULL_OP_HPP_91VUMNFZ

#include <eoInit.h>

namespace hisi
{

template<typename T>
static eoInit<T>* null_init(){return nullptr;}

template<typename T>
static eoMonOp<T>* null_mut(){return nullptr;}
} /* hisi */

#endif /* end of include guard: NULL_OP_HPP_91VUMNFZ */
