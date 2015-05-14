
#ifndef LOAD_OP_HPP_GIJF74H2
#define LOAD_OP_HPP_GIJF74H2

#include "load_cross.hpp"
#include "load_mut.hpp"

#include <eoSGAGenOp.h>

namespace hisi
{
    namespace load
    {

template<typename Indi>
eoGenOp<Indi>& load_op(
        const libconfig::Config& cfg, eoState& state, eoInit<typename Indi::AtomType>& init, const DataInfo& info)
{

    using namespace libconfig;
    try {

        const Setting& chrom = cfg.lookup(config::path::CHROM_A);
        double mut_rate = chrom["mut_rate"];
        double cross_rate = chrom["cross_rate"];

        eoMonOp<Indi>& mut = load::load_mut<Indi>(cfg, state, init, info);
        eoQuadOp<Indi>& cross = load::load_cross<Indi>(cfg, state, info);

        eoSGAGenOp<Indi>* gop = new eoSGAGenOp<Indi>(cross, cross_rate, mut, mut_rate);
        state.storeFunctor(gop);
        return *gop;

    } catch (SettingException sx) {
        throw LoadingErrorException("Error loading operations: " + std::string(sx.getPath()));
    }
}

    } /* load */
} /* hisi */

#endif /* end of include guard: LOAD_OP_HPP_GIJF74H2 */
