#ifndef _SVGEN_H_
#define _SVGEN_H_

#include "Motor/controller_data_types.h"
#include "modules/ipark/ipark.h"

namespace svgen {

using Svgen = MATH_vec3;

Svgen run(const ipark::Ipark& Valfa_beta_pu);

} /* namespace svgen */

//@}  // ingroup

#endif // end of _SVGEN_H_ definition

