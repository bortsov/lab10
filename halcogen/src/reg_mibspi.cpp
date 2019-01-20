/*
 * reg_mibspi.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_mibspi.h"

static volatile MibspiRam * const mibspiRAM1 = reinterpret_cast<volatile MibspiRam *>(0xFF0E0000U);
static volatile MibspiRam * const mibspiRAM3 = reinterpret_cast<volatile MibspiRam *>(0xFF0C0000U);
static volatile MibspiRam * const mibspiRAM5 = reinterpret_cast<volatile MibspiRam *>(0xFF0A0000U);

static volatile uint32_t * const mibspiPARRAM1 = reinterpret_cast<volatile uint32_t*>(mibspiRAM1 + 0x00000400U);
static volatile uint32_t * const mibspiPARRAM3 = reinterpret_cast<volatile uint32_t*>(mibspiRAM3 + 0x00000400U);
static volatile uint32_t * const mibspiPARRAM5 = reinterpret_cast<volatile uint32_t*>(mibspiRAM5 + 0x00000400U);

static volatile MibspiBase * const mibspiREG1 = reinterpret_cast<volatile MibspiBase *>(0xFFF7F400U);
static volatile MibspiBase * const mibspiREG3 = reinterpret_cast<volatile MibspiBase *>(0xFFF7F800U);
static volatile MibspiBase * const mibspiREG5 = reinterpret_cast<volatile MibspiBase *>(0xFFF7FC00U);

static volatile GioPort * const mibspiPORT1 = reinterpret_cast<volatile GioPort *>(0xFFF7F418U);
static volatile GioPort * const mibspiPORT3 = reinterpret_cast<volatile GioPort *>(0xFFF7F818U);
static volatile GioPort * const mibspiPORT5 = reinterpret_cast<volatile GioPort *>(0xFFF7FC18U);

volatile MibspiRam& getMibspi1Ram() {return *mibspiRAM1;}
volatile MibspiRam& getMibspi3Ram() {return *mibspiRAM3;}
volatile MibspiRam& getMibspi5Ram() {return *mibspiRAM5;}

volatile uint32_t * getMibspi1ParityRam() {return mibspiPARRAM1;}
volatile uint32_t * getMibspi3ParityRam() {return mibspiPARRAM3;}
volatile uint32_t * getMibspi5ParityRam() {return mibspiPARRAM5;}

volatile MibspiBase& getMibspi1() {return *mibspiREG1;}
volatile MibspiBase& getMibspi3() {return *mibspiREG3;}
volatile MibspiBase& getMibspi5() {return *mibspiREG5;}

volatile GioPort& getMibspi1Port() {return *mibspiPORT1;}
volatile GioPort& getMibspi3Port() {return *mibspiPORT3;}
volatile GioPort& getMibspi5Port() {return *mibspiPORT5;}



