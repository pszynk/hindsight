
#ifndef FIRSTPOSITIONRECORD_HPP_CZKBSQ9G
#define FIRSTPOSITIONRECORD_HPP_CZKBSQ9G

#include "../position_predicates.hpp"

#include <tokext/Token.hpp>
#include <tokext/Position.hpp>

namespace hisi
{

template<typename PredArgT, class PosPredT = NoPredicate<PredArgT>>
class FirstPositionRecord : PosPredT
{
public:

    FirstPositionRecord(
            const PredArgT& arg,
            const tokext::Token& token,
            const tokext::Position& pos): PosPredT(arg)
    {
        initiate(token, pos);
    }

    FirstPositionRecord(
            const PredArgT& arg,
            const FirstPositionRecord& prev,
            const tokext::Token& token,
            const tokext::Position& pos): PosPredT(arg)
    {
        update(prev, token, pos);
    }

    bool valid() const {
        return lastPos_ != 0;
    }

    bool initiate(
            const tokext::Token& token,
            const tokext::Position& pos)
    {
        assert(pos.offset() + token.length() > pos.offset());
        if (!valid()) {
            lastPos_ = pos.offset() + token.length();
            return true;
        }

        auto last = pos.offset() + token.length();
        if (last < lastPos_) {
            lastPos_ = last;
            return true;
        }

        return false;
    }

    bool update(
            const FirstPositionRecord& prev,
            const tokext::Token& token,
            const tokext::Position& pos)
    {
        if ( !(pos.offset() > prev.lastPos_)) {
            // token must start on position strictly grater then last position
            // of previous record
            return false;
        }

        if ( !(PosPredT::check(pos.offset() - prev.lastPos_)) ) {
            // position predicate wasn't fulfilled
            return false;
        }

        return initiate(token, pos);
    }

    template<typename A, class P>
    friend std::ostream& operator<<(std::ostream& strm, const FirstPositionRecord<A, P>& record)
    {
        return strm << record.lastPos_;
    }

private:
    tokext::Position::idx_t lastPos_ = 0;
};

} /* hisi */


#endif /* end of include guard: FIRSTPOSITIONRECORD_HPP_CZKBSQ9G */
