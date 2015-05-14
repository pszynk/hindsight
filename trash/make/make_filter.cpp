
#include "make_filter.hpp"

#include "hisi/core/tools/AcceptAllFlowFilter.hpp"

namespace hisi
{
    namespace make
    {

FlowFilter& make_filter(eoParser& /*parser*/, eoState& state)
{
    // Just return AcceptAll but coz thats the only one
    FlowFilter* filter = new AcceptAllFlowFilter();
    state.storeFunctor(filter);
    return *filter;
}

    } /* make */
} /* hisi */
