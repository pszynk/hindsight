
#ifndef VECTOR_OP_HPP_3OPTYRT1
#define VECTOR_OP_HPP_3OPTYRT1

#include "vector_cross_op.hpp"

#include <eoVector.h>
#include <eoInit.h>
#include <eoVariableLengthCrossover.h>
#include <eoVariableLengthMutation.h>
#include <eoShiftMutation.h>
#include <eoFlOrMonOp.h>
#include <ga/eoBitOp.h>

namespace hisi
{

//
// INIT
//

// fixed length
template <class EOT>
    class VectorInitFixedLength : public eoInit<EOT>
{
public:
    typedef typename EOT::AtomType AtomType;

    VectorInitFixedLength (unsigned size, eoInit<AtomType>& init)
        : size_(size), init_(init) {}

    void operator()(EOT& chrom) override
    {
        chrom.resize(size_);
        for(auto& a : chrom) {
            init_(a);
        }
        chrom.invalidate();
    }

private :
    unsigned size_;
    eoInit<AtomType>& init_;
};


// variable length
template<class VecT>
using VectorInitVariableLength = eoInitVariableLength<VecT>;

//
// MUT
//

// changing genes
template<class VecT>
using VectorMutRateOnAllGenes = eoFlOrAllMutation<VecT>;

template<class VecT>
using VectorMutOnKGenes = eoFlOrKMutation<VecT>;

// changing vector itself
template<class VecT>
using VectorMutAddGene = eoVlAddMutation<VecT>;

// choosing genes
template<class GeneT>
using GeneChooser = eoGeneDelChooser<GeneT>;

template<class GeneT>
using UniformGeneChooser = eoUniformGeneChooser<GeneT>;

template<class VecT>
using VectorMutDelGene = eoVlDelMutation<VecT>;

template<class VecT>
using VectorMutShiftGenes = eoShiftMutation<VecT>;


//
// CROSSOVER
//

template<class VecT>
using VectorCross1Point = eo1PtBitXover<VecT>;

template<class VecT>
using VectorCrossNPoint = eoNPtsBitXover<VecT>;

// exchange
// (gene idx, gene ref) -> bool (should be exchanged)
template<typename GenT>
using GeneExchange = eoAtomExchange<GenT>;

// (rate) -> every gene has a `rate` chance to get be set true (get exchanged)
template<typename GenT>
using UniformGeneExchange = eoUniformAtomExchange<GenT>;

// cross

template<typename T, typename I>
auto operator<<(std::ostream& os, eoVector<T, I> const& t)
    -> std::ostream&
{
    t.printOn(os);
    return os;
}


template<typename T, typename I>
auto operator>>(std::istream& is, eoVector<T, I>& t)
    -> std::istream&
{
    t.readFrom(is);
        return is;
}

} /* hisi */

#endif /* end of include guard: VECTOR_OP_HPP_3OPTYRT1 */
