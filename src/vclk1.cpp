/*
 * vclk1.cpp
 *
 *  Created on: 03 сент. 2015 г.
 *      Author: bortsov
 */

#include "vclk1.h"
#include "system.h"

namespace vclk1 {

float getTicksFor_ns(uint32_t ns)
{
    const double t = ns * 1e-9;
    constexpr double vclk1tick = (1e-6 / getFreqVCLK1());
    return t / vclk1tick;
}


float getFreqHz()
{
    return getFreqVCLK1() * 1e6F;
}

float getFreqMhz()
{
    return getFreqVCLK1();
}

} /* namespace vclk1 */


