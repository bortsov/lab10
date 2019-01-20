/*
 * reg_gio.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_gio.h"

static volatile GioBase * const gioREG   = reinterpret_cast<volatile GioBase *>(0xFFF7BC00U);

static volatile GioPort * const gioPORTA = reinterpret_cast<volatile GioPort *>(0xFFF7BC34U);
static volatile GioPort * const gioPORTB = reinterpret_cast<volatile GioPort *>(0xFFF7BC54U);

volatile GioBase& getGio()   {return *gioREG;}
volatile GioPort& getPortA() {return *gioPORTA;}
volatile GioPort& getPortB() {return *gioPORTB;}



