
#ifndef HINDISGHT_HPP_YIUA0RPY
#define HINDISGHT_HPP_YIUA0RPY

#include "make/make_data.hpp"
#include "eval/tools/TokenPool.hpp"

#include <tokext/SuffixContext.hpp>
#include <tokext/ExtractionParams.hpp>
#include <tokext/utils/extraction.hpp>
#include <tokext/utils/translation.hpp>

#include <prettyprint/prettyprint.h>

#include <iostream>
#include <algorithm>

namespace hisi
{

struct comp_token_len {
    bool operator() (const tokext::Token& t1, const tokext::Token& t2)
    {
        //return (t1.occurrence().inSuspicious()) > (t2.occurrence().inSuspicious());
        return t1.length() > t2.length();
    }
};

void start(int argc, char* argv[])
{
    using namespace std;
    using namespace tokext;
    eoParser parser(argc, argv);
    SuffixContext scx(make_data(parser));
    //cout << idata << endl;
    if(parser.userNeedsHelp()) {
        parser.printHelp(std::cout);
        return;
    }

    cout << "read " << scx.suspiciousFlowCount() << " suspicious" << endl;
    cout << "read " << scx.normalFlowCount() << " normal" << endl;
    cout << "read " << scx.totalFlowCount() << " total" << endl;


    ExtractionParams params(2, 2);
    //match::tools::TokenPool<unsigned> tokens = utils::extractTokens(scx, params);
    //sort(tokens.begin(), tokens.end(), comp_token_len());

    //for(int i = 0; i < 5; ++i) {
    //    cout << tokext::utils::translateToken<string>(scx, tokens.token(i))<< endl;
    //    cout << tokens.token(i) << endl;
    //}
}


} /* hisi */

#endif /* end of include guard: HINDISGHT_HPP_YIUA0RPY */
