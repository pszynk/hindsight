
#ifndef ACCEPTALLFLOWFILTER_HPP_EPVUF8ID
#define ACCEPTALLFLOWFILTER_HPP_EPVUF8ID

#include "FlowFilter.hpp"

namespace hisi
{

class AcceptAllFlowFilter : public FlowFilter
{
public:

    bool operator()(const tokext::Flow&) override {return true;};
    void randomize(unsigned, unsigned) override {};
};

} /* hisi */

#endif /* end of include guard: ACCEPTALLFLOWFILTER_HPP_EPVUF8ID */
