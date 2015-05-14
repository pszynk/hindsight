
#include "IdxBounds.hpp"

namespace hisi
{

int TrimAddPolicy::add(idx_t_ val, int by, idx_t_ lb, idx_t_ ub)
{
    if (by < 0) {
        if (idx_t_(abs(by)) > (val - lb)) {
            return lb;
        }
        return val + by;

    }

    if (idx_t_(abs(by)) > (ub - val)) {
        return ub;
    }
    return val + by;
}

int WrapAddPolicy::add(idx_t_ val, int by, idx_t_ lb, idx_t_ ub)
{
    if (by < 0) {
        idx_t_ dist = val - lb;
        if (idx_t_(abs(by)) > dist) {
            return ub - (abs(by) - dist) + 1;
        }
        return val + by;

    }
    idx_t_ dist = ub - val;
    if (idx_t_(abs(by)) > dist) {
        return lb + (abs(by) - dist) -1;
    }
    return val + by;
}

int BounceAddPolicy::add(idx_t_ val, int by, idx_t_ lb, idx_t_ ub)
{
    if (by < 0) {

        idx_t_ dist = val - lb;
        if (idx_t_(abs(by)) > dist) {
            return lb + (abs(by) - dist);
        }

        return val + by;
    }

    idx_t_ dist = ub - val;
    if (idx_t_(abs(by)) > dist) {
        return ub - (abs(by) - dist);
    }

    return val + by;
}

} /* hisi */
