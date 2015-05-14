
#include "make_data.hpp"

// names, desc etc..
#include "param_config.hpp"
#include "InputParamError.hpp"

// EO INCLUDES
// parser and param
#include <utils/eoState.h>

// STD
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

// stat function
#include <sys/stat.h>

#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>



using namespace log4cxx;
using namespace log4cxx::helpers;

static LoggerPtr logger(Logger::getLogger("hisi::make::make_data"));

using namespace std;

static const string COMMENT_CHARS = "#";

// quick way to check if sth of this filename exists
bool exists(const string& filename)
{
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) == 0) {
        if (S_ISREG(buffer.st_mode)) {
            return true;
        }
    };
    return false;
}


bool skip_line(const string& line) {
    if(line.empty()) return true;
    if(COMMENT_CHARS.find(line.at(0)) != string::npos) return true;
    return false;
}

typedef pair<string, unsigned> record_t;
typedef vector<record_t> file_list_t;

// read file with file names on each line (possible comments)
file_list_t readFileList(const string& filename)
{
    file_list_t files;

    ifstream ifs(filename);

    if(!ifs) {
        stringstream ss;
        ss << "Error opening LIST FILE `"<< filename <<"'";
        throw runtime_error(ss.str());
    }

    unsigned counter = 0;
    string line;

    while(getline(ifs, line)) {
        ++counter;

        if(skip_line(line)) continue;

        if(!exists(line)) {
            stringstream ss;
            ss << "Error reading LIST FILE: " << filename << endl
                << "\tLine["<<counter<<"] file path: `"<< line
                <<"' does not exists or is not a proper file!";
            throw runtime_error(ss.str());
        }

        files.push_back(make_pair(move(line), counter));
    }

    return move(files);
}

using buffer_t_ = tokext::InputData::buffer;

buffer_t_ readBinary(const string& parent, const record_t& record)
{
    // open the file:
    std::streampos fileSize;

    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {

        ifs.open(record.first, std::ios::binary);

        // get its size:
        ifs.seekg(0, std::ios::end);
        fileSize = ifs.tellg();
        assert(!ifs.bad());

        ifs.seekg(0, std::ios::beg);

        // read the data:
        buffer_t_ fileData(fileSize);
        ifs.read((char*) &fileData[0], fileSize);
        ifs.close();
        return move(fileData);
    }
    catch(std::ifstream::failure e) {
        stringstream ss;
        ss << "Error opening/reading BINARY FILE `"<<record.first<< endl
            << "\tfile path read from line["<<record.second<<"] of LIST FILE `"
            << parent <<"'"<< endl
            << "\terror: " << e.what();
        throw runtime_error(ss.str());
    }
    catch(std::bad_alloc e) {
        stringstream ss;
        ss << "Error reading BINARY FILE `"<<record.first<< endl
            << "\tfile path read from line["<<record.second<<"] of LIST FILE `"
            << parent <<"'"<< endl
            << "\tpossibly file is to big?, error: " << e.what();
        throw runtime_error(ss.str());
    }
}

vector<buffer_t_> readFlows(const string& filename)
{
    auto fileList = readFileList(filename);
    vector<buffer_t_> flows;

    for(auto& r : fileList) {
        flows.push_back(readBinary(filename, r));
    }

    return flows;
}


namespace hisi
{
    namespace make
    {

tokext::InputData make_data(eoParser& parser, unsigned& maxSuspLen)
{
    using namespace make_param_;

    tokext::InputData idata;
    string suspFileName =
        SUSPICIOUS_FILE_NAME.doGetORcreateParam(parser).value();

    if (!suspFileName.empty()) {
        LOG4CXX_DEBUG(logger, "reading suspicious file list...");
        auto susp = readFlows(suspFileName);
        LOG4CXX_DEBUG(logger, "list contains " << susp.size() << " files");

        idata.insertSuspiciousData(
                make_move_iterator(susp.begin()),
                make_move_iterator(susp.end()));
    } else {
        throw InputParamError("No suspicious file given");
    }

    string normFileName =
        NORMAL_FILE_NAME.doGetORcreateParam(parser).value();

    if (!normFileName.empty()) {
        auto norm = readFlows(normFileName);

        idata.insertNormalData(
                make_move_iterator(norm.begin()),
                make_move_iterator(norm.end()));
    } else {
        std::cerr << "Warning! no normal flow loaded" << std::endl;
    }

    if (idata.empty()) {
        throw InputParamError("No data loaded");
    }

    for(auto it = idata.beginSuspicious(); it != idata.endSuspicious(); ++it) {
        maxSuspLen = std::max(maxSuspLen, unsigned(it->size()));
    }
    return idata;
}


    } /* make */
} /* hisi */
