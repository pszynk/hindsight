
#include "sig_gen.hpp"

#include <cctype>
#include <sstream>

namespace hisi
{
    namespace utils
    {

static const char hex_chars[16] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

std::string snort_rep(const std::string& str)
{
    std::string ret = "";
    bool binary = false;
    for(auto c : str) {
        if(isalnum(c) || ispunct(c) || (c == ' ')) {

            if (binary) {
                binary = false;
                ret.push_back('|');
            }

            if(c == '\\' || c == ';' || c == '\"') {
                ret += {'\\', c};
            } else {
                ret.push_back(c);
            }
        } else {
            if (binary) {
                ret.push_back(' ');
            } else {
                binary = true;
                ret.push_back('|');
            }
            ret += hex_chars[ ( c & 0xF0 ) >> 4 ];
            ret += hex_chars[ ( c & 0x0F ) >> 0 ];
        }
    }
    if (binary) {
        ret.push_back('|');
    }
    return std::move(ret);
}
    } /* utils */
} /* hisi */
