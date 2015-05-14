
#ifndef PCCHECKER_HPP_5XCFNESX
#define PCCHECKER_HPP_5XCFNESX

#include "PCFactory.hpp"

namespace hisi
{

template<class K>
class ArgMapAccess
{
    static const PCFactory::ArgMap& gain(const K& type)
    {
        return static_cast<PCFactory::ArgMap&>(type);
    }
};

template<class T, class Access = ArgMapAccess<T>>
class PCChecker {

public:
    PCChecker(PCFactory::FuncMap funcMap):funcMap_(funcMap)
    {}

    bool check(const T& type, int proposed, int previous)
    {
        auto args = Access::gain(type);
        auto aIt = args.begin();
        auto fIt = funcMap_.begin();
        while(fIt != funcMap_.end()) {

            if (aIt == args.end()) {
                throw std::out_of_range("PCChecker out of range, not enough arguments!");
            }
            if (fIt->first != aIt->first) {
                //TODO warning unused argument!
                ++aIt;
            }

            if (!fIt->second->operator()(aIt->second, proposed, previous)) {
                return false;
            }
            ++aIt; ++fIt;
        }
        return true;;
    }

private:
    PCFactory::FuncMap funcMap_;
};


} /* hisi */

#endif /* end of include guard: PCCHECKER_HPP_5XCFNESX */
