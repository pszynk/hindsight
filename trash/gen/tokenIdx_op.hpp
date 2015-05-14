
#ifndef TOKENIDX_OP_HPP_B0P9GZAD
#define TOKENIDX_OP_HPP_B0P9GZAD

#include "../utils/gamath.hpp"
#include "../utils/constant.hpp"

#include <utils/eoUniformInit.h>

namespace hisi
{
namespace
{
    using tokenIdx_t = std::size_t;
} /* anonymous */

typedef eoUniformInit<tokenIdx_t> UniformTokenIdxInit;

template<class GenT>
class TokenIdxMutation : public eoMonOp<tokenIdx_t>
{
public:
    //[lb, ub)
    TokenIdxMutation(tokenIdx_t lb, tokenIdx_t ub, GenT& gen):
        lb_(lb), range_(ub - lb), gen_(gen)
    {
        if (lb > ub) {throw std::invalid_argument(
            "TokenIdxMutation lower bound "
            "must be smaller then upper bound!");}
    }

    bool operator()(tokenIdx_t& eo) override {
        int by = 0;
        // force mutation!
        while(by == 0) { by = gen_();}
        // proposed value
        long val = long(eo) + by;

        eo = r_wrap(val, (long)lb_, (long)range_);

        return by;
    }

    std::string className() const {return "TokenIdxMutation";}

private:
    GenT& gen_;
    int lb_, range_;
};

typedef TokenIdxMutation<eoUniformGenerator<int>> UniformTIMut;
typedef TokenIdxMutation<utils::constant::Value<int>> ConstantTIMut;

} /* hisi */

#endif /* end of include guard: TOKENIDX_OP_HPP_B0P9GZAD */
