
//#include "hisi/core/gen/integer_op.hpp"
//#include "hisi/core/gen/tuple_op.hpp"
//#include "hisi/core/gen/null_op.hpp"
//#include "hisi/core/gen/array_op.hpp"
//#include "hisi/core/gen/content_op.hpp"
//#include "hisi/core/gen/content_op.hpp"
//#include "hisi/core/gen/Content.hpp"
//#include "hisi/core/gen/genetics.hpp"
//#include "hisi/core/pos/PositionRuleFunc.hpp"
//#include "hisi/core/pos/position_rules.hpp"
//#include "hisi/core/match/GlobalMatchAlgo.hpp"


//#include <prettyprint/prettyprint.h>

//#include <eoPop.h>
//#include <utils/eoState.h>
//#include <eoVector.h>
//#include <eoInit.h>

//#include <iostream>
//#include <tuple>
//#include <type_traits>
//#include <utility>
//#include <sstream>

#include "hisi/start.hpp"

//#include "tokext/InputBuffer.hpp"
//#include "tokext/InputData.hpp"
//#include "tokext/SuffixContext.hpp"
//#include "tokext/utils/extraction.hpp"
//#include "tokext/utils/translation.hpp"
//#include "tokext/ExtractionParams.hpp"

//#include "hisi/core/pos/PositionRuleFunc.hpp"
//#include "hisi/core/tools/AcceptAllFlowFilter.hpp"
//using namespace tokext;
//using namespace tokext::utils;
//using namespace std;

using namespace hisi;

//void test_array_op()
//{
//    //test Init
//    array<int, 3> arr_eo_1 {{0, 0, 0}};
//    using IIT = IntegerInitUniform<int>;
//    IIT init_1(-10, 0);
//    IIT init_2(0, 10);
//    IIT*  init_3 = new IIT(10, 20);
//    ArrayInit<decltype(arr_eo_1)> arr_init_1{{{&init_1, &init_2, init_3}}};

//    unsigned times = 10;
//    for(unsigned i = 0; i<times; ++i) {
//        arr_init_1(arr_eo_1);
//        cout << i << "] " << arr_eo_1 << endl;
//    }

//    arr_init_1 = {{{init_3, nullptr, &init_1}}};
//    cout << "\n\n";

//    for(unsigned i = 0; i<times; ++i) {
//        arr_init_1(arr_eo_1);
//        cout << i << "] " << arr_eo_1 << endl;
//    }

//    // test Mut
//    cout << "Mut" << endl;

//    arr_eo_1 = {{-10, 0, 10}};
//    IntegerMutConstantWrap<int> mut_1{-10, 0, {-1}};
//    IntegerMutUniformWrap<int>* mut_2 = new IntegerMutUniformWrap<int>{0, 11, {-1, 3}};
//    ArrayMutAll<decltype(arr_eo_1)> arr_mut_1{{{&mut_1, nullptr, mut_2}}};

//    for(unsigned i = 0; i<times; ++i) {
//        arr_mut_1(arr_eo_1);
//        cout << i << "] " << arr_eo_1 << endl;
//    }

//    cout << "\n\n";
//    arr_eo_1 = {{0, 0, 0}};
//    IntegerMutConstantWrap<int> mut_3{0, 10, {1}};
//    //array<pair<eoMonOp<int>*, double>, 3> roul{{{&mut_3, 0.5}, {&mut_3, 0.0}, {&mut_3, 0.5}}};
//    ArrayMutRoulette<decltype(arr_eo_1)> arr_mut_2{{{{&mut_3, 0.5}, {&mut_3, 0.0}, {&mut_3, 0.5}}}};

//    for(unsigned i = 0; i<times; ++i) {
//        arr_mut_2(arr_eo_1);
//        cout << i << "] " << arr_eo_1 << endl;
//    }
//}

//void test_tuple_op()
//{
//    //test Init
//    tuple<int, unsigned, bool> tup_eo_1 {0, 0, 0};
//    IntegerInitUniform<int> init_1(-10, 0);
//    IntegerInitUniform<unsigned> init_2(0, 10);
//    eoUniformInit<bool>*  init_3 = new eoUniformInit<bool>();
//    TupleInit<decltype(tup_eo_1)> tup_init_1{&init_1, &init_2, init_3};

