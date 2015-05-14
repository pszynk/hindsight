
#ifndef POSITIONRULEFUNC_HPP_GFLYQJRA
#define POSITIONRULEFUNC_HPP_GFLYQJRA

#include "position_policies.hpp"
#include "position_rules.hpp"
#include "hisi/core/gen/Content.hpp"

#include <cassert>

namespace hisi
{

// NullRule -> zwróć true
namespace
{

template<typename ContT, typename ArgT>
struct DefaultArgGetter
{
    static const ArgT& get(const ContT& c)
    {
        return static_cast<const ArgT&>(c);
    }
};
}


template<
    typename RuleT,
    typename ContT = Content<typename RuleT::ArgT>,
    class ArgGetter = DefaultArgGetter<ContT, typename RuleT::ArgT>
    >
struct PositionRuleFunc
{
    using Rule = RuleT;
    using RuleArg = typename RuleT::ArgT;
    using ContentType = ContT;
    using DataT = pos::PolicyData;
    static bool check(DataT data, const ContT& cont)
    {
        return RuleT::check(std::move(data), ArgGetter::get(cont));
    }
};


template<typename ContT = DefaultTokenIndexType>
struct NullRuleFunc
{
    using Rule = pos::NullRule;
    using RuleArg = pos::NullRule::ArgT;
    using ContentType = ContT;
    using DataT = pos::NullPolicyData;
    static bool check(DataT, const ContT&)
    {
        return true;
    }
};

} /* hisi */

#endif /* end of include guard: POSITIONRULEFUNC_HPP_GFLYQJRA */
