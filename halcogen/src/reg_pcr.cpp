/*
 * reg_pcr.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_pcr.h"

static volatile PcrBase * const pcrREG = reinterpret_cast<volatile PcrBase *>(0xFFFFE000U);

volatile PcrBase& getPcr() {return *pcrREG;}



