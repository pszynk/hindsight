
#ifndef PARAM_CONFIG_HPP_YOMOQC0P
#define PARAM_CONFIG_HPP_YOMOQC0P

#include <utils/eoParser.h>
#include <utils/eoParam.h>

#include <string>

namespace
{
const static std::string NULL_DESC_ = "No description";
const static std::string NULL_SECT_ = "";
const static char NULL_SHORT_ = 0;
}

namespace hisi
{
    namespace make
    {

template<typename T>
class ParamPack
{
public:
    ParamPack(
            std::string longName,
            T defaultValue = {},
            std::string description = NULL_DESC_,
            char shortHand = NULL_SHORT_,
            std::string section = NULL_SECT_,
            bool required = false
    ):
        longName_(longName),
        defaultValue_(defaultValue),
        description_(description),
        shortHand_(shortHand),
        section_(section),
        required_(required)
    {}


    eoValueParam<T>& doGetORcreateParam(eoParser& parser) const
    {
        return parser.getORcreateParam(
                this->defaultValue_,
                this->longName_,
                this->description_,
                this->shortHand_,
                this->section_,
                this->required_);
    }

    eoValueParam<T> createParam() const {
        return eoValueParam<T>(
                this->defaultValue_,
                this->longName_,
                this->description_,
                this->shortHand_,
                this->required_);
    }

private:
    std::string longName_;
    T defaultValue_;
    std::string description_;
    char shortHand_;
    std::string section_;
    bool required_;

};


namespace make_param_
{
    template<typename T>
    using PPack = const ParamPack<T>;

    // MODE --------------------------------------------------
    PPack<unsigned> MODE_CODE {
        "mode",
        0,
        "Code of mode",
        NULL_SHORT_,
        "MODE",
        false
    };

    // EXTRACTION --------------------------------------------------

    PPack<double> MIN_TOKEN_LENGTH {
        "token_len",
        2,
        "Config desc",
        NULL_SHORT_,
        "CONFIG",
        false
    };

    PPack<int> MIN_TOKEN_OCC_NB {
        "token_occ_nb",
        3,
        "Config desc",
        NULL_SHORT_,
        "CONFIG",
        false
    };
    PPack<double> MIN_TOKEN_OCC_PCT {
        "token_occ_pct",
        0.15,
        "Config desc",
        NULL_SHORT_,
        "CONFIG",
        false
    };

    // DATA --------------------------------------------------
    PPack<std::string> CONFIG_FILE_NAME {
        "config",
        "",
        "Config desc",
        NULL_SHORT_,
        "CONFIG",
        true
    };

    PPack<std::string> SUSPICIOUS_FILE_NAME {
        "susp",
        "",
        "Suspicious desc",
        NULL_SHORT_,
        "DATA",
        true
    };

    PPack<std::string> NORMAL_FILE_NAME {
        "norm",
        "",
        "Normal desc",
        NULL_SHORT_,
        "DATA",
        true
    };



} /* make_param_ */


    } /* make */
} /* hisi */

#endif /* end of include guard: PARAM_CONFIG_HPP_YOMOQC0P */
