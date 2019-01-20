/*
 * vclk1.h
 *
 *  Created on: 03 сент. 2015 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_VCLK1_H_
#define INCLUDE_VCLK1_H_

#include <cstdint>

namespace vclk1 {

float getTicksFor_ns(uint32_t ns);
float getFreqHz();
float getFreqMhz();

} /* namespace vclk1 */



#endif /* INCLUDE_VCLK1_H_ */