//    unsigned times = 10;
//    for(unsigned i = 0; i<times; ++i) {
//        tup_init_1(tup_eo_1);
//        cout << i << "] " << tup_eo_1 << endl;
//    }
//    tup_init_1 = {&init_1, nullptr, init_3};

//    cout << "\n\n";
//    for(unsigned i = 0; i<times; ++i) {
//        tup_init_1(tup_eo_1);
//        cout << i << "] " << tup_eo_1 << endl;
//    }

//    IntegerMutConstantWrap<int> mut_1{-10, 0, {-1}};
//    IntegerMutUnsafeUniformWrap<unsigned>* mut_2 = new IntegerMutUnsafeUniformWrap<unsigned>{0, 11, {unsigned(-1), 3}};
//    BoolMutFlip mut_3;

//    TupleMut<decltype(tup_eo_1)> tup_mut_1{nullptr, mut_2, &mut_3};

//    cout << "\n\n";
//    for(unsigned i = 0; i<times; ++i) {
//        tup_mut_1(tup_eo_1);
//        cout << i << "] " << tup_eo_1 << endl;
//    }

//    // tuple + array

//    tuple<bool, array<int, 2>, unsigned> tup_arr{0, {{0, 0}}, 0};
//    //ArrayInit<array<int, 2>> arr_init {{{&init_1, &init_1}}};
//    ArrayInit<array<int, 2>> arr_init {{{nullptr, &init_1}}};
//    TupleInit<decltype(tup_arr)> tup_arr_init{init_3, &arr_init, &init_2};

//    cout << "\n\n";
//    for(unsigned i = 0; i<times; ++i) {
//        tup_arr_init(tup_arr);
//        cout << i << "] " << tup_arr << endl;
//    }


//    BoolMutFlip mut_bool;
//    tuple<unsigned, array<int, 2>, bool> tup_arr_bl{0, {{0, 0}}, 0};
//    ArrayMutAll<array<int, 2>> arr_mut {{{&mut_1, &mut_1}}};
//    TupleMut<decltype(tup_arr_bl)> tup_arr_mut{mut_2, &arr_mut, &mut_bool};
//    //arr_init_1 = {{{init_3, nullptr, &init_1}}};
//    cout << "\n\n";

//    for(unsigned i = 0; i<times; ++i) {
//        tup_arr_mut(tup_arr_bl);
//        cout << i << "] " << tup_arr_bl << endl;
//    }
//}


//void test_pop_io()
//{
//    using ArrT = array<int, 2>;
//    using TupT = tuple<bool, ArrT>;
//    using ConT = Content<TupT>;
//    using Indi = eoVector<double, ConT>;
//    using Pop = eoPop<Indi>;

//    IntegerInitUniform<int> int_i(-10, 0);
//    IntegerInitUniform<typename ConT::TokenIdxT> uns_i(0, 10);
//    eoUniformInit<bool> bool_i;

//    ArrayInit<ArrT> arr_i{{{&int_i, &int_i}}};
//    TupleInit<TupT> tup_i{&bool_i, &arr_i};
//    ContentInit<ConT> con_i{&uns_i, &tup_i};
//    VectorInitVariableLength<Indi> indi_i{3, 5, con_i};

//    Pop pop(10, indi_i);

//    pop.printOn(cout);

//    stringstream ss, dd;
//    eoState state("stateName");
//    state.registerObject(pop);
//    state.save("temp");
//    cout << ss.str() << endl << endl;
//    eoState ll("otherState");
//    Pop pop1;
//    ll.registerObject(pop1);
//    ll.load("temp");
//    pop1.printOn(cout);

//}

