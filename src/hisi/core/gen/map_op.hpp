
#ifndef MAP_OP_HPP_NP6U0PJZ
#define MAP_OP_HPP_NP6U0PJZ

#include "utils/constant.hpp"

#include <eoInit.h>

#include <map>
#include <stdexcept>
namespace hisi
{


//
// INIT
//

template<class MapT>
class MapCommonInit : public eoInit<MapT>
{
    using mapped_type = typename MapT::mapped_type;
    using key_type = typename MapT::key_type;

public:
    MapCommonInit(std::size_t size, eoInit<mapped_type>& init):size_(size), init_(init)
    {}

    void operator()(MapT& dict) override
    {
        for (auto aIt = dict.begin();  aIt != dict.end(); ++aIt)
        {
            init_(dict.second);
        }
    }

    std::string className() const {return "MapCommonInit";}

private:
    std::size_t size_;
    eoInit<mapped_type>& init_;
};


template<class MapT>
class MapIndivInit : public eoInit<MapT>
{
    using mapped_type = typename MapT::mapped_type;
    using key_type = typename MapT::key_type;

public:
    MapIndivInit(key_type key, eoInit<mapped_type>& init)
    {
        inits_[key] = &init;
    }

    void add(key_type key, eoInit<mapped_type>& init)
    {
        inits_[key] = &init;
    }

    void operator()(MapT& dict) override
    {
        for (auto iIt = inits_.begin();  iIt != inits_.end(); ++iIt)
        {
            iIt->second->operator()(dict[iIt->first]);
        }
    }

    std::string className() const {return "MapIndivInit";}

private:
    std::map<key_type, eoInit<mapped_type>*> inits_;
};

//
// MUT
//

template<class MapT, class Cond = utils::constant::Value<bool>>
class MapIndivMut : public eoMonOp<MapT>
{
    using mapped_type = typename MapT::mapped_type;
    using key_type = typename MapT::key_type;

public:
    MapIndivMut(key_type key, eoMonOp<mapped_type>& mut, Cond cond = {true}):cond_(cond)
    {
        muts_[key] = &mut;
    }

    void add(key_type key, eoMonOp<mapped_type>& mut)
    {
        muts_[key] = &mut;
    }

    bool operator()(MapT& dict) override
    {
        bool changed = false;
        auto dIt = dict.begin();
        auto mIt = muts_.begin();
        while(mIt != muts_.end())
        {
            if (dIt == dict.end()) {
                throw std::out_of_range("MapIndivInit out of range");
            }
            if (mIt->first != dIt->first) {
                ++dIt;
            }

            if (cond_()) {
                changed |= (mIt)->second->operator()(dIt->second);
            }
            ++dIt; ++mIt;
        }
        return changed;
    }

    std::string className() const {return "MapIndivMut";}

private:
    Cond cond_;
    std::map<key_type, eoMonOp<mapped_type>*> muts_;
};

//TODO: mutuj rulette wheel, i ile razy rolować [(08/02/15 10:56:53) - pawel]

} /* hisi */

#endif /* end of include guard: MAP_OP_HPP_NP6U0PJZ */
