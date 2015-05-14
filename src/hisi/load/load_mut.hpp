
#ifndef LOAD_MUT_HPP_FHQWCBNB
#define LOAD_MUT_HPP_FHQWCBNB

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
#include <eoProportionalCombinedOp.h>

#include <libconfig.h++>

#include <type_traits>

namespace hisi
{
    namespace load
    {

        namespace
        {

// signed integrals
template<typename T,
    typename std::enable_if<std::is_signed<T>::value>::type* = nullptr>
eoMonOp<T>& load_integer_mut(
        const libconfig::Setting& stg, eoState& state,
        T lb, T ub)
{
    using namespace libconfig;
    eoMonOp<T>* ret = nullptr;
    try {
        switch(stg.getType()) {
            case Setting::TypeInt:
                ret = new IntegerMutConstantWrap<T>(lb, ub, {stg});
                break;
            case Setting::TypeArray :
                ret = new IntegerMutUnsafeUniformWrap<T>(lb, ub, {stg[0], stg[1]});
                break;
            default :
                throw LoadingErrorException("Error loading mutation: " + std::string(stg.getPath()));
        }
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading integer mutation: " + std::string(sx.getPath()));
    }
    state.storeFunctor(ret);
    return *ret;
}

// unsigned integrals
template<typename T,
    typename std::enable_if<!std::is_signed<T>::value>::type* = nullptr>
eoMonOp<T>& load_integer_mut(
        const libconfig::Setting& stg, eoState& state,
        T lb, T ub)
{

    using namespace libconfig;
    eoMonOp<T>* ret = nullptr;
    try {
        switch(stg.getType()) {
            case Setting::TypeInt:
                ret = new IntegerMutConstantWrap<T>(lb, ub, {stg});
                break;
            case Setting::TypeArray :
                ret = new IntegerMutUniformWrap<T>(lb, ub, {stg[0], stg[1]});
                break;
            default :
                throw LoadingErrorException("Error loading mutation: " + std::string(stg.getPath()));
        }
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading integer mutation: " + std::string(sx.getPath()));
    }
    state.storeFunctor(ret);
    return *ret;
}

// array
template<typename T>
eoMonOp<std::array<T, 2>>& load_arr_mut(
        const libconfig::Setting& stg, eoState& state,
        eoMonOp<T>& mut1, eoMonOp<T>& mut2)
{

}

// content
template<typename T, typename E>
eoMonOp<Content<T, E>>& load_content_mut(
        const libconfig::Setting& stg, eoState& state,
        eoMonOp<T>& mutT, eoMonOp<E>& mutE)
{
    using namespace libconfig;
    eoMonOp<T>* ret = nullptr;
    try {
        switch(stg.getType()) {
            case Setting::TypeFloat:
                ret = new ContentPropMut<Content<T, E>>(mutT, mutE, stg);
                break;
            case Setting::TypeString :
                new ContentAllMut<Content<T, E>>(mutT, mutE);
                break;
            default :
                throw LoadingErrorException("Error loading content mutation: " + std::string(stg.getPath()));
        }
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading content mutation: " + std::string(sx.getPath()));
    }
    state.storeFunctor(ret);
    return *ret;
}

// vector
template<typename T>
eoMonOp<T>& load_vector_mut(
        const libconfig::Setting& stg, eoState& state,
        eoInit<typename T::AtomType>& initAtom,
        eoMonOp<typename T::AtomType>& mutAtom,
        unsigned minSize, unsigned maxSize)
{
    using namespace libconfig;
    using std::string;

    eoMonOp<T>* ret = nullptr;
    string type = stg["type"];

    try {
        if (type == string("shift")) {
            ret = new VectorMutShiftGenes<T>();
        }
        else if (type == string("add")) {
            if (minSize == maxSize) {
                std::cerr << "vector `add` mutation ignored, chrom has fixed size" << std::endl;
            }
            ret = new VectorMutAddGene<T>(maxSize, initAtom);
        }
        else if (type == string("delete")) {
            if (minSize == maxSize) {
                std::cerr << "vector `delete` mutation ignored, chrom has fixed size" << std::endl;
            }
            //using Atom = typename T::AtomType;
            UniformGeneChooser<T>* chooser = new UniformGeneChooser<T>();
            state.storeFunctor(chooser);
            ret = new VectorMutDelGene<T>(minSize, *chooser);

        }
        else if (type == string("rate")) {
            ret = new VectorMutRateOnAllGenes<T>(mutAtom, stg["rate"]);

        }
        else if (type == string("number")) {
            ret = new VectorMutOnKGenes<T>(mutAtom, stg["number"]);
        }
        else {
            throw LoadingErrorException("Unknown vector mutation: " + string(stg.getPath()));
        }

        state.storeFunctor(ret);
        return *ret;
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading vector mutation: " + std::string(sx.getPath()));
    }
}


template<typename T>
eoMonOp<T>& load_gene_mut(
        const libconfig::Setting& stg, eoState& state,
        const DataInfo& info)
{
    return load_integer_mut<T>(stg["token"]["mut"], state, 0, info.tokenCount);
}

// make_int
template<>
eoMonOp<Content<pos::single_t>>& load_gene_mut<Content<pos::single_t>>(
        const libconfig::Setting& stg, eoState& state, const DataInfo& info)
{
    using ContentType = Content<pos::single_t>;
    using TokenIdxType = ContentType::TokenIdxT;

    eoMonOp<TokenIdxType>& tokenMut =
        load_integer_mut<TokenIdxType>(stg["token"]["mut"], state, 0, info.tokenCount);

    eoMonOp<pos::single_t>& attrMut =
        load_integer_mut<pos::single_t>(stg["attr"]["arr"][0]["mut"], state,
                info.attr1_range.first, info.attr1_range.second);

    using namespace libconfig;
    const Setting& contMut = stg["mut"];
    eoMonOp<ContentType>* contentMut = nullptr;
    if (contMut.isNumber()) {
        contentMut = new ContentPropMut<ContentType>(&tokenMut, &attrMut, double(contMut));
    } else {
        contentMut = new ContentAllMut<ContentType>(&tokenMut, &attrMut);
    }
    state.storeFunctor(contentMut);

    return *contentMut;

}

// make_array
template<>
eoMonOp<Content<pos::dual_t>>& load_gene_mut<Content<pos::dual_t>>(
        const libconfig::Setting& stg, eoState& state, const DataInfo& info)
{
    using ContentType = Content<pos::dual_t>;
    using TokenIdxType = ContentType::TokenIdxT;

    eoMonOp<TokenIdxType>& tokenMut =
        load_integer_mut<TokenIdxType>(stg["token"]["mut"], state, 0, info.tokenCount);

    eoMonOp<pos::single_t>& attr1Mut =
        load_integer_mut<pos::single_t>(stg["attr"]["arr"][0]["mut"], state,
                info.attr1_range.first, info.attr1_range.second);

    eoMonOp<pos::single_t>& attr2Mut =
        load_integer_mut<pos::single_t>(stg["attr"]["arr"][1]["mut"], state,
                info.attr2_range.first, info.attr2_range.second);

    using namespace libconfig;

    const Setting& arrMut = stg["attr"]["mut"];
    eoMonOp<pos::dual_t>* arrayMut = nullptr;
    if (arrMut.isNumber()) {
        double rate = arrMut;
        arrayMut = new ArrayMutRoulette<pos::dual_t>(
                {{{&attr1Mut, rate}, {&attr2Mut, 1-rate}}});
    } else {
        arrayMut = new ArrayMutAll<pos::dual_t>({{{&attr1Mut, &attr2Mut}}});
    }
    state.storeFunctor(arrayMut);

    const Setting& contMut = stg["mut"];
    eoMonOp<ContentType>* contentMut = nullptr;
    if (contMut.isNumber()) {
        contentMut = new ContentPropMut<ContentType>(&tokenMut, arrayMut, double(contMut));
    } else {
        contentMut = new ContentAllMut<ContentType>(&tokenMut, arrayMut);
    }
    state.storeFunctor(contentMut);

    return *contentMut;
}

        } // anonymus

template<typename Indi>
eoMonOp<Indi>& load_mut(
        const libconfig::Config& cfg, eoState& state, eoInit<typename Indi::AtomType>& init, const DataInfo& info)
{
    using namespace libconfig;
    try {
        using AtomType = typename Indi::AtomType;
        eoMonOp<AtomType>& atomMut = load_gene_mut<AtomType>(
                cfg.lookup(config::path::CONTENT_A), state, info);

        const Setting& mut = cfg.lookup(config::path::CHROM_A)["mut"];

        eoMonOp<Indi>* ret = nullptr;
        if (mut.getLength() > 1) {
            eoPropCombinedMonOp<Indi>* comb = new eoPropCombinedMonOp<Indi>(
                    load_vector_mut<Indi>(
                        mut[0][0], state, init, atomMut,
                        info.minSigLen, info.maxSigLen), mut[0][1]);
            for(int i = 1; i < mut.getLength(); ++i) {
                comb->add(load_vector_mut<Indi>(
                        mut[i][0], state, init, atomMut,
                        info.minSigLen, info.maxSigLen), mut[i][1]);
            }
            ret = comb;
            state.storeFunctor(ret);
        } else {
            ret = &load_vector_mut<Indi>(
                        mut[0][0], state, init, atomMut,
                        info.minSigLen, info.maxSigLen);
        }

        return *ret;


    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading mutation: " + std::string(sx.getPath()));
    }
}

    } /* load */
} /* hisi */

#endif /* end of include guard: LOAD_MUT_HPP_FHQWCBNB */
