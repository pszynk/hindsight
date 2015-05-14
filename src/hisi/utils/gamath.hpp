
#ifndef GAMAT_HP_DIPEKNXL
#define GAMAT_HP_DIPEKNXL

#include <type_traits>

namespace hisi
{
    namespace utils
    {

template<typename T>
T r_wrap(T val, T lb, T range)
{
    if (val < lb)
        val += range * ((lb - val) / range + 1);

    return lb + (val - lb) % range;
}

template<typename T>
T u_wrap(T val, T lb, T ub)
{
   T range = ub - lb + 1;
   return r_add_wrap(val, lb, range);
}

//template<typename T>
//struct make_bigger_signed
//{
//    typedef typename std::enable_if<std::is_signed<T>::value, T>::type type;
//};

//template<>
//struct make_bigger_signed<unsigned char>
//{
//    typedef short type;
//};

//template<>
//struct make_bigger_signed<unsigned short>
//{
//    typedef int type;
//};

//template<>
//struct make_bigger_signed<unsigned>
//{
//    typedef long type;
//};

//template<>
//struct make_bigger_signed<unsigned long>
//{
//    typedef long long type;
//};

//int r_wrap(int val, const int lb, const int range)
//{
//    if (val < lb)
//        val += range * ((lb - val) / range + 1);

//    return lb + (val - lb) % range;
//}

//int u_wrap(int val, const int lb, const int ub)
//{
//   int range = ub - lb + 1;
//   return r_wrap(val, lb, range);
//}

//long r_wrap(long val, const long lb, const long range)
//{
//    if (val < lb)
//        val += range * ((lb - val) / range + 1);

//    return lb + (val - lb) % range;
//}

//long u_wrap(long val, const long lb, const long ub)
//{
//   long range = ub - lb + 1;
//   return r_wrap(val, lb, range);
//}

//long r_wrap(long long val, const long long lb, const long long range)
//{
//    if (val < lb)
//        val += range * ((lb - val) / range + 1);

//    return lb + (val - lb) % range;
//}

//long u_wrap(long val, const long lb, const long long ub)
//{
//   long range = ub - lb + 1;
//   return r_wrap(val, lb, range);
//}

//namespace
//{

//template<typename T>
//using Big = typename make_bigger_signed<T>::type;
//}
//template<typename T>
//T r_add_wrap(int by, Big<T> curr, Big<T> lb, Big<T> range)
//{
//    Big<T> val = by + curr;
//    if (val < lb)
//        val += range * ((lb - val) / range + 1);

//    return lb + (val - lb) % range;
//}

//template<typename T>
//T u_add_wrap(int by, Big<T> curr, Big<T> lb, Big<T> ub)
//{
//   Big<T> range = ub - lb + 1;
//   Big<T> val = by + curr;
//   if (val < lb)
//       val += range * ((lb - val) / range + 1);

//   return lb + (val - lb) % range;
//}

    } /* utils */
} /* hisi */
#endif /* end of include guard: GAMAT_HP_DIPEKNXL */
