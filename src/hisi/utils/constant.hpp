
#ifndef CONSTANT_HPP_TIHV0OW4
#define CONSTANT_HPP_TIHV0OW4

namespace hisi
{
    namespace utils
    {
        namespace constant
        {

template<typename ... Args>
struct Truth
{
    bool operator()(const Args&...)
    {
        return true;
    }
};

template<typename T>
struct Value
{
    Value(T val):val_(val){}

    T operator()()
    {
        return val_;
    }

private:
    T val_;
};

        } /* constant */
    } /* utils */
} /* hisi */

#endif /* end of include guard: CONSTANT_HPP_TIHV0OW4 */
