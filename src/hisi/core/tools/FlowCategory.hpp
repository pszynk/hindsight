
#ifndef FLOWCATEGORY_HPP_TTRZK2GE
#define FLOWCATEGORY_HPP_TTRZK2GE

#include <cstdint>

namespace hisi
{
    namespace utils
    {

enum class FlowCategory : std::uint8_t
{
    IGNORED = 0,
    SUSPICIOUS = 1,
    NORMAL = 2,
};

    } /* utils */
} /* hisi */

#endif /* end of include guard: FLOWCATEGORY_HPP_TTRZK2GE */
