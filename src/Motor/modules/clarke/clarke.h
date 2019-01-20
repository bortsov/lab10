#ifndef _CLARKE_H_
#define _CLARKE_H_


#include "Motor/controller_data_types.h"

namespace clarke {

using Clarke = MATH_vec2;

Clarke run(const MATH_vec3& pInVec);
Clarke run(const MATH_vec2& pInVec);

} /* namespace clarke */


#endif // end of _CLARKE_H_ definition

