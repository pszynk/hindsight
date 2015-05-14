
#ifndef POSITIONRULES_HPP_HC3Z7G6N
#define POSITIONRULES_HPP_HC3Z7G6N

#include "position_policies.hpp"

#include <array>
#include <tuple>

namespace hisi
{
    namespace pos
    {

using single_t = int;

using dual_t = std::array<single_t, 2>;

template<typename T>
using switch_t = std::tuple<bool, T>;

namespace
{
//template<std::size_t I>
//const single_t& get(const single_t&);

//template<>
//const single_t& get<0>(const single_t& val) {return val;};
// TODO make check a template method, and use get on everything
}

struct NullRule
{
    using ArgT = std::nullptr_t;
};

template<class Policy>
struct SingleRule
{
    using ArgT = single_t;
    static bool check(PolicyData data, const ArgT& value)
    {
        return Policy::check(data, value);
    }
};

template<class FirstPolicy, class SecondPolicy>
struct DualDependentRule
{
    using ArgT = dual_t;
    static bool check(PolicyData data, const ArgT& value)
    {
        if (FirstPolicy::check(data, value[0]))
            // first policy could modify start!
            return SecondPolicy::check(data, value[1]);
        return false;
    }
};


using OffsetDepthRule = DualDependentRule<OffsetPolicy, DepthPolicy>;
using DistanceWithinRule = DualDependentRule<DistancePolicy, WithinPolicy>;


//template<class FirstRule, class SecondRule>
//struct SwitchRule
//{
//    static_assert(std::is_same<
//            typename FirstRule::ArgT,
//            typename SecondRule::ArgT>::value,
//            "rules must take the same argument");

//    using ArgT = switch_t<typename FirstRule::ArgT>;
//    static bool check(PolicyData data, const ArgT& value)
//    {
//        using std::get;

//        if(get<0>(value))
//            return FirstRule::check(std::move(data), get<1>(value));

//        return SecondRule::check(std::move(data), get<1>(value));
//    }
//};



    } /* pos */
} /* hisi */

#endif /* end of include guard: POSITIONRULES_HPP_HC3Z7G6N */
