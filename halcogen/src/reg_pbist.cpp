/*
 * reg_pbist.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_pbist.h"

static volatile PbistBase * const pbistREG = reinterpret_cast<volatile PbistBase *>(0xFFFFE560U);

volatile PbistBase& getPbist() {return *pbistREG;}



