
#include "SSPNMatchConverter.hpp"

#include <cassert>

namespace hisi
{

double SSPNMatchConverter::operator()(const MatchResult& stats)
{
//    // true positive
    double tp = stats.suspicious().matched();
//    // false negative
    double fn = stats.suspicious().unmatched();

//    // false positive
    double fp = stats.normal().matched();
//    // true negative
    double tn = stats.normal().unmatched();


//    // sensitivity
    double se = 1;

//    // positive predictive value
    double ppv = 1;

    // if there were any suspicious flows
    if (stats.suspicious().considered()) {
        // didn't match any suspicious then value will be 0
        if(!tp) {return 0;}
        if (this->se_p_) {
            se = tp / (tp + fn);
        }
        if (this->ppv_p_) {
            ppv = tp / (tp + fp);
        }
    }

//    // specificity
    double sp = 1;

//    // negative predictive value
    double npv = 1;

    if (stats.normal().considered()) {
        // matched all normal then value will be 0
        if(!tn) {return 0;}
        if (this->sp_p_) {
            sp = tn / (tn + fp);
        }
        if (npv_p_) {
            npv = tn / (tn + fn);
        }
    }

    return se * sp * ppv * npv;
}

} /* hisi */
