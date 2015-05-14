
#ifndef LOAD_CROSS_HPP_JIJHQBFT
#define LOAD_CROSS_HPP_JIJHQBFT

#include "hisi/core/gen/vector_op.hpp"

#include "hisi/core/pos/position_rules.hpp"

#include "LoadingErrorException.hpp"
#include "DataInfo.hpp"
#include "hisi/config.hpp"

#include <utils/eoState.h>
#include <eoProportionalCombinedOp.h>

#include <libconfig.h++>

namespace hisi
{
    namespace load
    {

template<typename T>
eoQuadOp<T>& load_vector_cross(
        const libconfig::Setting& stg, eoState& state,
        unsigned minSize, unsigned maxSize)
{
    using namespace libconfig;
    using std::string;

    using Atom = typename T::AtomType;

    eoQuadOp<T>* ret = nullptr;
    string type = stg["type"];

    try {
        if (type == string("uni_exch")) {
            eoUniformAtomExchange<Atom>* uax = new eoUniformAtomExchange<Atom>(stg["rate"]);
            state.storeFunctor(uax);
            ret = new VectorCrossGeneExchange<T>(minSize, maxSize, *uax);
        }
        else if (type == string("1pt_cross")) {
            ret = new VectorCross1Point<T>();
        }
        else if (type == string("Npt_cross")) {
            ret = new VectorCrossNPoint<T>(stg["number"]);
        }
        else {
            throw LoadingErrorException("Unknown vector crossover: " + string(stg.getPath()));
        }

        state.storeFunctor(ret);
        return *ret;
    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading vector crossover: " + std::string(sx.getPath()));
    }
}


template<typename Indi>
eoQuadOp<Indi>& load_cross(
        const libconfig::Config& cfg, eoState& state, const DataInfo& info)
{
    using namespace libconfig;
    try {
        //using AtomType = typename Indi::AtomType;

        const Setting& cross = cfg.lookup(config::path::CHROM_A)["cross"];

        eoQuadOp<Indi>* ret = nullptr;
        if (cross.getLength() > 1) {
            eoPropCombinedQuadOp<Indi>* comb = new eoPropCombinedQuadOp<Indi>(
                    load_vector_cross<Indi>(
                        cross[0][0], state,
                        info.minSigLen, info.maxSigLen), cross[0][1]);
            for(int i = 1; i < cross.getLength(); ++i) {
                comb->add(load_vector_cross<Indi>(
                        cross[i][0], state,
                        info.minSigLen, info.maxSigLen), cross[i][1]);
            }
            ret = comb;
            state.storeFunctor(ret);
        } else {
            ret = &load_vector_cross<Indi>(
                        cross[0][0], state,
                        info.minSigLen, info.maxSigLen);
        }

        return *ret;


    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading crossover: " + std::string(sx.getPath()));
    }
}

    } /* load */
} /* hisi */

#endif /* end of include guard: LOAD_CROSS_HPP_JIJHQBFT */
