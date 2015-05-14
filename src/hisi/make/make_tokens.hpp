
#ifndef MAKE_TOKENS_HPP_T6QEAFFI
#define MAKE_TOKENS_HPP_T6QEAFFI

#include "param_config.hpp"
#include "InputParamError.hpp"

#include <tokext/SuffixContext.hpp>
#include <tokext/utils/extraction.hpp>
#include <tokext/ExtractionParams.hpp>

#include <utils/eoParser.h>

#include <sstream>

namespace hisi
{
    namespace make
    {

template<typename T>
T make_tokens(eoParser& parser, const tokext::SuffixContext& scx)
{
    using namespace make_param_;
    int token_len =
        MIN_TOKEN_LENGTH.doGetORcreateParam(parser).value();
    if (token_len <= 0) {
        throw InputParamError("token length must be > 0");
    }

    int token_occ_nb =
        MIN_TOKEN_OCC_NB.doGetORcreateParam(parser).value();

    auto token_occ_pct_PRM = MIN_TOKEN_OCC_PCT.doGetORcreateParam(parser);
    //auto token_occ_pct_PRM = MIN_TOKEN_OCC_PCT.createParam();

    unsigned occ;

    if (parser.isItThere(token_occ_pct_PRM)) {
        double pct = token_occ_pct_PRM.value();
        std::cout << "Using percentage token occurance: " << pct << std::endl;
        occ = pct * scx.suspiciousFlowCount();
    } else {
        if (token_occ_nb < 2) {
            throw InputParamError("token occurrence must be > 2");
        }
        if (unsigned(token_occ_nb) > scx.suspiciousFlowCount()) {
            std::stringstream ss;
            ss << "token occurrence must be smaller then suspicious flows cout: " << scx.suspiciousFlowCount();
            throw InputParamError(ss.str());
        }
        occ = token_occ_nb;
    }
    std::cout << "Extracting tokens with params:" << std::endl <<
        "\tmin lenth: " << token_len << std::endl <<
        "\tmin occ  : " << occ << std::endl;

    tokext::ExtractionParams params(token_len, occ);
    T tokens = tokext::utils::extractTokens(scx, params);

    std::cout << "extracted: " << tokens.size() << " tokens" << std::endl;
    if(tokens.size() == 0) {
        throw InputParamError("extracted ZERO tokens!");
    }

    return std::move(tokens);

}

    } /* make */
} /* hisi */

#endif /* end of include guard: MAKE_TOKENS_HPP_T6QEAFFI */
