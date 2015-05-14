
#ifndef IDX_OP_HPP_5KHCFWQE
#define IDX_OP_HPP_5KHCFWQE

#include "IdxBounds.hpp"

#include <utils/eoRndGenerators.h>
#include <utils/eoUniformInit.h>

#include <cstddef>


namespace
{
    using idx_t = std::size_t;
}

namespace hisi
{
    namespace ev
    {

// generators
typedef eoUniformGenerator<idx_t> IdxUniformGenerator;
typedef eoNormalGenerator<idx_t> IdxNormalGenerator;
typedef eoNegExpGenerator<idx_t> IdxNegExpGenerator;

// init
// CAUTION!!! about ranges [min max) prob!!
typedef eoUniformInit<idx_t> IdxUniformInit;
typedef eoNormalInit<idx_t> IdxNormalInit;
typedef eoNegExpInit<idx_t> IdxNegExpInit;

// mutations:
template<class GenT>
class IdxRandomShifter : public eoMonOp<idx_t>
{
public:
    IdxRandomShifter(GenT& gen):
        gen_(gen)
    {}

    bool operator()(idx_t& eo) override {
        auto by = gen_();
        eo += by;
        return by;
    }

private:
    GenT& gen_;
};


template<class BoundsT, class GenT>
class IdxBoundedRandomShifter : public eoMonOp<idx_t>
{
public:
    IdxBoundedRandomShifter(BoundsT bounds, GenT& gen):
        bounds_(bounds), gen_(gen)
    {}

    bool operator()(idx_t& eo) override {
        auto by = gen_();
        eo = bounds_.add(eo, by);
        return by;
    }

private:
    BoundsT bounds_;
    GenT& gen_;
};

    } /* ev */
} /* hisi */

#endif /* end of include guard: IDX_OP_HPP_5KHCFWQE */
