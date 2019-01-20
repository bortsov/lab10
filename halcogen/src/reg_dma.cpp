/*
 * reg_dma.cpp
 *
 *  Created on: 8 янв. 2018 г.
 *      Author: Виталий
 */


#include "reg_dma.h"

static volatile DmaBase * const dmaBase =  reinterpret_cast<volatile DmaBase *>(0xFFFFF000U);
static volatile DmaRamBase * const dmaRamBase = reinterpret_cast<volatile DmaRamBase *>(0xFFF80000U);

volatile DmaBase& getDmaBase() {return *dmaBase;}
volatile DmaRamBase& getDmaRamBase() {return *dmaRamBase;}
