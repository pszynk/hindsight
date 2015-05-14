
#ifndef CONFIG_HPP_TZLYXH9Q
#define CONFIG_HPP_TZLYXH9Q

#include <string>

namespace hisi
{
    namespace config
    {

static const std::string NO_VAL = "none";

        namespace path
        {

static const std::string ROOT = "hindsight";

//match
static const std::string MATCH_A = ROOT+".match";
static const std::string MATCH_RULE_A = MATCH_A+".rule";

//eval
static const std::string EVAL_A = ROOT+".eval";

//chrom
static const std::string CHROM_A = ROOT+".chrom";

//content
static const std::string CONTENT_A = CHROM_A+".content";

        } /* path */

        namespace def
        {

static const std::string DEF_MATCH_RULE = NO_VAL;

        } /* def */
    } /* config */
} /* hisi */

#endif /* end of include guard: CONFIG_HPP_TZLYXH9Q */
