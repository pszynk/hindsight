
#ifndef DATAINFO_HPP_M1HZCJRF
#define DATAINFO_HPP_M1HZCJRF

#include <utils/eoParam.h>


#include <limits>
#include <cstdint>
#include <sstream>

namespace hisi
{
    namespace load
    {

struct DataInfo
{
    unsigned tokenCount = std::numeric_limits<unsigned>::max();
    unsigned suspCount = std::numeric_limits<unsigned>::max();
    unsigned normCount = std::numeric_limits<unsigned>::max();
    int maxFlowLength = std::numeric_limits<uint16_t>::max();

    int minSigLen = 2;
    int maxSigLen = 15;
    bool constSize = false;

    std::pair<int, int> attr1_range = {0, 0};
    std::pair<int, int> attr2_range = {0, 0};
};


    } /* load */
} /* hisi */

#endif /* end of include guard: DATAINFO_HPP_M1HZCJRF */
