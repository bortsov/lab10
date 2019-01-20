/*
 * physv.h
 *
 *  Created on: 26 дек. 2014 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_PHYSV_H_
#define INCLUDE_PHYSV_H_


#include "PhysicalValue.h"

#define PHYSICAL_VALUE(nameValue)    \
    namespace physv {                \
    namespace nameValue {            \
    void create();                   \
    void destroy();                  \
    float get();                     \
    int32_t getADCcode();            \
    const float& getRefValue();      \
    const int32_t& getRefIntegerValue(); \
    void setOffset(int32_t);         \
    } /* namespace nameValue */      \
    } /* namespace physv */

PHYSICAL_VALUE(boardVoltage)

#endif /* INCLUDE_PHYSV_H_ */
