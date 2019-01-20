/*
 * vclk2.h
 *
 *  Created on: 03 сент. 2015 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_VCLK2_H_
#define INCLUDE_VCLK2_H_

#include <cstdint>

namespace vclk2 {

float getTicksFor_ns(uint32_t ns);
float getFreq();
float getFreqMhz();

} /* namespace vclk2 */



#endif /* INCLUDE_VCLK2_H_ */
