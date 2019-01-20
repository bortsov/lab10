#ifndef _IPARK_H_
#define _IPARK_H_


#include "Motor/controller_data_types.h"
#include "modules/phasor/phasor.h"

namespace ipark {

using Ipark = MATH_vec2;

Ipark run(const float theta_pu, const MATH_vec2& pInVec);
Ipark run(const phasor::PhasorSinCos& phasor, const MATH_vec2& pInVec);

} /* namespace ipark */


#endif // end of _IPARK_H_ definition


