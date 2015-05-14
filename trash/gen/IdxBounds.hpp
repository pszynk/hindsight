
#ifndef IDXBOUNDS_HPP_PLXKJZZI
#define IDXBOUNDS_HPP_PLXKJZZI

#include <cstddef>
#include <stdexcept>

namespace
{
    using idx_t_ = std::size_t;
}
namespace hisi
{

template<class AddPolicy>
class IdxBounds
{
public:
    using idx_t = idx_t_;

    IdxBounds(idx_t lb, idx_t ub) : lb_(lb), ub_(ub)
    {
        if (lb > ub) {
            throw std::invalid_argument("IdxBounds constructor, wrong bounds!");
        }
    }

    idx_t lb() const {return lb_;}
    idx_t ub() const {return ub_;}

    idx_t add(idx_t val, int by) const
    {return AddPolicy::add(val, by, lb_, ub_);}

private:
    idx_t lb_, ub_;
};

class TrimAddPolicy
{
    static int add(
            idx_t_ val,
            int by,
            idx_t_ lb,
            idx_t_ ub);
};

class WrapAddPolicy
{
    static int add(
            idx_t_ val,
            int by,
            idx_t_ lb,
            idx_t_ ub);
};

class BounceAddPolicy
{
    static int add(
            idx_t_ val,
            int by,
            idx_t_ lb,
            idx_t_ ub);
};

} /* hisi */

#endif /* end of include guard: IDXBOUNDS_HPP_PLXKJZZI */
