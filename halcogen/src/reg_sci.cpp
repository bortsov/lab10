/*
 * reg_sci.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_sci.h"

static volatile SciBase * const sciREG        = reinterpret_cast<volatile SciBase *>(0xFFF7E500U);
static volatile SciBase * const scilinREG     = reinterpret_cast<volatile SciBase *>(0xFFF7E400U);
static volatile GioPort * const sciPORT       = reinterpret_cast<volatile GioPort *>(0xFFF7E540U);
static volatile GioPort * const scilinPORT    = reinterpret_cast<volatile GioPort *>(0xFFF7E440U);

volatile SciBase& getSci()        {return *sciREG;}
volatile SciBase& getScilin()     {return *scilinREG;}
volatile GioPort& getSciPort()    {return *sciPORT;}
volatile GioPort& getScilinPort() {return *scilinPORT;}



