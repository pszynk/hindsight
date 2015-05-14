
#ifndef SIG_GEN_HPP_HQXRIDZW
#define SIG_GEN_HPP_HQXRIDZW

#include "printer.hpp"

#include "hisi/core/gen/Content.hpp"
#include "hisi/core/pos/position_rules.hpp"

#include <tokext/utils/translation.hpp>

#include <string>
#include <sstream>

namespace hisi
{
    namespace utils
    {

std::string snort_rep(const std::string& str);

        namespace
        {

template<typename T>
std::string attr_rep(const T&) {return "";}

template<typename T>
std::string attr_rep(const Content<T> &c) {
    std::stringstream ss;
    ss << c.attr();
    return ss.str();
}

template<typename T, class Mapper>
std::string convert(const T &t, const Mapper& mapper,
        const tokext::SuffixContext& scx) {
    using std::string;
    auto token = mapper[t];
    string tokstr = tokext::utils::translateToken<string>(scx, token, true);
    string ret = string("\"") + snort_rep(tokstr) + string("\"");
    string attr = attr_rep(t);
    if (attr.empty()) {
        return ret;
    }
    ret += string("(") + std::to_string(tokstr.size()) + string("): ") + attr;
    return ret;
}
        }

template<class Vec, class Mapper>
std::string generate_sig(const Vec &vec, const Mapper& mapper, const tokext::SuffixContext& scx) {
    std::string ret = "";
    for(auto c : vec) {
        ret += convert(c, mapper, scx) + "; ";
    }
    return ret;
}

    } /* utils */
} /* hisi */

#endif /* end of include guard: SIG_GEN_HPP_HQXRIDZW */
