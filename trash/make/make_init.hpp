
#ifndef MAKE_INIT_HPP_CWSROZ2M
#define MAKE_INIT_HPP_CWSROZ2M

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

#include <eoInit.h>
#include <utils/eoParser.h>
#include <utils/eoState.h>

namespace hisi
{
    namespace make
    {
        namespace help_
        {

// make_tokenidx
template<typename T>
eoInit<T>& make_tokenInit(eoParser& /*parser*/, eoState &state, const DataInfo& info)
{
    // no param
    eoInit<T>* init = new IntegerInitUniform<T>(0, info.tokenCount);
    state.storeFunctor(init);
    return *init;
}

template<typename AttrT>
eoInit<AttrT>& make_attr_init(eoParser& /*parser*/, eoState &state, const DataInfo& info);

// make_int
template<>
eoInit<pos::single_t>& make_attr_init<pos::single_t>(eoParser& parser, eoState &state, const DataInfo& info);

// make_array
template<>
eoInit<pos::dual_t>& make_attr_init<pos::dual_t>(eoParser& parser, eoState &state, const DataInfo& info);



        } /* help_ */

template<class Indi>
eoInit<Indi>& make_init(eoParser &parser, eoState &state, const DataInfo& info)
{
    using ContentType = typename Indi::AtomType;
    using TokenIndexType = typename ContentType::TokenIdxT;
    using AttrType = typename ContentType::AttrT;

    using namespace help_;

    eoInit<TokenIndexType>& tokenInit =
        make_tokenInit<TokenIndexType>(parser, state, info);

    eoInit<AttrType>& attrInit = make_attr_init<AttrType>(parser, state, info);

    eoInit<ContentType>* contInit = new ContentInit<ContentType>(&tokenInit, &attrInit);
    state.storeFunctor(contInit);


    using namespace make_param_;
    // vector init
    int sig_lb =
        INIT_SIGNATURE_LENGTH_LOWER.doGetORcreateParam(parser).value();

    int sig_ub =
        INIT_SIGNATURE_LENGTH_UPPER.doGetORcreateParam(parser).value();

    if(sig_ub <= 0) {
        // fixed
        if (sig_lb < info.minSigLen || sig_lb > info.maxSigLen) {
            std::stringstream ss;
            ss << "Init: signature length should be from range: "
                << "["<< info.minSigLen << ", "<< info.maxSigLen << "]";
            throw InputParamError(ss.str());
        }
        eoInit<Indi>* vec_init = new VectorInitFixedLength<Indi>(sig_lb, *contInit);
        state.storeFunctor(vec_init);
        return *vec_init;
    }

    if (sig_lb < info.minSigLen || sig_lb >= sig_ub || sig_ub > info.maxSigLen) {
        std::stringstream ss;
        ss << "Init: signature length should be from range: "
            << "["<< info.minSigLen << ", "<< info.maxSigLen << "] and "
            << "lb < ub, now lb=" << sig_lb <<" ub=" << sig_ub;
        throw InputParamError(ss.str());
    }

    // variable length
    eoInit<Indi>* vec_init = new VectorInitVariableLength<Indi>(sig_lb, sig_ub, *contInit);
    state.storeFunctor(vec_init);
    return *vec_init;
}


    } /* make */
} /* hisi */

#endif /* end of include guard: MAKE_INIT_HPP_CWSROZ2M */
