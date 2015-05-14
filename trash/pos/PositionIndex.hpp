
#ifndef POSITIONINDEX_HPP_LK4BMDYB
#define POSITIONINDEX_HPP_LK4BMDYB

#include <tokext/Token.hpp>
#include <tokext/Flow.hpp>
#include <tokext/Position.hpp>
#include <tokext/SuffixContext.hpp>

#include <prettyprint/prettyprint.h>

#include <map>

namespace hisi
{
    namespace pos_
    {


template<class RecordT>
class PositionIndex
{
public:

    RecordT* find(const tokext::Flow& flow)
    {
        auto& data = getData_(flow);
        auto it = data.find(flow.globalIdx());
        if (it != data.end()) {
            return &it->second;
        }
        return nullptr;
    }

    const RecordT* find(const tokext::Flow& flow) const
    {
        const auto& data = getData_(flow);
        const auto it = data.find(flow.globalIdx());
        if (it != data.end()) {
            return &it->second;
        }
        return nullptr;
    }

    void put(const tokext::Flow& flow, RecordT record)
    {
        if(!record.valid()) {return;}

        auto& data = getData_(flow);
        /*auto p =*/ data.insert(
                std::make_pair(flow.globalIdx(), std::move(record)));
    }


    bool empty() const {
        return suspicious_.empty() && normal_.empty();
    }

    unsigned suspicious() const {
        return suspicious_.size();
    }

    unsigned normal() const {
        return normal_.size();
    }

    template<class R>
    friend std::ostream& operator<<(std::ostream& strm, const PositionIndex<R>& index)
    {
        strm << "PositionIndex {" << std::endl
            << " SUSPICIOUS: [" << std::endl;
        for(auto& s : index.suspicious_) {
            strm << "  " << s << std::endl;
        }

        strm << "]" << std::endl << " NORMAL: [" << std::endl;
        for(auto& n : index.normal_) {
            strm << "  " << n << std::endl;
        }
        return strm << "]}";
    }

private:

    using data_t = std::map<tokext::Flow::idx_t, RecordT>;

    data_t suspicious_;
    data_t normal_;

    data_t& getData_(const tokext::Flow& flow)
    {return (flow.isSuspicious() ? suspicious_ : normal_);};

    const data_t& getData_(const tokext::Flow& flow) const
    {return (flow.isSuspicious() ? suspicious_ : normal_);};

};

    } /* pos_ */
} /* hisi */

#endif /* end of include guard: POSITIONINDEX_HPP_LK4BMDYB */
