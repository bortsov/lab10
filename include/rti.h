/*
 * rti.h
 *
 *  Created on: 16 сент. 2015 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_RTI2_H_
#define INCLUDE_RTI2_H_

#include <cstdint>

namespace mcal {
namespace rti {
uint32_t get();
void create();
bool isFlagSystick();
void resetFlagSystick();
uint32_t getCounterSysticks();
} /* namespace rti */
} /* namespace mcal */



#endif /* INCLUDE_RTI2_H_ */
