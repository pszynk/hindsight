
#ifndef INTEGER_OP_HPP_B0ECFLYO
#define INTEGER_OP_HPP_B0ECFLYO

#include "hisi/utils/gamath.hpp"
#include "hisi/utils/constant.hpp"

#include <utils/eoRndGenerators.h>
#include <utils/eoUniformInit.h>

namespace hisi
{

//
// INTEGER
//

// GENERATORS
template<typename T>
using IntegerGenerator = eoRndGenerator<T>;

template<typename T>
using IntegerGeneratorUniform = eoUniformGenerator<T>;

template<typename T>
class IntegerGeneratorConstant : IntegerGenerator<T>
{
public:
    IntegerGeneratorConstant(T val):val_(val){}

    T operator()()
    {
        return val_;
    }

private:
    T val_;
};

template<typename T>
class IntegerGeneratorUnsafeUniform : public eoRndGenerator<T>
{
public :
    IntegerGeneratorUnsafeUniform(T _max = T(1.0), eoRng& _rng = rng) :
        minim(T(0.0)), range(_max), uniform(_rng) {}
    IntegerGeneratorUnsafeUniform(T _min, T _max, eoRng& _rng = rng) :
        minim(_min), range(_max-_min), uniform(_rng) {}

    T operator()(void) { return minim+static_cast<T>(uniform.uniform(range)); }
private :
    T minim;
    T range;
    eoRng& uniform;
};


// INITIATORS
template<typename T>
using IntegerInitUniform = eoUniformInit<T>;

template<typename T>
class IntegerInitUnsafeUniform : public eoInit<T>, private IntegerGeneratorUnsafeUniform<T>
{
public:
    using IntegerGeneratorUnsafeUniform<T>::IntegerGeneratorUnsafeUniform;

    void operator()(T& t) override
    {
        t = IntegerGeneratorUnsafeUniform<T>::operator()();
    }
};

// MUTATORS
template<typename T, class GenT>
class IntegerMutWrap : public eoMonOp<T>
{
public:
    //[lb, ub)
    IntegerMutWrap(T lb, T ub, GenT gen):
        lb_(lb), range_(ub - lb), gen_(gen)
    {
        if (lb > ub) {throw std::invalid_argument(
            "IntegerMutWrap lower bound "
            "must be smaller then upper bound!");}
    }

    bool operator()(T& eo) override {
        T by = 0;
        // force mutation!
        by = gen_();
        if(by == 0) {
            if(eo::rng.flip()) {
                by = T(-1);
            } else {
                by = 1;
            }
        }

        // proposed value
        T val = eo + by;

        eo = hisi::utils::r_wrap(val, lb_, range_);

        return by;
    }

    std::string className() const override {return "IntegerMutWrap";}

private:
    T lb_, range_;
    GenT gen_;
};

template<typename T>
using IntegerMutUniformWrap = IntegerMutWrap<T, eoUniformGenerator<T>>;

template<typename T>
using IntegerMutUnsafeUniformWrap = IntegerMutWrap<T, IntegerGeneratorUnsafeUniform<T>>;

template<typename T>
using IntegerMutConstantWrap = IntegerMutWrap<T, IntegerGeneratorConstant<T>>;


//
// BOOL
//
//

using BoolInitUniform = eoUniformInit<bool>;

class BoolMutFlip : public eoMonOp<bool>
{
public:
    bool operator()(bool& b) override
    {
        b = !b;
        return true;
    }
    std::string className() const override {return "BoolMutFlip";}
};

} /* hisi */

#endif /* end of include guard: INTEGER_OP_HPP_B0ECFLYO */
