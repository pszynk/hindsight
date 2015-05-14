

#include "test.hpp" // Brings in the Catch framework

//#include "hisi/core/pos/position_rules.hpp"

#include <tuple>
#include <vector>

using namespace std;
//using namespace hisi::pos;

TEST_CASE( "Rules are checked", "[rules][pos]" ) {

    SECTION("SingleRule checking", "")
    {
        SECTION("Offset checking", "")
        {

            // point | start | len | val
            // ^ doent matter  ^
            //int x = 1;
            //vector<tuple<int, int, int, single_t>>
                //good_tests  = {
                    //make_tuple(x , 0 , x,  0) ,
                    //make_tuple(x , 10 , x, 11) ,
                    //make_tuple(x    , 20 , x, 3)
                //};

        }
        //REQUIRE( 0 == 1 );
        //TODO: dokonczyc [(13/02/15 14:45:49) - pawel]
    }
}
