
#ifndef CONTENT_OP_HPP_VBF2JJMQ
#define CONTENT_OP_HPP_VBF2JJMQ

#include "Content.hpp"

#include <eoInit.h>
#include <utils/eoRNG.h>

namespace hisi
{

template<class ConT>
class ContentInit : public eoInit<ConT>
{
private:
    using TokenIdxT = typename ConT::TokenIdxT;
    using AttrT = typename ConT::AttrT;

public:
    ContentInit(
            eoInit<TokenIdxT>* tokenIdxInit,
            eoInit<AttrT>* attrInit):
        tokenIdxInit_(tokenIdxInit), attrInit_(attrInit)
    {}

    void operator()(ConT& content) override
    {
        tokenIdxInit_->operator()(content.tokenIdx());
        attrInit_->operator()(content.attr());
    }

private:
    eoInit<TokenIdxT>* tokenIdxInit_;
    eoInit<AttrT>* attrInit_;

};

template<class ConT>
class ContentAllMut : public eoMonOp<ConT>
{
private:
    using TokenIdxT = typename ConT::TokenIdxT;
    using AttrT = typename ConT::AttrT;

public:
    ContentAllMut(
            eoMonOp<TokenIdxT>* tokenIdxMut,
            eoMonOp<AttrT>* attrMut):
        tokenIdxMut_(tokenIdxMut), attrMut_(attrMut)
    {}

    bool operator()(ConT& content) override
    {
        return
            tokenIdxMut_->operator()(content.tokenIdx()) |
            attrMut_->operator()(content.attr());
    }

private:
    eoMonOp<TokenIdxT>* tokenIdxMut_;
    eoMonOp<AttrT>* attrMut_;
};

template<class ConT>
class ContentPropMut : public eoMonOp<ConT>
{
private:
    using TokenIdxT = typename ConT::TokenIdxT;
    using AttrT = typename ConT::AttrT;

public:
    ContentPropMut(
            eoMonOp<TokenIdxT>* tokenIdxMut,
            eoMonOp<AttrT>* attrMut, double rate = 0.5):
        tokenIdxMut_(tokenIdxMut), attrMut_(attrMut), rate_(rate)
    {}

    bool operator()(ConT& content) override
    {
        if(eo::rng.flip(rate_)) {
            return tokenIdxMut_->operator()(content.tokenIdx());
        }
        return attrMut_->operator()(content.attr());
    }

private:
    eoMonOp<TokenIdxT>* tokenIdxMut_;
    eoMonOp<AttrT>* attrMut_;
    double rate_;

};
} /* hisi */

#endif /* end of include guard: CONTENT_OP_HPP_VBF2JJMQ */
