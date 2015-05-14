#include "hisi/eval/tools/TokenPool.hpp"
#include "hisi/eval/tools/AcceptAllFlowFilter.hpp"
//#include "hisi/eval/conv/SSPNMatchConverter.hpp"
//#include "hisi/eval/StateMatcher.hpp"
//#include "hisi/utils/constant.hpp"
//#include "hisi/eval/pos/FirstPositionRecord.hpp"
//#include "hisi/eval/pos/PositionsSetRecord.hpp"
//#include "hisi/match/pos/PositionStateFactory.hpp"
//#include "hisi/match/StateMatcher.hpp"
#include "hisi/eval/MatchEvaluator.hpp"

#include <tokext/utils/translation.hpp>
#include <tokext/InputBuffer.hpp>
#include <tokext/InputData.hpp>
#include <tokext/SuffixContext.hpp>
#include <tokext/utils/extraction.hpp>
#include <tokext/utils/translation.hpp>
#include <tokext/ExtractionParams.hpp>
#include <tokext/TokenUtility.hpp>

#include <prettyprint/prettyprint.h>

#include <eo>
#include <es.h>
#include <eoInt.h>
#include <eoVariableLengthCrossover.h>
#include <eoVariableLengthMutation.h>
#include <eoShiftMutation.h>
#include <eoFlOrMonOp.h>

#include <stdexcept>
#include <iostream>

using namespace tokext;
using namespace std;


// REPRESENTATION
//-----------------------------------------------------------------------------
// define your individuals
typedef eoVector<double, unsigned> Indi;     // A bitstring with fitness double

template <class T>
class CyclicShoveMutation : public eoMonOp<T>
{
public:
    CyclicShoveMutation(T value, T lb, T ub): value_(value), lb_(lb), ub_(ub)
    {}

    bool operator()(T & eo) override
    {
        if (rng.flip()) {
            // add
            eo = (eo + value_) % ub_;
        } else {
            if (eo < value_) {
                eo = ub_ - (value_ - eo);
            } else {
                eo -= value_;
            }
        }
        return true;
    }

private:
    T value_;
    T lb_;
    T ub_;
};

// EVAL
//-----------------------------------------------------------------------------

// GENERAL
//-----------------------------------------------------------------------------
#include "hisi/make/make_data.hpp"
//void main_function(int argc, char **argv)
//{
//    using namespace hisi;
//    eoParser parser(argc, argv);
//    tokext::InputData idata = make_data(parser);
//    //cout << idata << endl;
//    if(parser.userNeedsHelp()) {
//        parser.printHelp(std::cout);
//    }
//    SuffixContext scx(idata);
//    ExtractionParams params(2, 2);

//    TokenPool<unsigned> tokens = utils::extractTokens(scx, params);
//    //cout << tokens << endl;

//    AcceptAllFlowFilter filter(scx.suspiciousFlowCount(), scx.normalFlowCount());
//    //cout << filter.acceptedNormal() << endl;

//    auto sm = create_PositionMatcher<unsigned, FirstPositionRecord<unsigned>>
//        (scx, tokens, filter);
//    //cout << sizeof(sm) << endl;

//    auto eval = create_MatchEvaluator<Indi>(sm, SSPNMatchConverter());


//    // PARAMETRES
//    // all parameters are hard-coded!
//    const unsigned int SEED = 0;      // seed for random number generator
//    const unsigned int T_SIZE = 13;     // size for tournament selection
//    const unsigned int MIN_VEC_SIZE = 4;   // Number of bits in genotypes
//    const unsigned int MAX_VEC_SIZE = 5;   // Number of bits in genotypes
//    const unsigned int POP_SIZE = 1000;  // Size of population
//    const unsigned int MAX_GEN = 4000;  // Maximum number of generation before STOP
//    const float CROSS_RATE = 0.8;      // Crossover rate
//    const double P_MUT_PER_BIT = 0.01; // probability of bit-flip mutation
//    const float MUT_RATE = 1.0;        // mutation rate

//    // GENERAL
//    //////////////////////////
//    //  Random seed
//    //////////////////////////
//    //reproducible random seed: if you don't change SEED above,
//    // you'll aways get the same result, NOT a random run
//    rng.reseed(SEED);

//    // EVAL
//    /////////////////////////////
//    // Fitness function
//    ////////////////////////////
//    // Evaluation: from a plain C++ fn to an EvalFunc Object
//    //eval;

//    // INIT
//    ////////////////////////////////
//    // Initilisation of population
//    ////////////////////////////////

//    // declare the population
//    eoPop<Indi> pop;
//    // fill it!
//    eoUniformInit<Indi::AtomType> atomInit(tokens.size());
//    eoInitVariableLength<Indi> init(MIN_VEC_SIZE, MAX_VEC_SIZE, atomInit);
//    for (unsigned int igeno=0; igeno<POP_SIZE; igeno++)
//    {
//        Indi v;           // void individual, to be filled
//        init(v);
//        eval(v);                 // evaluate it
//        pop.push_back(v);        // and put it in the population
//    }

//    // OUTPUT
//    // sort pop before printing it!
//    pop.sort();
//    // Print (sorted) intial population (raw printout)
//    cout << "Initial Population" << endl;
//    //cout << pop;
//    // shuffle  - this is a test
//    pop.shuffle();
//    // Print (sorted) intial population (raw printout)
//    cout << "Shuffled Population" << endl;
//    //cout << pop;

