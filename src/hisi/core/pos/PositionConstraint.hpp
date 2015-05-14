
#ifndef POSITIONCONSTRAINT_HPP_2GFNSUYU
#define POSITIONCONSTRAINT_HPP_2GFNSUYU

#include <cassert>
#include <cstdlib>
#include <vector>
#include <memory>

namespace hisi
{

class PositionConstraintBase
{
public:
    virtual PositionConstraintBase* clone() const = 0;

    virtual bool operator()(int constraint, int proposed, int previous) const = 0;
    virtual ~PositionConstraintBase(){};

};

template<class ConstraintPolicy>
class PositionConstraint : public PositionConstraintBase
{
public:
    PositionConstraint<ConstraintPolicy>* clone() const override
    { return new PositionConstraint<ConstraintPolicy>(*this); }

    bool operator()(int constraint, int proposed, int previous) const override
    {
        return ConstraintPolicy::check(constraint, proposed, previous);
    }

    //virtual ~PositionConstraint (){};
};



} /* hisi */

#endif /* end of include guard: POSITIONCONSTRAINT_HPP_2GFNSUYU */
