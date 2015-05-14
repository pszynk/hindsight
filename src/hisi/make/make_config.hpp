
#ifndef MAKE_CONFIG_HPP_ZR48IBUT
#define MAKE_CONFIG_HPP_ZR48IBUT

#include "param_config.hpp"
#include "InputParamError.hpp"

#include <utils/eoParser.h>

#include <libconfig.h++>

#include <sstream>

namespace hisi
{
    namespace make
    {

void make_config(eoParser& parser, libconfig::Config& cfg)
{
    std::string config =
        make_param_::CONFIG_FILE_NAME.doGetORcreateParam(parser).value();

    using namespace libconfig;
    try {
        cfg.readFile(config.c_str());
    }
    catch(const FileIOException &fioex) {
        std::stringstream ss;
        ss << "config file: I/O error while reading file: `" << config << "`";
        throw InputParamError(ss.str());
    }
    catch(const ParseException &pex) {
        std::stringstream ss;
        ss << "Parse error at " << pex.getFile() << ":" << pex.getLine()
            << " - " << pex.getError();
        throw InputParamError(ss.str());
    }
}


    } /* make */
} /* hisi */

#endif /* end of include guard: MAKE_CONFIG_HPP_ZR48IBUT */
