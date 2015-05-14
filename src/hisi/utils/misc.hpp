
#ifndef MISC_HPP_TA8FBQ79
#define MISC_HPP_TA8FBQ79

#include <type_traits>
#include <memory>
#include <utility>

namespace hisi
{
    namespace utils
    {

template <typename EnumT>
auto enum_as_integer(EnumT const value)
    -> typename std::underlying_type<EnumT>::type
{
    return static_cast<typename std::underlying_type<EnumT>::type>(value);
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args) {
   static_assert(std::extent<T>::value == 0,
       "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

   typedef typename std::remove_extent<T>::type U;
   return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
   return make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}


    } /* utils */
} /* hisi */

#endif /* end of include guard: MISC_HPP_TA8FBQ79 */
