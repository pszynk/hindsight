
#ifndef STH_HPP_KXDLNZ0J
#define STH_HPP_KXDLNZ0J

#include <iostream>
#include <tuple>
#include <array>
#include "hisi/utils/printer.hpp"
namespace hisi
{

void func()
{

    using namespace std;
    using namespace hisi::utils;
    using comp_tuple = std::tuple<array<int, 2>, tuple<bool, double>>;
    comp_tuple tt {{{1,2}},std::tuple<bool, double>{true,1.1}};
    cout << tt << " to jest tt" << endl;
}
} /* hisi */

#endif /* end of include guard: STH_HPP_KXDLNZ0J */
