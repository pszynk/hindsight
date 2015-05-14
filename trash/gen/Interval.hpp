
#ifndef INTERVAL_HPP_XIVP8OZS
#define INTERVAL_HPP_XIVP8OZS

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <sstream>

namespace hisi
{

class Interval
{
public:
    using idx_t = std::size_t;

    Interval() {}

    explicit Interval(idx_t val):lb_(val), ub_(val) {}

    static Interval byBounds(idx_t lb, idx_t ub)
    { return Interval(lb, ub); }

    static Interval bySpace(idx_t lb, idx_t space)
    { return Interval(lb, lb + space); }

    idx_t size() const { return ub_ - lb_; }
    idx_t lowerBound() const { return lb_; }
    idx_t upperBound() const { return ub_; }

    void bounds(idx_t lb, idx_t ub) {
        check_(lb, ub);
        lb_ = lb;
        ub_ = ub;
    }

    void safeBounds(idx_t val1, idx_t val2) {
        auto p = std::minmax(val1, val2);
        lb_ = p.first;
        ub_ = p.second;
    }


    void space(idx_t lb, idx_t space) { bounds(lb, lb + space); }

    void lowerBound(idx_t val) {
        check_(val, ub_);
        lb_ = val;
    }

    void upperBound(idx_t val) {
        check_(lb_, val);
        ub_ = val;
    }

private:

    Interval(idx_t lb, idx_t ub):lb_(lb), ub_(ub)
    { check_(lb, ub); }

    static void check_(Interval::idx_t lb, idx_t ub)
    {
        if (lb > ub)
        {
            std::stringstream ss;
            ss << "Interval: lower bound must not be grater then upper bound!: [lb=]"<<lb
                <<" <= [ub=]"<<ub;
            throw std::invalid_argument(ss.str());
        }
    }

    idx_t lb_ = 0, ub_ = 0;
};

bool inInterval(const Interval& interval, Interval::idx_t val)
{
    return (val >= interval.lowerBound() && val <= interval.upperBound());
}

struct InInterval
{
    using first_argument_type = Interval;
    using second_argument_type = Interval::idx_t;
    using result_type = bool;

    bool operator() (const Interval& interval, Interval::idx_t val)
    {
        return inInterval(interval, val);
    }
};

} /* hisi */

#endif /* end of include guard: INTERVAL_HPP_XIVP8OZS */
