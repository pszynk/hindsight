
#ifndef LOAD_INIT_HPP_JPNNQX6D
#define LOAD_INIT_HPP_JPNNQX6D

#include "hisi/core/gen/integer_op.hpp"
#include "hisi/core/gen/array_op.hpp"
//#include "hisi/core/gen/tuple_op.hpp"
#include "hisi/core/gen/vector_op.hpp"
#include "hisi/core/gen/content_op.hpp"

#include "hisi/core/pos/position_rules.hpp"

#include "LoadingErrorException.hpp"
#include "DataInfo.hpp"
#include "hisi/config.hpp"

#include <utils/eoState.h>

#include <libconfig.h++>

#include <algorithm>

namespace hisi
{
    namespace load
    {
        namespace
        {


template<typename T,
    typename std::enable_if<std::is_signed<T>::value>::type* = nullptr>
eoInit<T>& create_integer_init(eoState& state, T lb, T ub)
{
    eoInit<T>* init = new IntegerInitUniform<T>(lb, ub);
    state.storeFunctor(init);
    return *init;
}

// unsigned integrals
template<typename T,
    typename std::enable_if<!std::is_signed<T>::value>::type* = nullptr>
eoInit<T>& create_integer_init(eoState& state, T lb, T ub)
{
    eoInit<T>* init = new IntegerInitUnsafeUniform<T>(lb, ub);
    state.storeFunctor(init);
    return *init;
}

template<typename T>
eoInit<T>& load_gene_init(
        const libconfig::Setting& /*stg*/, eoState& state,
        DataInfo& info)
{
    return create_integer_init<T>(state, 0, info.tokenCount);
}

template<>
eoInit<Content<pos::single_t>>& load_gene_init<Content<pos::single_t>>(
        const libconfig::Setting& stg, eoState& state, DataInfo& info)
{

    using ContentType = Content<pos::single_t>;
    using TokenIdxType = ContentType::TokenIdxT;

    eoInit<TokenIdxType>& tokenInit =
        create_integer_init<TokenIdxType>(state, 0, info.tokenCount);

    using namespace libconfig;
    const Setting& range = stg["attr"]["arr"][0]["range"];
    using std::max;
    using std::min;
    int lb = max(int(range[0]), -info.maxFlowLength);
    int ub = min(int(range[1]), info.maxFlowLength);
    info.attr1_range = std::make_pair(lb, ub);
    eoInit<pos::single_t>& attrInit =
        create_integer_init<pos::single_t>(state, lb, ub);

    eoInit<ContentType>* contentInit = new ContentInit<ContentType>(&tokenInit, &attrInit);
    state.storeFunctor(contentInit);

    return *contentInit;
}

// make_array
template<>
eoInit<Content<pos::dual_t>>& load_gene_init<Content<pos::dual_t>>(
        const libconfig::Setting& stg, eoState& state, DataInfo& info)
{
    using ContentType = Content<pos::dual_t>;
    using TokenIdxType = ContentType::TokenIdxT;

    eoInit<TokenIdxType>& tokenInit =
        create_integer_init<TokenIdxType>(state, 0, info.tokenCount);

    using namespace libconfig;
    const Setting& range1 = stg["attr"]["arr"][0]["range"];
    const Setting& range2 = stg["attr"]["arr"][1]["range"];
    using std::max;
    using std::min;

    int lb1 = max(int(range1[0]), -info.maxFlowLength);
    int ub1 = min(int(range1[1]), info.maxFlowLength);
    int lb2 = max(int(range2[0]), -info.maxFlowLength);
    int ub2 = min(int(range2[1]), info.maxFlowLength);

    info.attr1_range = std::make_pair(lb1, ub1);
    info.attr2_range = std::make_pair(lb2, ub2);

    eoInit<pos::single_t>& attr1Init =
        create_integer_init<pos::single_t>(state, lb1, ub1);
    eoInit<pos::single_t>& attr2Init =
        create_integer_init<pos::single_t>(state, lb2, ub2);

    eoInit<pos::dual_t>* arrInit = new ArrayInit<pos::dual_t>({{{&attr1Init, &attr2Init}}});
    state.storeFunctor(arrInit);

    eoInit<ContentType>* contentInit = new ContentInit<ContentType>(&tokenInit, arrInit);
    state.storeFunctor(contentInit);

    return *contentInit;
}

        } //anonymus

template<typename Indi>
std::tuple<eoInit<Indi>*, eoInit<typename Indi::AtomType>*>
load_init(
        const libconfig::Config& cfg, eoState& state, DataInfo& info)
{
    using namespace libconfig;
    try {
        using AtomType = typename Indi::AtomType;
        eoInit<AtomType>& atomInit = load_gene_init<AtomType>(
                cfg.lookup(config::path::CONTENT_A), state, info);

        const Setting& init = cfg.lookup(config::path::CHROM_A)["size"];

        eoInit<Indi>* ret = nullptr;
        if (init.isScalar()) {
            info.minSigLen = init;
            info.maxSigLen = init;
            info.constSize = true;
            ret = new VectorInitFixedLength<Indi>(init, atomInit);
        } else {
            info.minSigLen = init[0];
            info.maxSigLen = init[1];
            info.constSize = false;
            ret = new VectorInitVariableLength<Indi>(init[0], init[1], atomInit);
        }

        state.storeFunctor(ret);
        return std::make_tuple(ret, &atomInit);

    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading initation: " + std::string(sx.getPath()));
    }
}

    } /* load */
} /* hisi */

#endif /* end of include guard: LOAD_INIT_HPP_JPNNQX6D */
