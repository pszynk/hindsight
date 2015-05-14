
#ifndef POSITION_PREDICATES_HPP_6UC1QYWN
#define POSITION_PREDICATES_HPP_6UC1QYWN


namespace hisi
{


template<typename ArgT>
class NoPredicate
{
protected:
    NoPredicate(const ArgT&){}

    bool check(unsigned) {return true;}
};

} /* hisi */

#endif /* end of include guard: POSITION_PREDICATES_HPP_6UC1QYWN */
