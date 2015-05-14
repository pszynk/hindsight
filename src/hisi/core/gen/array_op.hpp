
#ifndef ARRAY_OP_HPP_SNKZ09PA
#define ARRAY_OP_HPP_SNKZ09PA


#include <eoInit.h>
#include <utils/eoRNG.h>

#include <array>

namespace hisi
{

namespace
{

//template<typename T, std::size_t I>
//using ArrT = std::array<T,I>;

}


//
// INIT
//
template<typename ArrT>
class ArrayInit : public eoInit<ArrT>
{
    using ValT = typename ArrT::value_type;
    using InitArrayT = typename std::array<eoInit<ValT>*, std::tuple_size<ArrT>::value>;
public:
    //ArrayInit(eoInit<Args>*... inits):inits_(std::make_array(inits...))
    //{}

    ArrayInit(InitArrayT inits):inits_(inits)
    {}

    void data(InitArrayT inits) {inits_(inits);}

    void operator()(ArrT& eo) override
    {
        for(std::size_t i = 0; i < eo.size(); ++i) {
            if(inits_[i])
                inits_[i]->operator()(eo[i]);
        }
    }

    std::string className() const {return "ArrayInit";}

private:
    InitArrayT inits_;
};

template<typename ArrT>
class ArrayMutAll : public eoMonOp<ArrT>
{
    using ValT = typename ArrT::value_type;
    using MutArrayT = typename std::array<eoMonOp<ValT>*, std::tuple_size<ArrT>::value>;
public:
    //ArrayMut(eoMut<Args>*... muts):muts_(std::make_array(muts...))
    //{}

    ArrayMutAll(MutArrayT muts):muts_(muts)
    {}

    void data(MutArrayT muts) {muts_(muts);}

    bool operator()(ArrT& eo) override
    {
        bool flag = false;
        for(std::size_t i = 0; i < eo.size(); ++i) {
            if(muts_[i])
                flag |= muts_[i]->operator()(eo[i]);
        }
        return flag;
    }

    std::string className() const {return "ArrayMutAll";}

private:
    MutArrayT muts_;
};

template<typename ArrT>
class ArrayMutRoulette : public eoMonOp<ArrT>
{
    using ValT = typename ArrT::value_type;
    using MutArrayT = typename std::array<std::pair<eoMonOp<ValT>*, double>, std::tuple_size<ArrT>::value>;
public:
    //ArrayMut(eoMut<Args>*... muts):muts_(std::make_array(muts...))
    //{}

    ArrayMutRoulette(MutArrayT muts):muts_(muts)
    {
        for(auto m : muts_) {
            total_ += m.second;
        }
    }

    void data(MutArrayT muts) {muts_(muts);}

    bool operator()(ArrT& eo) override
    {
        double fortune = rng.uniform() * total_;
        size_t i = 0;

        while (fortune >= 0) {
            fortune -= muts_[i++].second;
        }

        --i;
        return muts_[i].first->operator()(eo[i]);
    }

    std::string className() const {return "ArrayMutRoulette";}

private:
    double total_ = 0.0;
    MutArrayT muts_;
};

} /* hisi */

#endif /* end of include guard: ARRAY_OP_HPP_SNKZ09PA */
