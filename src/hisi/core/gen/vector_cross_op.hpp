
#ifndef VECTOR_CROSS_OP_HPP_LQNABD9K
#define VECTOR_CROSS_OP_HPP_LQNABD9K

#include <eoVariableLengthCrossover.h>
#include <eoVariableLengthMutation.h>
#include <utils/eoLogger.h>

namespace hisi
{


// variable length exchange
// min -> min to exchange, max -> max to exchange, geneExchange -> way to choose
//  which one to exchange
template<typename VecT>
class VectorCrossGeneExchange : public eoQuadOp<VecT>
{
public:
    using AtomType = typename VecT::AtomType;

    VectorCrossGeneExchange(unsigned minSize, unsigned maxSize,
            eoAtomExchange<AtomType>& exchange,
            unsigned tries = 10000):
        minSize_(minSize), maxSize_(maxSize), exchange_(exchange), tries_(tries)
    {}

    bool operator()(VecT & left, VecT & right) override
    {
        bool success = false;
        for(unsigned tr = 0; tr < tries_; ++tr) {
            // initialize AtomExchnage
            exchange_.randomize(left.size(), right.size());

            unsigned lsize = 0, rsize = 0;
            for(unsigned i = 0; i < left.size(); ++i) {
                // exchange?
                if(exchange_(i, left[i])) {
                    ++rsize;
                } else {
                    ++lsize;
                }
            }
            for(unsigned i = 0; i < right.size(); ++i) {
                // exchange?
                if(exchange_(i + left.size(), right[i])) {
                    ++lsize;
                } else {
                    ++rsize;
                }
            }
            if((lsize >= minSize_) && (rsize >= minSize_) &&
                    (lsize <= maxSize_) && (rsize <= maxSize_)) {
                success = true;
                break;
            }
        }

        if(!success) {
            eo::log << eo::warnings <<
                "Warning - VectorGeneExchange: imimpossible to generate" <<
                " individual of the right size in " << tries_ << " trials" <<
                std::endl;
            return false;
        }

        VecT tempLeft;
        VecT tempRight;

        bool flag = false;
        for(unsigned i = 0; i < left.size(); ++i) {
            if(exchange_(i, left[i])) {
                tempRight.push_back(left[i]);
                flag = true;
            } else {
                tempLeft.push_back(left[i]);
            }
        }

        for(unsigned i = 0; i < right.size(); ++i) {
            if(exchange_(i + left.size(), right[i])) {
                tempLeft.push_back(right[i]);
                flag = true;
            } else {
                tempRight.push_back(right[i]);
            }
        }

        return flag;
    }

private:
    unsigned minSize_;
    unsigned maxSize_;
    eoAtomExchange<AtomType>& exchange_;
    unsigned tries_;
};

} /* hisi */


#endif /* end of include guard: VECTOR_CROSS_OP_HPP_LQNABD9K */
