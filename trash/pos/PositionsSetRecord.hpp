
#ifndef POSITIONSSETRECORD_HPP_V5SZFHQI
#define POSITIONSSETRECORD_HPP_V5SZFHQI

#include "../position_predicates.hpp"

#include <tokext/Token.hpp>
#include <tokext/Position.hpp>

#include <prettyprint/prettyprint.h>

#include <set>

namespace hisi
{

template<typename PredArgT, class PosPredT = NoPredicate<PredArgT>>
class PositionsSetRecord : PosPredT
{
public:
    PositionsSetRecord(
            const PredArgT& arg,
            const tokext::Token& token,
            const tokext::Position& pos): PosPredT(arg)
    {
        initiate(token, pos);
    }

    PositionsSetRecord(
            const PredArgT& arg,
            const PositionsSetRecord& prev,
            const tokext::Token& token,
            const tokext::Position& pos): PosPredT(arg)
    {
        update(prev, token, pos);
    }


    bool valid() const {
        return !posits_.empty();
    }

    bool initiate(
            const tokext::Token& token,
            const tokext::Position& pos)
    {
        auto newPos = pos.offset() + token.length();
        assert(newPos > pos.offset());
        // assume positions won't repeat
        auto pr = posits_.insert(newPos);
        assert(pr.second);
        return true;
    }

    bool update(
            const PositionsSetRecord& prev,
            const tokext::Token& token,
            const tokext::Position& pos)
    {
        for (auto prevPos : prev.posits_) {
            // token must start on position strictly grater then positions
            // of previous record
            if (!(pos.offset() > prevPos)) {return false;} // this one is greater
                                                    // then other ones are too

            // position predicate is fulfilled
            if ( (PosPredT::check(pos.offset() - prevPos)) ) {
                return initiate(token, pos);
            }
        }
        return false;
    }

    template<typename A, class P>
    friend std::ostream& operator<<(std::ostream& strm, const PositionsSetRecord<A, P>& record)
    {
        return strm << record.posits_;
    }

private:
    std::set<tokext::Position::idx_t> posits_;
};

} /* hisi */

#endif /* end of include guard: POSITIONSSETRECORD_HPP_V5SZFHQI */