//    // ENGINE
//    /////////////////////////////////////
//    // selection and replacement
//    ////////////////////////////////////
//    // SELECT
//    // The robust tournament selection
//    eoDetTournamentSelect<Indi> select(T_SIZE);  // T_SIZE in [2,POP_SIZE]

//    // REPLACE
//    // The simple GA evolution engine uses generational replacement
//    // so no replacement procedure is needed

//    // OPERATORS
//    //////////////////////////////////////
//    // The variation operators
//    //////////////////////////////////////
//    // CROSSOVER
//    // 1-point crossover for bitstring
//    eoUniformAtomExchange<Indi::AtomType> atomExchange;
//    eoVlAtomExchangeQuadOp<Indi> crossover(MIN_VEC_SIZE, MAX_VEC_SIZE, atomExchange);
//    // MUTATION
//    // standard bit-flip mutation for bitstring
//    eoVlAddMutation<Indi> addMutation(MAX_VEC_SIZE, atomInit);
//    eoUniformGeneChooser<Indi> geneChooser;
//    eoVlDelMutation<Indi> delMutation(MAX_VEC_SIZE, geneChooser);
//    eoShiftMutation<Indi> shiftMutation;
//    CyclicShoveMutation<Indi::AtomType> shove(1, 0, tokens.size() - 1);
//    eoFlOrAllMutation<Indi> allMutation(shove, 0.1);
//    eoPropCombinedMonOp<Indi> mutation(addMutation, 0.1);
//    mutation.add(delMutation, 0.1);
//    mutation.add(shiftMutation, 0.0);
//    mutation.add(allMutation, 0.4);
//    // STOP
//    // CHECKPOINT
//    //////////////////////////////////////
//    // termination condition
//    /////////////////////////////////////
//    // stop after MAX_GEN generations
//    eoGenContinue<Indi> continuator(MAX_GEN);

//    // GENERATION
//    /////////////////////////////////////////
//    // the algorithm
//    ////////////////////////////////////////
//    // standard Generational GA requires as parameters
//    // selection, evaluation, crossover and mutation, stopping criterion


//    eoSGA<Indi> gga(select, crossover, CROSS_RATE, mutation, MUT_RATE,
//            eval, continuator);

//    // Apply algo to pop - that's it!
//    gga(pop);

//    // OUTPUT
//    // Print (sorted) intial population
//    pop.sort();
//    //cout << "FINAL Population\n" << pop << endl;
//    // GENERAL
//    //
//    //
//    cout << pop[0].fitness() << endl;
//    cout << pop.best_element().fitness() << endl;
//    cout << pop.worse_element().fitness() << endl;
//    //eval(pop[0]);
//    unsigned idx = 0;
//    for(auto i : pop[0]) {
//        cout << idx++ << " -> " << tokext::utils::translateToken<std::string>(scx, tokens.token(i)) << endl;
//    }
//}
// A main that catches the exceptions

//#include "hisi/hindisght.hpp"
//#include <tuple>
//#include "hisi/utils/tuple_func.hpp"

//double f1(int& v) {v += 2; cout << "f(int)" << v << endl; return v;}
//double g1(double& v) {v += 1.1; cout << "g(double)"<< v<< endl; return v;}
//double ff(double v1, double v2) {cout << "middle" << endl; return v1+v2;}
//double fff(double d) {cout << d << " "; return d;}
//void ggg(double d) {cout << d << " "; return ;}

#include "hisi/eval/PositionConstraint.hpp"

//#include "hisi/utils/tuple_func.hpp"
//#include "hisi/utils/gamath.hpp"
#include "hisi/eval/Content.hpp"
#include "hisi/eval/content_op.hpp"
#include "hisi/eval/integer_dict_op.hpp"
#include "hisi/eval/integer_op.hpp"
#include "hisi/utils/constant.hpp"

#include <sstream>
using namespace hisi;

//int main(int argc, char* argv[])
//{
//    Content c1, c2, c3;
//    IntegerUniformInit<Content::idx_t> ti(0, 10);
//    IntegerUniformInit<int> c1i(-10, 20);
//    IntegerUniformInit<int> c2i(-100, -10);
//    IntegerUniformInit<int> c3i(20, 110);
//    IndivIntegerDictInit<ContentAttars, int> cai(ContentAttars::DEPTH, c1i);
//    cai.add(ContentAttars::OFFSET, c2i); cai.add(ContentAttars::WITHIN, c3i);
//    ContentInit cti(ti, cai);
//    cti(c1);
//    cti(c2);
//    cout << c1 << endl;
//    cout << c2 << endl;
//    stringstream ss;
//    ss << c1;
//    ss >> c3;
//    cout << ss.str() << endl;
//    cout << c3 << endl;


//    auto ts = hisi::utils::constant::Value<Content::idx_t>(2);
//    IntegerConstantWrapMut<Content::idx_t> tim(0, 10,ts );
//    auto c1s = hisi::utils::constant::Value<int>(-7);
//    IntegerConstantWrapMut<int> c1m(-10, 20, c1s );
//    IndivIntegerDictMut<ContentAttars, int> cam(ContentAttars::DEPTH, c1m);
//    ContentMut ctm(tim, cam);
//    for(auto i = 0; i<10; ++i) {
//       ctm(c1);
//        cout << c1 << endl;
//    }

//    return 0;
//}