//void test_global_algo()
//{
    //InputData idata = makeInputData<vector<string>>({
             //"jedendwatrzy()",
            //"+jedendwa=8trzy12345",
            //"asdfas"}, {});

    //InputData idata = makeInputData<vector<string>>({
    //         "jeden  dwa_ trzy()",
    //        "+jeden--dwa=8trzy12345",
    //        "asdfas"}, {});
    //InputData idata = makeInputData<vector<string>>({
    //         "abcba_xyzyx_xyabc",
    //         "abcbaabcxycbazyxy",
    //        //0123456789012345678901234
    //         "abcba_cba_xy_zyx_xy_abcxy",
    //         "abcba_cba_xy_zyx_abcxyxy",
    //         "cba  xy",
    //         "xy         cba",
    //         "    cba      xy cba",
    //         "    cba      xy cba   xy ",
    //        }, {});
    //SuffixContext scx(idata);
    //ExtractionParams params(2, 2);
    //auto it = findToken(scx, string("def"), tokens.begin(), tokens.end());

    //using ContT = Content<pos::dual_t>;
    //using ContT = unsigned;
    //TokenPool<ContT> tokens = extractTokens(scx, params);
    //auto it = findToken(scx, string("jeden"), tokens.begin(), tokens.end());
    //auto jeden = std::distance(tokens.begin(), it);
    //it = findToken(scx, string("dwa"), tokens.begin(), tokens.end());
    //auto dwa = std::distance(tokens.begin(), it);
    //it = findToken(scx, string("trzy"), tokens.begin(), tokens.end());
    //auto trzy = std::distance(tokens.begin(), it);

    //auto it = findToken(scx, string("abcba"), tokens.begin(), tokens.end());
    //unsigned abcba = std::distance(tokens.begin(), it);
    //it = findToken(scx, string("cba"), tokens.begin(), tokens.end());
    //unsigned cba = std::distance(tokens.begin(), it);
    //it = findToken(scx, string("xy"), tokens.begin(), tokens.end());
    //unsigned xy = std::distance(tokens.begin(), it);
    //it = findToken(scx, string("zyx"), tokens.begin(), tokens.end());
    //unsigned zyx = std::distance(tokens.begin(), it);
    //it = findToken(scx, string("abc"), tokens.begin(), tokens.end());
    //unsigned abc = std::distance(tokens.begin(), it);
    //it = findToken(scx, string("abcxy"), tokens.begin(), tokens.end());
    //unsigned abcxy = std::distance(tokens.begin(), it);

    //NullRule<unsigned> prf;
    //PositionRuleFunc<pos::DistanceWithinRule> prf;
    //GlobalMatchAlgo<decltype(tokens), decltype(prf)> algo(tokens);
    //algo.context(&scx);

    //std::vector<ContT> vec = {{xy, 0}, {abc, 0}, {abcba, 0}, {xy, 0}, {zyx, 0}, {cba, 0}};
    //std::vector<ContT> vec = {abcba,  cba,  xy,  zyx,  xy,  abc};
    //std::vector<ContT> vec = {{abcba, 0}, {cba, 2}, {xy, 1}, {zyx, 1}, {xy, 4}, {abc, 2}};
    //std::vector<ContT> vec = {{cba, {2,14}}, {xy, {-6, 2}}};
    //std::vector<long> vec = {dwa, jeden, trzy};
    //cout << vec << endl;
    //auto match = algo.detailed(vec.begin(), vec.end());
    //cout << match << endl;
    //vec = {{abcba, 0}, {cba, 0}, {xy, 0}, {zyx, 0}, {xy, 0}, {abc, 0}};
    //match = algo.detailed(vec.begin(), vec.end());
    //cout << match << endl;

    //vec = {dwa, jeden, trzy};
    //cout << vec << endl;
    //match = algo.match(vec.begin(), vec.end());
    //cout << match << endl;

    //vec = {trzy, dwa, jeden};
    //cout << vec << endl;
    //match = algo.match(vec.begin(), vec.end());
    //cout << match << endl;
//}

