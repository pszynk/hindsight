
#ifndef MATCHINGSTATE_HPP_DTMALWJN
#define MATCHINGSTATE_HPP_DTMALWJN

#include <set>

namespace hisi
{

template<typename T>
class Interval
{
public:
    Interval (T start, T length):
        start_(start), length_(length)
    {}

    T start() const {return start_;}
    T length() const {return length_;}
    T stop() const {return start_ + length_;}

private:
    T start_;
    T length_;
};

template<typename T>
struct IntervalComparator
{
  bool operator() (const Interval<T>& lhs, const Interval<T>& rhs) const {
      return lhs.start() < rhs.start();

      //return lhs.start() < rhs.start() ||
          //(!rhs.start() < lhs.start() && lhs.length() < rhs.length());
  }
};

template<typename T>
class MatchingState
{
public:
    MatchingState():valid_(false)
    {}

    bool valid() const {return valid_ && !state_.empty();}
    explicit operator bool() const {return valid();}
    void invalidate() {valid_ = false;}

    std::size_t size() const {return state_.size();}

    MatchingState<T> next(Interval<T> interval) const {
        // state is empty? any interval is ok
        if(state_.size() == 0) {
            return MatchingState(interval);
        }

        // get iter to interval that starts at the same point, or
        // further
        auto it = state_.lower_bound(interval);

        // if such interval exists AND
        // new interval stops at its starting point, or AFTER its starting point
        // then no luck!       X           X           OK
        // new interval ->  |    |  ;  |    |     ;  |    |
        // one after    ->   |  |   ;       |  |  ;        |   |
        if(it != state_.end() &&
                (interval.stop() > it->start())) {
            return MatchingState();
        }

        // if there is some interval before the new one AND
        // new interval starts at olds stopping point, or BEFORE its stopping point
        // then no luck!       X           X           OK
        // one after    ->   |  |   ;       |  |  ;        |   |
        // new interval ->  |    |  ;  |    |     ;  |    |
        if(it != state_.begin() &&
                (std::prev(it)->stop() > interval.start())) {
            return MatchingState();
        }

        // all is cool
        MatchingState<T> ret(*this);
        ret.state_.insert(std::move(interval));
        return ret;
    }

    T point() const {
        if(state_.empty()) {
            return 0;
        }
        return state_.rbegin()->stop();
    }

private:
    using state_t_ = std::set<Interval<T>, IntervalComparator<T>>;

    bool valid_;
    state_t_ state_;

    MatchingState(Interval<T> interval):valid_(true) {
        state_.insert(std::move(interval));
    }

};
} /* hisi */

#endif /* end of include guard: MATCHINGSTATE_HPP_DTMALWJN */
