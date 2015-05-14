
#ifndef INTERVAL_OP_HPP_XQYD3MIJ
#define INTERVAL_OP_HPP_XQYD3MIJ

#include "Interval.hpp"

//EO
#include <utils/eoRndGenerators.h>
#include <eoInit.h>

//std
#include <algorithm>

namespace hisi
{


// GENERATORS ------------------------------------------------------------------
template<class GenT>
class IntervalByBoundsGenerator : public eoRndGenerator<Interval>
{
public:
    IntervalByBoundsGenerator(GenT& gen):
        gen_(gen)
    {}

    Interval operator()() override {
        auto p = std::minmax(gen_(), gen_());
        return Interval::byBounds(p.first, p.second);
    }

private:
    GenT& gen_;
};

//TODO: ograniczyc ub bo albo slabe losowanie, albo bledy [(01/02/15 15:07:58) - pawel]
template<class LbGenT, class SpaceGenT>
class IntervalBySpaceGenerator : public eoRndGenerator<Interval>
{
public:
    IntervalBySpaceGenerator(LbGenT& lbGen, SpaceGenT& spaceGen):
        lbGen_(lbGen), spaceGen_(spaceGen)
    {}

    Interval operator()() override {
        return Interval::bySpace(lbGen_(), spaceGen_());
    }

private:
    LbGenT& lbGen_;
    SpaceGenT& spaceGen_;
};

// INIT ------------------------------------------------------------------------
template<class GenT>
class IntervalByBoundsInit : public eoInit<Interval>
{
public:
    IntervalByBoundsInit(GenT& gen):
        gen_(gen)
    {}

    void operator()(Interval& interval) override {
        auto p = std::minmax(gen_(), gen_());
        return interval.bounds(p.first, p.second);
    }

private:
    GenT& gen_;
};

//TODO: to samo co z generatorem [(01/02/15 15:08:13) - pawel]
template<class LbGenT, class SpaceGenT>
class IntervalBySpaceInit : public eoInit<Interval>
{
public:
    IntervalBySpaceInit(LbGenT& lbGen, SpaceGenT& spaceGen):
        lbGen_(lbGen), spaceGen_(spaceGen)
    {}

    void operator()(Interval& interval) override {
        interval.space(lbGen_(), spaceGen_());
    }

private:
    LbGenT& lbGen_;
    SpaceGenT& spaceGen_;
};

template<class GenT>
class IntervalMoveMutation : public eoMonOp<Interval>
{
public:
    IntervalMoveMutation(GenT& gen):
        gen_(gen)
    {}

    bool operator()(Interval& eo) override {
        auto by = gen_();
        eo.safeBounds(eo.lowerBound() + by, eo.upperBound() + by);
        return by;
    }

private:
    GenT& gen_;
};

template<class GenT>
class IntervalResizeMutation : public eoMonOp<Interval>
{
public:
    IntervalResizeMutation(GenT& gen):
        gen_(gen)
    {}

    bool operator()(Interval& eo) override {
        auto by = gen_();
        eo.safeBounds(eo.lowerBound() + by, eo.upperBound());
        return by;
    }

private:
    GenT& gen_;
};


template<class BoundsT, class GenT>
class IntervalBoundedMoveMutation : public eoMonOp<Interval>
{
public:
    IntervalBoundedMoveMutation(BoundsT bounds, GenT& gen):
        bounds_(bounds), gen_(gen)
    {}

    bool operator()(Interval& eo) override {
        auto by = gen_();
        eo.safeBounds(bounds_.add(eo.lowerBound(), by), bounds_.add(eo.upperBound()));
        return by;
    }

private:
    BoundsT bounds_;
    GenT& gen_;
};

template<class BoundsT, class GenT>
class IntervalBoundedResizeMutation : public eoMonOp<Interval>
{
public:
    IntervalBoundedResizeMutation(BoundsT bounds, GenT& gen):
        bounds_(bounds), gen_(gen)
    {}

    bool operator()(Interval& eo) override {
        auto by = gen_();
        eo.safeBounds(bounds_.add(eo.lowerBound(), by), eo.upperBound());
        return by;
    }

private:
    BoundsT bounds_;
    GenT& gen_;
};

} /* hisi */

#endif /* end of include guard: INTERVAL_OP_HPP_XQYD3MIJ */
