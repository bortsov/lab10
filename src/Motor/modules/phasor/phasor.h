/*
 * phasor.h
 *
 *  Created on: 25 февр. 2018 г.
 *      Author: Виталий
 */

#ifndef SRC_MOTOR_MODULES_PHASOR_PHASOR_H_
#define SRC_MOTOR_MODULES_PHASOR_PHASOR_H_

#include "Motor/controller_data_types.h"

namespace phasor {

using PhasorSinCos = MATH_vec2;


PhasorSinCos createFrom_pu(const float theta_pu);


} /* namespace phasor */

#endif /* SRC_MOTOR_MODULES_PHASOR_PHASOR_H_ */
