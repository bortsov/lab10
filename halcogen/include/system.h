#ifndef __SYS_SYSTEM_H__
#define __SYS_SYSTEM_H__

#include "reg_system.h"
#include "reg_flash.h"

#include <stdint.h>

/* System General Definitions */

/** @enum systemClockSource
*   @brief Alias names for clock sources
*
*   This enumeration is used to provide alias names for the clock sources:
*     - Oscillator
*     - Pll1
*     - External1
*     - Low Power Oscillator Low
*     - Low Power Oscillator High
*     - PLL2
*     - External2
*     - Synchronous VCLK1
*/
enum SYSTEM_CLOCK_SOURCE
{
    SYS_OSC             = 0U,  /**< Alias for oscillator clock Source                */
    SYS_PLL1            = 1U,  /**< Alias for Pll1 clock Source                      */
    SYS_EXTERNAL1       = 3U,  /**< Alias for external clock Source                  */
    SYS_LPO_LOW         = 4U,  /**< Alias for low power oscillator low clock Source  */
    SYS_LPO_HIGH        = 5U,  /**< Alias for low power oscillator high clock Source */
    SYS_PLL2            = 6U,  /**< Alias for Pll2 clock Source                      */
    SYS_EXTERNAL2       = 7U,  /**< Alias for external 2 clock Source                */
    SYS_VCLK            = 9U   /**< Alias for synchronous VCLK1 clock Source         */
};

#define SYS_DOZE_MODE        0x000F3F02U
#define SYS_SNOOZE_MODE      0x000F3F03U
#define SYS_SLEEP_MODE       0x000FFFFFU

#define POWERON_RESET        0x8000U
#define OSC_FAILURE_RESET    0x4000U
#define WATCHDOG_RESET       0x2000U
#define ICEPICK_RESET        0x2000U
#define CPU_RESET            0x0020U
#define SW_RESET             0x0010U

uint32_t getID();

static constexpr float getFreqOsc()   {return 16.0F;}
static constexpr float getFreqPLL1()  {return 180.00F;}
static constexpr float getFreqLPO_LF(){return 0.080F;}
static constexpr float getFreqLPO_HF(){return 12.067F;}
static constexpr float getFreqPLL2()  {return 180.00F;}
static constexpr float getFreqGCLK()  {return 180.000F;}
static constexpr float getFreqRTI()   {return 16.000F;}
static constexpr float getFreqAVCLK1(){return 16.000F;}
static constexpr float getFreqAVCLK2(){return 90.0F;}
static constexpr float getFreqAVCLK3(){return 0.000F;}
static constexpr float getFrqAVCLK4() {return 0.000F;}
static constexpr float getFreqVCLK1() {return 90.000F;}
static constexpr float getFreqVCLK2() {return 90.000F;}
static constexpr float getFreqVCLK3() {return 0.000F;}
static constexpr float getFreqVCLK4() {return 90.0F;}


static constexpr SYSTEM_CLOCK_SOURCE getSys_PRE1() {return SYS_OSC;}
static constexpr SYSTEM_CLOCK_SOURCE getSys_PRE2() {return SYS_OSC;}

extern volatile uint32_t * const WATCHDOG_STATUS;

/* FlashW General Definitions */

/** @enum flashWPowerModes
*   @brief Alias names for flash bank power modes
*
*   This enumeration is used to provide alias names for the flash bank power modes:
*     - sleep
*     - standby
*     - active
*/
enum flashWPowerModes
{
    SYS_SLEEP   = 0U, /**< Alias for flash bank power mode sleep   */
    SYS_STANDBY = 1U, /**< Alias for flash bank power mode standby */
    SYS_ACTIVE  = 3U  /**< Alias for flash bank power mode active  */
};


volatile uint32_t& getLpoTrimValue();
volatile uint32_t& getSysException();

volatile uint32_t& getFSM_WR_Ena_HL();
volatile uint32_t& getEEPROM_Config_HL();

uint32_t getHclkFreqHz();

void doReset();

#endif
