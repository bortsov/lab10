/*
 * pinmux.cpp
 *
 *  Created on: 14 сент. 2015 г.
 *      Author: bortsov
 */

#include "pinmux.h"
#include <cstdint>

namespace init {


static volatile uint32_t * const kickerReg = reinterpret_cast<volatile uint32_t*>(0xFFFFEA38U);
static volatile uint32_t * const pinMuxReg = reinterpret_cast<volatile uint32_t*>(0xFFFFEB10U);


static void setBitInPinMuxReg(uint32_t nReg, uint32_t nBit)
{
    uint32_t v = pinMuxReg[nReg];
    if (nBit <= 7) {
        v &= 0xFFFFFF00;
    } else if (nBit <= 15) {
        v &= 0xFFFF00FF;
    } else if (nBit <= 23) {
        v &= 0xFF00FFFF;
    } else {
        v &= 0x00FFFFFF;
    }
    v |= 1 << nBit;
    pinMuxReg[nReg] = v;
}


static void enableChangePinMux()
{
    kickerReg[0] = 0x83E70B13U;
    kickerReg[1] = 0x95A4F1E0U;
}


static void disableChangePinMux()
{
    kickerReg[0] = 0U;
    kickerReg[1] = 0U;
}

static void setPinA6AsGioa4()       {}
static void setPinM2AsGiob0()       {setBitInPinMuxReg(18, 24);}
static void setPinK2AsGiob1()       {setBitInPinMuxReg(21, 8);}
static void setPinK18AsNet1_0()     {setBitInPinMuxReg(5, 0);}
static void setPinV2AsNet1_1()      {setBitInPinMuxReg(4, 16);}
static void setPinB12AsHet1_4()     {}
static void setPinE18AsHet1_8()     {setBitInPinMuxReg(14, 0);}
static void setPinN1AsHet1_15()     {setBitInPinMuxReg(8, 16);}
static void setPinP2AsHet1_20()     {}
static void setPinB11AsHet1_30()	{setBitInPinMuxReg(19, 8);}
static void setPinW9AsHet1_31()     {setBitInPinMuxReg(26, 8);}


static void setUsedPins()
{
    setPinA6AsGioa4();
    setPinM2AsGiob0();
    setPinK2AsGiob1();
    setPinK18AsNet1_0();
    setPinV2AsNet1_1();
    setPinB12AsHet1_4();
    setPinE18AsHet1_8();
    setPinN1AsHet1_15();
    setPinP2AsHet1_20();
    setPinB11AsHet1_30();
    setPinW9AsHet1_31();
}



static void setOptionTriggerSourceForAllMibadcs() {setBitInPinMuxReg(30, 1);}


static void setTriggerPins()
{
    setOptionTriggerSourceForAllMibadcs();
}


void pinmux()
{
    enableChangePinMux();
    setUsedPins();
    setTriggerPins();
    disableChangePinMux();
}

} /* namespace init */
