
#ifndef FLOWFILTER_HPP_D0Y5E12Z
#define FLOWFILTER_HPP_D0Y5E12Z

#include <tokext/Flow.hpp>

#include <eoFunctor.h>

namespace hisi
{

class FlowFilter : public eoUF<const tokext::Flow&, bool>
{
public:
    virtual void randomize(unsigned, unsigned) = 0;
};

} /* hisi */

#endif /* end of include guard: FLOWFILTER_HPP_D0Y5E12Z */
