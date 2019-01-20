/*
 * reg_esm.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_esm.h"

static volatile EsmBase * const esmREG = reinterpret_cast<volatile EsmBase *>(0xFFFFF500U);

volatile EsmBase& getEsm() {return *esmREG;}


