
#ifndef POSITIONINDEX_HPP_LK4BMDYB
#define POSITIONINDEX_HPP_LK4BMDYB

#include <tokext/Position.hpp>
#include <tokext/SuffixContext.hpp>


#include <cstdint>
#include <map>

namespace hisi
{

class PositionIndex
{
public:
    using pos_t = int;
    using record_t = std::vector<std::vector<pos_t>>;
    using index_t = std::map<tokext::Flow::idx_t, record_t>;

    std::pair<bool, index_t::iterator> contains(const tokext::Position& position);
    void put(const tokext::Position& position, bool clean = true);
    void put(const index_t::iterator it, const tokext::Position& position);
    void nextLevel(bool clean = true);

    unsigned level() const {return level_;}
    bool empty() const {return suspicious_.empty() && normal_.empty();}
    unsigned suspiciousCount() const {return suspicious_.size();}
    unsigned normalCount() const {return normal_.size();}

    const index_t& suspicious() const {return suspicious_;}
    const index_t& normal() const {return normal_;}

private:
    unsigned level_ = 0;

    index_t suspicious_;
    index_t normal_;

    index_t& getIndex_(const tokext::Flow& flow);
    const index_t& getIndex_(const tokext::Flow& flow) const;
    void eraseIncompleteRecords_(index_t& index, unsigned level) const;

    friend std::ostream& operator<<(
            std::ostream& strm, const PositionIndex& index);
};

} /* hisi */

#endif /* end of include guard: POSITIONINDEX_HPP_LK4BMDYB */