int main(int argc, char* argv[])
{
    //test_array_op();
    //test_tuple_op();
    //test_pop_io();
    //test_global_algo();
    hisi::start(argc, argv);



    //using comp_tuple = tuple<array<int, 2>, tuple<bool, double>>;
    //comp_tuple tt {{{1,2}},tuple<bool, double>{true,1.1}};
    //tuple<array<int, 2>, array<int, 2>> ta {{{1,2}},{3,4}};
    //array<array<int, 2>, 2> aa {{1,2,  3,4}};
    ////cout << aa << endl;
    ////cout << explode(func, tt);
    ////cout << explode(func1, arr);
    //using ttype = tuple<int, double, char>;
    //using tttype = tuple<tuple<int, int>, double>;
    //stringstream ss;
    //ss << 2.1 << ' ' << 1 << ' ' << 3;
    //ss << 1 << ' ' << 2.1 << ' ' << 'z';
    //auto tup = parse<ttype>(ss);
    //auto tup = parse<int, double>(ss);
    //auto tup = parse<decltype(ta)>(ss);
    //cout << tup << endl;
    //cout << i << endl;
    //cout << ta << endl;
    //ttype z {1, 2.1, 'z'};
    //cout << z<<endl;
    //ttype q;
    //ss >> q;
    //cout << q << endl;
    //ss << tt;
    //cout << ss.str() << endl;
    //comp_tuple nt;
    ////ss >> nt;
    //cout << nt << endl;
    //hisi::func();
    //cout << tt << endl;


    return 0;
}

//#include "hisi/core/gen/pc/PCFactory.hpp"
//#include "hisi/core/gen/pc/PCChecker.hpp"
//#include "hisi/core/gen/Content.hpp"

//template<typename T>
//struct callable
//{
//    virtual void operator ()(T&)=0;
//};

    //addOne a1;
    //addTwo a2;
    ////tuple<callable<int>*, callable<double>*> tf {&a1, &a2};
    //tuple<callable<int>*, callable<double>*> tf {&a1, &a2};
    //tuple<int, double> tv {1, 1};
    ////get<0>(tf)->operator()(get<0>(tv));
    ////safe_apply(get<0>(tf), get<0>(tv));
    //Apply<safe_apply>::apply(tf, tv);
    //cout << tv << endl;

//struct addOne : callable<int>
//{
//    void operator ()(int& i) {i+=1;}
//};

//struct addTwo : callable<double>
//{
//    void operator ()(double& i) {i+=0.2;}
//};

//struct safe_apply {
//    template<typename T, typename U>
//    static void call(T& t, U& u) {
//        if(t)
//            t->operator()(u);
//    }
//};

//using namespace hisi;
    //vector<PCCodes> codes = {PCCodes::DEPTH, PCCodes::OFFSET};
    //auto z = PCFactory::createFuncMap(codes.begin(), codes.end());
    //using CT = Content<PCFactory::ArgMap>;
    //PCChecker<CT> checker(z);
    //PCMap<int> am {{PCCodes::DEPTH, 5}, {PCCodes::OFFSET, 10}};
    //using CPC = Content<PCFactory::ArgMap>;
    //CPC c1, c2, c3;
    //IntegerInitUniform<Content::idx_t> ti(0, 10);
    //IntegerInitUniform<int> c1i(-10, 20);
    //IntegerInitUniform<int> c2i(-100, -10);
    //IntegerInitUniform<int> c3i(20, 110);
    //IndivInitIntegerDict<ContentAttars, int> cai(ContentAttars::DEPTH, c1i);
    //cai.add(ContentAttars::OFFSET, c2i); cai.add(ContentAttars::WITHIN, c3i);
    //ContentInit cti(ti, cai);
    //cti(c1);
    //cti(c2);
    //cout << c1 << endl;
    //cout << c2 << endl;
    //stringstream ss;
    //ss << c1;
    //ss >> c3;
    //cout << ss.str() << endl;
    //cout << c3 << endl;


    //auto ts = hisi::utils::constant::Value<Content::idx_t>(2);
    //IntegerMutConstantWrap<Content::idx_t> tim(0, 10,ts );
    //auto c1s = hisi::utils::constant::Value<int>(-7);
    //IntegerMutConstantWrap<int> c1m(-10, 20, c1s );
    //IndivMutIntegerDict<ContentAttars, int> cam(ContentAttars::DEPTH, c1m);
    //ContentMut ctm(tim, cam);
    //for(auto i = 0; i<10; ++i) {
    //   ctm(c1);
    //    cout << c1 << endl;
    //}

