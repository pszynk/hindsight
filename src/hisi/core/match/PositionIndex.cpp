
#include "PositionIndex.hpp"

#include <prettyprint/prettyprint.h>

namespace hisi
{

std::pair<bool, PositionIndex::index_t::iterator>
    PositionIndex::contains(const tokext::Position& position)
{
    auto& index = getIndex_(position.flow());
    auto it = index.find(position.flow().globalIdx());
    if(it != index.end()) {
        return std::make_pair(true, it);
    }
    return std::make_pair(false, it);
}

/*
 * put position in right record of the flow
 * if level == 0 -> assert fail
 * if level == 1 -> index[flow].push_back([pos])
 * else:
 *  1) find if record exists
 *      1a) if not, do nothing, return
 *  2) check if record.size() ==
 *      2a) level -> record.back.push_back(pos)
 *      2a) level-1 -> record.push_back([pos])
 *      2c) else remove this record and return
 */
void PositionIndex::put(const tokext::Position& position, bool clean)
{
    assert(level_ > 0);
    auto& index = getIndex_(position.flow());

    if(level_ == 1) {
        record_t& record = index[position.flow().globalIdx()];

        assert(record.size() <= 1);

        if(record.size() == 0) {
            record.push_back( {(PositionIndex::pos_t)position.offset()} );
            return;
        }
        record.back().push_back(position.offset());
        return;
    }

    auto it = index.find(position.flow().globalIdx());

    if(it == index.end()) {
        return;
    }

    if(it->second.size() == level_) {
        it->second.back().push_back(position.offset());
        return;
    }

    if(it->second.size() == level_ - 1) {
        it->second.push_back( {(PositionIndex::pos_t)position.offset()} );
        return;
    }

    if(clean) {
        index.erase(it);
    }
    return;
}

void PositionIndex::put(
        const index_t::iterator it,
        const tokext::Position& position)
{
    if(it->second.size() == level_) {
        it->second.back().push_back(position.offset());
        return;
    }

    if(it->second.size() == level_ - 1) {
        it->second.push_back( {(PositionIndex::pos_t)position.offset()} );
        return;
    }
}

void PositionIndex::nextLevel(bool clean)
{
    if(level_ == 0) {
        ++level_; return;
    }

    if(clean) {
        eraseIncompleteRecords_(suspicious_, level_);
        eraseIncompleteRecords_(normal_, level_);
    }
    ++level_;
}

PositionIndex::index_t& PositionIndex::getIndex_(const tokext::Flow& flow)
{
    return (flow.isSuspicious() ? suspicious_ : normal_);
}

const PositionIndex::index_t& PositionIndex::getIndex_(
        const tokext::Flow& flow) const
{
    return (flow.isSuspicious() ? suspicious_ : normal_);
}

void PositionIndex::eraseIncompleteRecords_(
        index_t& index, unsigned level) const
{
    auto it = index.begin();
    while(it != index.end()) {
        if (it->second.size() < level) {
            it = index.erase(it);
        }
        else {
            ++it;
        }
    }
}

std::ostream& operator<<(std::ostream& strm, const PositionIndex& index)
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


} /* hisi */
