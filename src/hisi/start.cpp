
#include "start.hpp"
#include "config.hpp"
#include "make/make_config.hpp"

#include <ga/make_ga.h>

#include <iostream>
#include <stdexcept>


// ----------------------------  start  ---------------------------------------
namespace hisi
{


// impl. of starter class
struct StarterBase
{
    virtual void start(eoParser&, const libconfig::Config &config) = 0;
    virtual ~StarterBase() {};
};

// starter for ga with rule = RuleFunc
template<class RuleFunc>
struct RuleStarter : public StarterBase
{
    void start(eoParser& parser, const libconfig::Config &config) override
    {
        hisi::load::Loader<RuleFunc> loader(config);
        loader.run(parser);
        //hisi::configure<RuleFunc>(argv - 1, argc + 1);
    }
};

using starter_map = std::unordered_map<std::string, StarterBase*>;

// create starters;
// GA
RuleStarter<hisi::NullRuleFunc<>> noneStarter;
RuleStarter<hisi::PositionRuleFunc<hisi::pos::SingleRule<hisi::pos::OffsetPolicy>>>     offsStarter;
RuleStarter<hisi::PositionRuleFunc<hisi::pos::SingleRule<hisi::pos::DepthPolicy>>>      deptStarter;
RuleStarter<hisi::PositionRuleFunc<hisi::pos::SingleRule<hisi::pos::DistancePolicy>>>   distStarter;
RuleStarter<hisi::PositionRuleFunc<hisi::pos::SingleRule<hisi::pos::WithinPolicy>>>     withStarter;
RuleStarter<hisi::PositionRuleFunc<hisi::pos::OffsetDepthRule>>                         ofdeStarter;
RuleStarter<hisi::PositionRuleFunc<hisi::pos::DistanceWithinRule>>                      diwiStarter;

// commands:
starter_map starters
{
    {"none", &noneStarter },
    {"offs", &offsStarter },
    {"dept", &deptStarter },
    {"dist", &distStarter },
    {"with", &withStarter },
    {"ofde", &ofdeStarter },
    {"diwi", &diwiStarter },
};

int start (int argv ,char* argc[])
{
    using namespace hisi::start_;
    using namespace libconfig;
    using namespace std;

    eoParser parser(argv, argc);
    Config cfg;
    try {
        make::make_config(parser, cfg);
    } catch(make::InputParamError ipe) {
        parser.printHelp(cerr);
        cerr << "Error getting config: " << ipe.what() << endl;
        return (EXIT_FAILURE);
    }

    string rule = config::def::DEF_MATCH_RULE;
    try
    {
        rule = cfg.lookup(config::path::MATCH_RULE_A).c_str();
        cout << "Chosen rule: " << rule << endl << endl;
    }
    catch(const SettingNotFoundException &nfex)
    {
        cerr << "No '" << config::path::MATCH_RULE_A <<
            "' setting in configuration file." << endl
            << "Using default rule: '" << rule << "'" << endl;
    }

    try {
        starters.at(rule)->start(parser, cfg);
    } catch(out_of_range ex) {
        cerr << "Unknown rule: '" << rule << "'" << endl;
        return(EXIT_FAILURE);
    }

    return(EXIT_SUCCESS);

    } /* start */
} /* hisi */
