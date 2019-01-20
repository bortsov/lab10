/*
 * reg_i2c.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_i2c.h"

static volatile I2cBase * const i2cREG1  = reinterpret_cast<volatile I2cBase *>(0xFFF7D400U);
static volatile GioPort * const i2cPORT1 = reinterpret_cast<volatile GioPort *>(0xFFF7D44CU);

volatile I2cBase& getI2c()      {return *i2cREG1;}
volatile GioPort& getI2cPort()  {return *i2cPORT1;}



