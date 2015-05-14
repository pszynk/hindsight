
#ifndef CONTENT_HPP_WA0PAXYB
#define CONTENT_HPP_WA0PAXYB

#include "hisi/utils/printer.hpp"

#include <tokext/Token.hpp>

#include <eoObject.h>
#include <eoPersistent.h>

#include <map>

namespace hisi
{
    using DefaultTokenIndexType = tokext::Token::idx_t;

// T needs >> and <<
template<typename T, typename K = DefaultTokenIndexType>
class Content : public eoObject, public eoPersistent
{
public:
    using AttrT = T;
    using TokenIdxT = K;

    Content(TokenIdxT tokenIdx = 0, AttrT attr = {}):tokenIdx_(tokenIdx), attr_(attr)
    {}

    // token idx
    const TokenIdxT& tokenIdx() const {return tokenIdx_;}
    TokenIdxT& tokenIdx() {return tokenIdx_;}
    explicit operator const TokenIdxT&() const {return tokenIdx_;}

    // attr
    const AttrT& attr() const {return attr_;}
    AttrT& attr() {return attr_;}
    explicit operator const AttrT&() const {return attr_;}


    // OVERRIDE
    std::string className() const override { return "Content"; }

    void printOn(std::ostream& os) const override;
    void readFrom(std::istream& is) override;

    bool operator== (const Content<T, K>& other) {
        return this->tokenIdx() == other.tokenIdx() &&
            this->attr() == other.attr();
    }

private:
    TokenIdxT tokenIdx_ = 0;
    AttrT attr_;
};

template<typename T, typename K>
void Content<T, K>::printOn(std::ostream& os) const
{
    using utils::operator<<;
    os << tokenIdx() << ' ' << '|' << ' ';
    os << attr();
}

template<typename T, typename K>
void Content<T, K>::readFrom(std::istream& is)
{
    using utils::operator>>;
    TokenIdxT tx;
    char trash;
    is >> tx >> trash;
    tokenIdx() = tx;
    is >> attr();
}

template<typename T, typename K>
std::ostream & operator<<(std::ostream& os, const Content<T, K>& content)
{
    content.printOn(os);
    return os;
}

} /* hisi */

#endif /* end of include guard: CONTENT_HPP_WA0PAXYB */
