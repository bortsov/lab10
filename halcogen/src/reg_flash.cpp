/*
 * reg_flash.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_flash.h"

static volatile FlashBase * const flashWREG = reinterpret_cast<volatile FlashBase *>(0xFFF87000U);

volatile FlashBase& getFlash() {return *flashWREG;}


