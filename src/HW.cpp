/*
 * HW.cpp
 *
 *  Created on: 10 сент. 2015 г.
 *      Author: bortsov
 */

#include "HW.h"
#include "userExitCodes.h"

#include <cstdint>

namespace hw {

static bool isMainOscillatorFailed()
{
    const uint32_t *GLBSSTAT_address = reinterpret_cast<const uint32_t *>(0xFFFFFFECUL);
    return (*GLBSSTAT_address) & 1;
}

void create()
{
    if (isMainOscillatorFailed()) {
    	exit(EXIT_SYSTEM_MAIN_OSCILLATOR_FAIL);
    }
}

} /* namespace hw */




