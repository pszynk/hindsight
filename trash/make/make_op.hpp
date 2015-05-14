
#ifndef MAKE_OP_HPP_DZQFXOWH
#define MAKE_OP_HPP_DZQFXOWH

#include "DataInfo.hpp"
#include "param_config.hpp"
#include "InputParamError.hpp"

#include "hisi/core/gen/integer_op.hpp"
#include "hisi/core/gen/array_op.hpp"
#include "hisi/core/gen/tuple_op.hpp"
#include "hisi/core/gen/vector_op.hpp"
#include "hisi/core/gen/vector_cross_op.hpp"
#include "hisi/core/gen/content_op.hpp"

#include "hisi/core/pos/position_rules.hpp"

#include <utils/eoParser.h>
#include <utils/eoState.h>

namespace hisi
{
    namespace make
    {
        namespace help_
        {

template<typename TokenIndexType>
eoMonOp<TokenIndexType>& make_tokenMut(
        eoParser& parser, eoState& state, const DataInfo& info)
{

}

template<typename AttrType>
eoMonOp<AttrType>& make_attrMut(
        eoParser& parser, eoState& state, const DataInfo& info)
{

}

template<typename ContentType>
eoMonOp<ContentType>& make_contentMut(
        eoParser& parser, eoState& state,
        typename ContentType::TokenIdxT& tokenMut,
        typename ContentType::AttT& attrMut)
{

}

template<typename Indi>
eoMonOp<Indi>& make_vectorMut(
        eoParser& parser, eoState& state, typename Indi::AtomType& contentMut)
{

}

        } /* help_ */

template<class Indi>
eoMonOp<Indi>& make_op(eoParser &parser, eoState &state, const DataInfo& info)
{
    using ContentType = typename Indi::AtomType;
    using TokenIndexType = typename ContentType::TokenIdxT;
    using AttrType = typename ContentType::AttrT;

    using namespace help_;

    eoMonOp<TokenIndexType>& tokenMut =
        make_tokenMut<TokenIndexType>(parser, state, info);

    eoMonOp<AttrType>& attrMut =
        make_attrMut<AttrType>(parser, state, info);

    eoMonOp<ContentType>& contentMut =
        make_contentMut<ContentType>(parser, state, &tokenMut, &attrMut);

    eoMonOp<Indi>& = vectorMut =
        make_vectorMut<ContentType>(parser, state, &contentMut);

    // crossover




}

    } /* make */
} /* hisi */

#endif /* end of include guard: MAKE_OP_HPP_DZQFXOWH */
