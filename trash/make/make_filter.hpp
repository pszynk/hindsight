
#ifndef MAKE_FILTER_HPP_ZAQQEPIU
#define MAKE_FILTER_HPP_ZAQQEPIU

#include "hisi/core/tools/AcceptAllFlowFilter.hpp"

#include <utils/eoParser.h>
#include <utils/eoState.h>

namespace hisi
{
    namespace make
    {

FlowFilter& make_filter(eoParser& parser, eoState& state);

    } /* make */
} /* hisi */

#endif /* end of include guard: MAKE_FILTER_HPP_ZAQQEPIU */
