/*
 * vclk2.cpp
 *
 *  Created on: 03 сент. 2015 г.
 *      Author: bortsov
 */

#include "vclk2.h"
#include "system.h"

namespace vclk2 {

float getTicksFor_ns(uint32_t ns)
{
    const double t = ns * 1e-9;
    constexpr double vclk2tick = (1e-6 / getFreqVCLK2());
    return t / vclk2tick;
}


float getFreq()
{
    return getFreqVCLK2() * 1e6F;
}

float getFreqMhz()
{
    return getFreqVCLK2();
}

} /* namespace vclk2 */


