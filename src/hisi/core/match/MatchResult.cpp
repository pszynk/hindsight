
#include "MatchResult.hpp"


namespace hisi
{

std::ostream&
operator<<(std::ostream &strm, const MatchResult::StatsPack &stats)
{
    return strm <<
        "("<<stats.matched()<<" / "<<stats.considered()
        <<" | "<<stats.total()<<")";
}

std::ostream&
operator<<(std::ostream &strm, const MatchResult::InterruptRaport &interrupt)
{
    if(!interrupt) {
        return strm << "[X]";
    }

    return strm <<
        "("<<interrupt.breakPoint()<<" / "<<interrupt.patternLength()<<")";

}

std::ostream&
operator<<(std::ostream &strm, const MatchResult &result)
{
    return strm
        << "Suspicious -> " << result.suspicious() << std::endl
        << "Normal -> " << result.normal() << std::endl
        << "Interrupt -> " << result.interrupt();
}

} /* hisi */
