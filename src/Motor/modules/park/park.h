#ifndef _PARK_H_
#define _PARK_H_


#include "Motor/controller_data_types.h"
#include "modules/phasor/phasor.h"
#include "modules/clarke/clarke.h"

namespace park {

using Park = MATH_vec2;

Park run(const float theta_pu, const clarke::Clarke& pInVec);
Park run(const phasor::PhasorSinCos& phasor, const clarke::Clarke& pInVec);

} /* namespace park */


#endif
