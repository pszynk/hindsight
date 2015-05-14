
#ifndef MATCHRESULT_HPP_ANJWBKOR
#define MATCHRESULT_HPP_ANJWBKOR

#include <ostream>

namespace hisi
{

class MatchResult
{

public:

    class StatsPack
    {
    public:
        StatsPack(
                unsigned total = 0,
                unsigned considered = 0,
                unsigned matched = 0):
            total_(total), considered_(considered), matched_(matched)
        {};

        unsigned total() const {return total_;}
        unsigned considered() const {return considered_;}
        unsigned matched() const {return matched_;}
        unsigned unmatched() const {return considered_ - matched_;}

        unsigned ignored() const {return total_ - considered_;}

        void total(unsigned val) {total_ = val;}
        void considered(unsigned val) {considered_ = val;}
        void matched(unsigned val) {matched_ = val;}

        void incConsidered(unsigned val=1) {considered_ += val;}
        void incMatched(unsigned val=1) {matched_ += val;}

        void decConsidered(unsigned val=1) {if (considered_ >= val) considered_ -= val;}
        void decMatched(unsigned val=1) {if (matched_ >= val) matched_ -= val;}

    private:
        unsigned total_;
        unsigned considered_;
        unsigned matched_;

    };

    class InterruptRaport
    {
    public:
        InterruptRaport() {}
        InterruptRaport(unsigned breakPoint, unsigned patternLength):
            flag_(true), breakPoint_(breakPoint), patternLength_(patternLength)
        {}

        explicit operator bool() const {return flag_;}

        bool flag() const {return flag_;}
        bool breakPoint() const {return breakPoint_;}
        bool patternLength() const {return patternLength_;}

        void flugUp(bool val = true) {flag_ = val;}
        void breakPoint(unsigned val) {breakPoint_ = val;}
        void patternLength(unsigned val) {patternLength_ = val;}

    private:
        bool flag_ = false;
        unsigned breakPoint_ = 0;
        unsigned patternLength_ = 0;

    };

    MatchResult (StatsPack suspicious = {},
            StatsPack normal = {},
            InterruptRaport interrupt = {}):
        suspicious_(suspicious), normal_(normal), interrupt_(interrupt)
    {};

    const StatsPack& suspicious() const {return suspicious_;}
    StatsPack& suspicious() {return suspicious_;}
    const StatsPack& normal() const {return normal_;}
    StatsPack& normal() {return normal_;}

    const StatsPack& stats(bool isSuspicious) const
    {return isSuspicious ? suspicious_ : normal_;}
    StatsPack& stats(bool isSuspicious)
    {return isSuspicious ? suspicious_ : normal_;}

    const InterruptRaport& interrupt() const {return interrupt_;}
    InterruptRaport& interrupt() {return interrupt_;}

    void suspicious(StatsPack val) {suspicious_ = val;}
    void normal(StatsPack val) {normal_ = val;}

private:
    StatsPack suspicious_;
    StatsPack normal_;
    InterruptRaport interrupt_;

};
std::ostream&
operator<<(std::ostream &strm, const MatchResult::StatsPack &stats);

std::ostream&
operator<<(std::ostream &strm, const MatchResult::InterruptRaport &interrupt);

std::ostream&
operator<<(std::ostream &strm, const MatchResult &result);

} /* hisi */

#endif /* end of include guard: MATCHRESULT_HPP_ANJWBKOR */
