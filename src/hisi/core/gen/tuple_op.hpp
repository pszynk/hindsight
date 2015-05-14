
#ifndef TUPLE_OP_HPP_3PF2JQKZ
#define TUPLE_OP_HPP_3PF2JQKZ

#include "hisi/utils/tuple_func.hpp"

#include <eoInit.h>

#include <tuple>

namespace hisi
{

namespace
{

template<typename... T>
using TupT = std::tuple<T...>;


struct SafeCall_ {
    template<typename T, typename U>
    static void call(T& t, U& u) {
        if(t)
            t->operator()(u);
    }
};

}


//
// INIT
//
template<typename... Args>
class TupleInit; //: public eoInit<TupT<Args...>>
////{};
template<typename... Args>
class TupleInit<TupT<Args...>> : public eoInit<TupT<Args...>>
{
    using TupleT = TupT<Args...>;
    using InitTupleT = TupT<eoInit<Args>*...>;
public:
    TupleInit(eoInit<Args>*... inits):inits_(std::make_tuple(inits...))
    {}

    TupleInit(InitTupleT inits):inits_(inits)
    {}

    void data(InitTupleT inits) {inits_(inits);}

    void operator()(TupleT& eo) override
    {
        utils::Caller<SafeCall_>::call(inits_, eo);
    }

    std::string className() const {return "TupleInit";}

private:
    InitTupleT inits_;
};


template<typename... Args>
TupleInit<std::tuple<Args...>> make_TupleInit(eoInit<Args>*... inits)
{
    TupleInit<std::tuple<Args...>> ti(inits...);
    return ti;
}
//TupleInit<Args...> make_TupleInit(eoInit<Args>*... inits)
//{
    //TupleInit<Args...> ti(inits...);
    //return ti;
//}

//
// MUT
//

template<typename... Args>
class TupleMut;

template<typename... Args>
class TupleMut<TupT<Args...>> : public eoMonOp<TupT<Args...>>
{
    using TupleT = TupT<Args...>;
    using MutTupleT = TupT<eoMonOp<Args>*...>;
public:

    TupleMut(eoMonOp<Args>*... muts):muts_(std::make_tuple(muts...))
    {}

    TupleMut(MutTupleT muts):muts_(muts)
    {}

    void data(MutTupleT muts) {muts_(muts);}

    bool operator()(TupleT& eo) override
    {
        utils::Caller<SafeCall_>::call(muts_, eo);
        return true;
    }

    std::string className() const {return "TupleMut";}

private:
    MutTupleT muts_;
};

template<typename... Args>
TupleInit<std::tuple<Args...>> make_TupleMut(eoMonOp<Args>*... muts)
{
    TupleMut<std::tuple<Args...>> tum(muts...);
    return tum;
}
} /* hisi */

#endif /* end of include guard: TUPLE_OP_HPP_3PF2JQKZ */
