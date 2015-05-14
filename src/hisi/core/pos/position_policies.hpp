
#ifndef POSITION_POLICIES_HPP_EFIXP156
#define POSITION_POLICIES_HPP_EFIXP156

#include <cassert>
#include <cstdlib>
#include <algorithm>

namespace hisi
{
    namespace pos
    {

class PolicyData
{
public:
    PolicyData (
            int relative,
            int start
            //int len
            ):relative_(relative), start_(start) //, len_(len)
    {
        assert(relative >= 0);
        assert(start >= 0);
        //assert(len > 0);
    }

    const int& absolute() const {return absolute_;}
    int& absolute() {return absolute_;}

    const int& relative() const {return relative_;}
    int& relative() {return relative_;}

    int start() const {return start_;}
    //int len() const {return len_;}

private:
    int absolute_ = 0;
    int relative_;
    int start_; //, len_;
};

struct NullPolicyData
{
    NullPolicyData(int, int){}
};

// MODIFIES start!
struct OffsetPolicy
{
    // relative can be whatever, but it always is treated as 0
    static bool check(PolicyData &data, int value)
    {
        //assert(value >= 0); can't do that, coz can be switched with distance
        data.absolute() = std::max(value, 0); // but shouldn change relative to be < 0
        return data.start() >= value;
    }
};

struct DepthPolicy
{
    static bool check(PolicyData &data, int value)
    {
        assert(value >= 0);
        return data.start() - data.absolute() <= value;
    }
};

// MODIFIES start!
struct DistancePolicy
{
    // relative should be position of last byte in previous token, or
    // 0 if this is first one
    static bool check(PolicyData &data, int value)
    {
        data.relative() += value;
        if(data.relative() <= 0) {
            data.relative() = 0;
            // start >= 0 ! always!
            return true;
        }
        return data.start() >= data.relative();
    }
};

struct WithinPolicy
{
    static bool check(PolicyData &data, int value)
    {
        assert(value >= 0);
        return data.start() >= data.relative() &&
            data.start() -  data.relative()                <= value;
            //&& data.relative() - (data.start() + data.len())  <= value;
    }
};

    } /* pos */
} /* hisi */

#endif /* end of include guard: POSITION_POLICIES_HPP_EFIXP156 */
