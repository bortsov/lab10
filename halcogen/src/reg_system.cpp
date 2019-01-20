/*
 * reg_system.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_system.h"

static volatile System1Base * const systemREG1 = reinterpret_cast<volatile System1Base *>(0xFFFFFF00U);
static volatile System2Base * const systemREG2 = reinterpret_cast<volatile System2Base *>(0xFFFFE100U);

static volatile GioPort * const systemPORT = reinterpret_cast<volatile GioPort *>(0xFFFFFF04U);

volatile System1Base& getSystem1() {return *systemREG1;}
volatile System2Base& getSystem2() {return *systemREG2;}

volatile GioPort& getSystemPort() {return *systemPORT;}



