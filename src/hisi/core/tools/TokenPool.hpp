
#ifndef TOKENPOOL_HPP_IU2DV4EH
#define TOKENPOOL_HPP_IU2DV4EH

#include <tokext/Token.hpp>

//#include <prettyprint/prettyprint.h>

#include <vector>

namespace hisi
{

template<typename T>
struct SimpleCastTranslator
{
    static const tokext::Token::idx_t& translate(const T& t) {
        return static_cast<const tokext::Token::idx_t&>(t);
    }
};

template<
    typename KeyT_,
    class TranslatorT_ = SimpleCastTranslator<KeyT_>,
    class ContainerT_ = std::vector<tokext::Token>>
class TokenPool : ContainerT_
{
public:

    using KeyType = KeyT_;
    using ContainerType = ContainerT_;
    using TranslatorType = TranslatorT_;


    using iterator = typename ContainerType::iterator;
    using const_iterator = typename ContainerType::const_iterator;

    using ContainerType::begin;
    using ContainerType::end;

    using ContainerType::cbegin;
    using ContainerType::cend;

    using ContainerType::size;

    TokenPool(const ContainerType& ctnr):ContainerType(ctnr){}
    TokenPool(ContainerType&& ctnr = ContainerType()):ContainerType(std::move(ctnr)){}

    const tokext::Token& operator[](const KeyType& key) const {
        assert(size() > TranslatorType::translate(key));
        return ContainerType::operator[](TranslatorType::translate(key));
    }

    //const tokext::Token token(const KeyT& key) const
    //{
    //    return ContainerType::operator[](TranslatorType::translate(key));
    //}

};

template<typename K, class T, class C>
std::ostream& operator<<(std::ostream& strm, const TokenPool<K, T, C>& tp)
{
    strm << "TokenPool {" << std::endl;
    for(auto& t : tp) {
        strm << t << std::endl;
    }
    return strm << "}";
}

} /* hisi */

#endif /* end of include guard: TOKENPOOL_HPP_IU2DV4EH */
