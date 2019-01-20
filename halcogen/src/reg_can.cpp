/*
 * reg_can.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_can.h"

static volatile CanBase * const canREG1 =  reinterpret_cast<volatile CanBase *>(0xFFF7DC00U);
static volatile CanBase * const canREG2 =  reinterpret_cast<volatile CanBase *>(0xFFF7DE00U);
static volatile CanBase * const canREG3 =  reinterpret_cast<volatile CanBase *>(0xFFF7E000U);


volatile CanBase& getCan1() {return *canREG1;}
volatile CanBase& getCan2() {return *canREG2;}
volatile CanBase& getCan3() {return *canREG3;}

