
#ifndef PC_OP_HPP_QHJ7UE5L
#define PC_OP_HPP_QHJ7UE5L

#include "PositionConstraint.hpp"
#include "../utils/gamath.hpp"
#include "../utils/constant.hpp"

#include <eoOp.h>
#include <eoInit.h>

#include <stdexcept>

namespace hisi
{

template<class GenT>
class PositionConstraintInit : public eoInit<PositionConstraintBase>
{
public:
    // leave bounds to gen!
    void operator()(PositionConstraintBase& eo) override
    {
        eo.valueRef() = gen_();
    }

    std::string className() const {return "PositionConstraintInit";}

private:
    GenT& gen_;
};

typedef PositionConstraintInit<eoUniformGenerator<int>> UniformPCInit;
typedef PositionConstraintInit<utils::constant::Value<int>> ConstantPCInit;


template<class GenT>
class PositionConstraintMut : public eoMonOp<PositionConstraintBase>
{
public:
    //[lb, ub)
    PositionConstraintMut(int lb, int ub, GenT& gen):
        lb_(lb), range_(ub - lb), gen_(gen)
    {
        if (lb > ub) {throw std::invalid_argument(
            "PositionConstraintMut lower bound "
            "must be smaller then upper bound!");}
    }

    bool operator()(PositionConstraintBase& eo) override {
        int by = 0;
        // force mutation!
        while(by == 0) { by = gen_();}
        // proposed value
        int val = eo.value() + by;

        eo.valueRef() = r_wrap(val, lb_, range_);

        return by;
    }

    std::string className() const {return "PositionConstraintMut";}

private:
    GenT& gen_;
    int lb_, range_;
};

typedef PositionConstraintMut<eoUniformGenerator<int>> UniformPCMut;
typedef PositionConstraintMut<utils::constant::Value<int>> ConstantPCMut;

class PositionConstraintPackInit : public eoInit<PositionConstraintPack>
{
public:
    PositionConstraintPackInit(
            eoInit<PositionConstraintBase>& init,
            PositionConstraintBase * const constr)
    {
        inits_.push_back(&init);
        constrs_.push_back(
                std::unique_ptr<PositionConstraintBase>(constr->clone()));
    }

    void add(
            eoInit<PositionConstraintBase>& init,
            PositionConstraintBase * const constr)
    {
        inits_.push_back(&init);
        constrs_.push_back(
                std::unique_ptr<PositionConstraintBase>(constr->clone()));
    }

    void operator()(PositionConstraintPack& pack) override
    {
        for (unsigned i = 0; i < inits_.size(); ++i)
        {
            inits_[i]->operator()(*constrs_[i]);
            pack.add(constrs_[i].get());
        }
    }

    std::string className() const {return "PositionConstraintPackInit";}
private:
    std::vector<eoInit<PositionConstraintBase>*> inits_;
    std::vector<std::unique_ptr<PositionConstraintBase>> constrs_;
};

class AllPositionConstraintPackMut : public eoMonOp<PositionConstraintPack>
{
public:
    AllPositionConstraintPackMut(eoMonOp<PositionConstraintBase>& mut)
    {
        muts_.push_back(&mut);
    }

    void add(eoMonOp<PositionConstraintBase>& mut)
    {
        muts_.push_back(&mut);
    }

    bool operator()(PositionConstraintPack& pack) override
    {
        bool flag = false;
        for (unsigned i = 0; i < muts_.size(); ++i)
        {
            flag |= muts_[i]->operator()(*pack[i]);
        }
        return flag;
    }

    std::string className() const {return "PositionConstraintPackMut";}
private:
    std::vector<eoMonOp<PositionConstraintBase>*> muts_;
};
} /* hisi */

#endif /* end of include guard: PC_OP_HPP_QHJ7UE5L */
