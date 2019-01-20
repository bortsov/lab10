/*
 * phasor.cpp
 *
 *  Created on: 25 февр. 2018 г.
 *      Author: Виталий
 */

#include "phasor.h"

#include "sin_cos_f32.h"


namespace phasor {

PhasorSinCos createFrom_pu(const float theta_pu)
{
    return sin_cos_pu_f32(theta_pu);
}



} /* namespace phasor */
