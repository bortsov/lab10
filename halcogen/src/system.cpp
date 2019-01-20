/*
 * system.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "system.h"
#include "userExitCodes.h"

#include <cmath>



volatile uint32_t * const WATCHDOG_STATUS = reinterpret_cast<volatile uint32_t*>(0xFFFFFC98U);

static volatile uint32_t * const LPO_TRIM_VALUE = reinterpret_cast<volatile uint32_t*>(0xF00801B4U);
static volatile uint32_t * const SYS_EXCEPTION  = reinterpret_cast<volatile uint32_t*>(0xFFFFFFE4U);

static volatile uint32_t * const FSM_WR_ENA_HL    = reinterpret_cast<volatile uint32_t*>(0xFFF87288U);
static volatile uint32_t * const EEPROM_CONFIG_HL = reinterpret_cast<volatile uint32_t*>(0xFFF872B8U);

static volatile uint32_t * const DEVICE_ID_REV   = reinterpret_cast<volatile uint32_t*>(0xFFFFFFF0U);

volatile uint32_t& getLpoTrimValue() {return *LPO_TRIM_VALUE;}
volatile uint32_t& getSysException() {return *SYS_EXCEPTION;}

volatile uint32_t& getFSM_WR_Ena_HL() {return *FSM_WR_ENA_HL;}
volatile uint32_t& getEEPROM_Config_HL() {return *EEPROM_CONFIG_HL;}

uint32_t getID() {return *DEVICE_ID_REV;}


void doReset()
{
    uint32_t *SYSECR = reinterpret_cast<uint32_t *>(0xFFFFFFE0);
    *SYSECR = 0x8000;
}



static SYSTEM_CLOCK_SOURCE getClkSourceForHclk()
{
    return static_cast<SYSTEM_CLOCK_SOURCE>(getSystem1().GHVSRC & 0xF);
}


uint32_t getHclkFreqHz()
{
    const SYSTEM_CLOCK_SOURCE src = getClkSourceForHclk();
    float f_MHz;
    if (SYS_OSC == src) {
        f_MHz = getFreqOsc();
    } else if (SYS_PLL1 == src) {
        f_MHz = getFreqPLL1();
    } else {
        exit(EXIT_SYSTEM_INVALID_HCLK_SOURCE);
    }
    return f_MHz * 1e6F;
}



