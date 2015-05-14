
#ifndef PRINTER_HPP_VLPFBBWQ
#define PRINTER_HPP_VLPFBBWQ

#include <iostream>
#include <tuple>
#include <array>

namespace hisi
{
    namespace utils
    {

template<typename T, size_t I>
auto operator<<(std::ostream& os, std::array<T, I> const& t) -> std::ostream&
{
    for(size_t i = 0; i < I - 1; ++i) {
        os << t[i] << ' ';
    }
    os << t[I-1];
    return os;
}


template<typename T, size_t I>
auto operator>>(std::istream& is, std::array<T, I>& t) -> std::istream&
{
    for(size_t i = 0; i < I; ++i) {
        is >> t[i];
    }
    return is;
}

namespace aux{
template<size_t...> struct seq{};

template<size_t N, size_t... Is>
struct gen_seq : gen_seq<N-1, N-1, Is...>{};

template<size_t... Is>
struct gen_seq<0, Is...> : seq<Is...>{};

template<class Tuple, size_t... Is>
void print_tuple(std::ostream& os, Tuple const& t, seq<Is...>){
  using swallow = int[];
  (void)swallow{0, ((os << (Is == 0 ? "" : " ") << std::get<Is>(t)), 0)...};
  //(void)swallow{0, ((os << (Is == 0 ? "" : " "), myPrint(os, get<Is>(t))), 0)...};
}

template<class Tuple, size_t... Is>
void read_tuple(std::istream& is, Tuple& t, seq<Is...>){
  using swallow = int[];
  (void)swallow{0, ((is >> std::get<Is>(t)), 0)...};
}
} // aux::

template<class... Args>
auto operator<<(std::ostream& os, std::tuple<Args...> const& t)
    -> std::ostream&
{
  aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
  return os;
}


template<class... Args>
auto operator>>(std::istream& is, std::tuple<Args...>& t)
    -> std::istream&
{
  aux::read_tuple(is, t, aux::gen_seq<sizeof...(Args)>());
  return is;
}

    } /* utils */
} /* hisi */
#endif /* end of include guard: PRINTER_HPP_VLPFBBWQ */

//template<typename T>
//void myPrint(ostream& os, const T& t) {
//    os << t <<'z';
//}

//template<typename T, size_t I>
//void myPrint(ostream& os, const array<T, I>& t) {
//  for(size_t i = 0; i < I - 1; ++i) {
//      os << t[i] << ' ';
//  }
//  os << t[I-1];
//}


//template<class... Args>
//void myPrint(ostream& os, const tuple<Args...>& t) {
//  aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
//}
