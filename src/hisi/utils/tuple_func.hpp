
#ifndef TUPLE_FUNC_HPP_H5W4AHXP
#define TUPLE_FUNC_HPP_H5W4AHXP

#include <tuple>
#include <utility>



namespace hisi
{
    namespace utils
    {

namespace
{

// sequences
template<int... Is> struct seq {};
template<int N, int... Is> struct gen_seq : gen_seq<N-1, N-1, Is...> {};
template<int... Is> struct gen_seq<0, Is...> : seq<Is...> {};


// apply
template<int N, class T, class F>
auto apply_one(T& p, F func) -> decltype(func( std::get<0>(p) ))
{
    return func( std::get<N>(p) );
}

template<class T, class F, int... Is>
auto apply(T& p, int index, F func, seq<Is...>) -> decltype(func( std::get<0>(p) ))
{
    using FT = decltype(func( std::get<0>(p) ))(T&, F);
    static constexpr FT* arr[] = { &apply_one<Is, T, F>... };
    return arr[index](p, func);
}

} /* anonymus */

template<class T, class F>
auto apply(T& p, int index, F func) -> decltype(func( std::get<0>(p) ))
{
    return apply(p, index, func, gen_seq<std::tuple_size<T>::value>{});
}

// map_tuple
template<
    std::size_t I = 0,
    typename FuncT,
    typename Zero,
    typename... Tp,
    typename... Up>
typename std::enable_if<I == sizeof...(Tp), Zero>::type
map_tuple(std::tuple<Tp...>&, std::tuple<Up...>&, FuncT, Zero zero)
{
    static_assert(sizeof...(Tp) == sizeof...(Up),
            "map_tuple, tuples must be equal sizes!");
    return zero;
}

template<
    std::size_t I = 0,
    typename FuncT,
    typename Zero,
    typename... Tp,
    typename... Up>
typename std::enable_if<I < sizeof...(Tp), Zero>::type
map_tuple(std::tuple<Tp...>& t, std::tuple<Up...>& u, FuncT f, Zero z)
{
    return f(std::get<I>(u)(std::get<I>(t)), map_tuple<I + 1>(t, u, f, z));
}

template<
    std::size_t I = 0,
    typename... Tp,
    typename... Up>
typename std::enable_if<I == sizeof...(Tp), void>::type
map_tuple(std::tuple<Tp...>&, std::tuple<Up...>&)
{
    static_assert(sizeof...(Tp) == sizeof...(Up),
            "map_tuple, tuples must be equal sizes!");
    return void();
}

template<
    std::size_t I = 0,
    typename... Tp,
    typename... Up>
typename std::enable_if<I < sizeof...(Tp), void>::type
map_tuple(std::tuple<Tp...>& t, std::tuple<Up...>& u)
{
    std::get<I>(u)(std::get<I>(t));
    map_tuple<I + 1>(t, u);
}

namespace
{

template< std::size_t I, class Func, typename TupF, typename TupV>
struct CallerHelper_
{
    static void call(TupF& tf, TupV& tv)
    {
        Func::call(std::get<I-1>(tf), std::get<I-1>(tv));
        CallerHelper_<I-1, Func, TupF, TupV>::call(tf, tv);
    }
};

template<class Func,typename TupF, typename TupV>
struct CallerHelper_<0, Func, TupF, TupV>
{
    static void call(TupF& tf, TupV& tv)
    {
        Func::call(std::get<0>(tf), std::get<0>(tv));
    }
};

} /* anonymus */

template<class Func>
struct Caller {
    template<class...Tf, class... Tv>
    static void call(std::tuple<Tf...>& tf, std::tuple<Tv...>& tv)
    {
        return CallerHelper_<sizeof...(Tf), Func, std::tuple<Tf...>&, std::tuple<Tv...>&>::call(tf, tv);
    }
};


    } /* utils */
} /* hisi */

#endif /* end of include guard: TUPLE_FUNC_HPP_H5W4AHXP */
