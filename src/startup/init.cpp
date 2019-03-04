/*
 * commonInit.c
 *
 *  Created on: 14 сент. 2015 г.
 *      Author: bortsov
 */
#include <cstddef>
#include "system.h"
#include "sys_vim.h"
#include "init.h"
#include "rti.h"
#include "reg_adc.h"
#include "reg_pcr.h"
#include "reg_vim.h"
#include "internalAdc1.h"


void esmHighInterrupt(void) __attribute__((target("arm"),interrupt("FIQ")));
extern "C" void phantomInterruptFromIsr() __attribute__ ((target("arm"), interrupt("IRQ")));


namespace init {

void pll()
{
    auto systemREG1 = &getSystem1();

    /* Disable PLL1 */
    systemREG1->CSDISSET = 0x00000002U;
    while((systemREG1->CSDIS & 0x2U) != 0x2U) ;

    /* Clear Global Status Register */
    systemREG1->GBLSTAT = 0x301U;

    systemREG1->PLLCTL1 =  (uint32_t)0x00000000U
                        |  (uint32_t)0x20000000U
                        |  (uint32_t)((uint32_t)0x1FU << 24U)
                        |  (uint32_t)0x00000000U
                        |  (uint32_t)((uint32_t)(8U - 1U)<< 16U)
                        |  (uint32_t)(0xB300U);

    systemREG1->PLLCTL2 =  (uint32_t)((uint32_t)255U << 22U)
                        |  (uint32_t)((uint32_t)7U << 12U)
                        |  (uint32_t)((uint32_t)(2U - 1U) << 9U)
                        |  (uint32_t)61U;

    /** - Enable PLL(s) to start up or Lock */
    systemREG1->CSDIS = 0x00000000U
                      | 0x00000000U
                      | 0x00000008U
                      | 0x00000080U
                      | 0x00000000U
                      | 0x00000000U
                      | 0x00000000U;
}


void peripheral()
{
    auto systemREG1 = &getSystem1();
    auto pcrREG = &getPcr();

    systemREG1->CLKCNTL &= 0xFFFFFEFFU;
    pcrREG->PSPWRDWNCLR0 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR1 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR2 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR3 = 0xFFFFFFFFU;
    systemREG1->CLKCNTL |= 0x00000100U;
}


void flash()
{
    auto flashWREG = &getFlash();

    flashWREG->FRDCNTL =  0x00000000U
                       | (uint32_t)((uint32_t)3U << 8U)
                       | (uint32_t)((uint32_t)1U << 4U)
                       |  1U;
    getFSM_WR_Ena_HL()    = 0x5U;
    getEEPROM_Config_HL() = 0x00000002U | (3 << 16) ;
    getFSM_WR_Ena_HL()    = 0xAU;
    flashWREG->FBFALLBACK = 0x00000000U
                          | (uint32_t)((uint32_t)SYS_SLEEP << 14U) /* BANK 7 */
                          | (uint32_t)((uint32_t)SYS_SLEEP << 2U)  /* BANK 1 */
                          | (uint32_t)((uint32_t)SYS_ACTIVE << 0U); /* BANK 0 */
}


/** - Configure the LPO such that HF LPO is as close to 10MHz as possible */
void trimLpo()
{
    auto& sys = getSystem1();
    if ((getLpoTrimValue() >> 16) != 0xFFFFU) {
        sys.LPOMONCTL  = (1U << 24U) | (getLpoTrimValue() >> 16);
    } else {
        sys.LPOMONCTL   =  (1U << 24U) | (21U << 8U) | 16U;
    }
}


void mapClocks()
{
    uint32_t SYS_CSVSTAT, SYS_CSDIS;
    auto systemREG1 = &getSystem1();
    auto systemREG2 = &getSystem2();

    /** @b Initialize @b Clock @b Tree: */
    /** - Disable / Enable clock domain */
    systemREG1->CDDIS = (uint32_t)((uint32_t)0U << 4U ) /* AVCLK1 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)1U << 5U ) /* AVCLK2 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)1U << 8U ) /* VCLK3 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)0U << 9U ) /* VCLK4 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)1U << 10U) /* AVCLK3 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)1U << 11U); /* AVCLK4 , 1 - OFF, 0 - ON */


    /* Work Around for Errata SYS#46:
     *
     * Errata Description:
     *            Clock Source Switching Not Qualified with Clock Source Enable And Clock Source Valid
     * Workaround:
     *            Always check the CSDIS register to make sure the clock source is turned on and check
     * the CSVSTAT register to make sure the clock source is valid. Then write to GHVSRC to switch the clock.
     */
    /** - Wait for until clocks are locked */
    SYS_CSVSTAT = systemREG1->CSVSTAT;
    SYS_CSDIS = systemREG1->CSDIS;
    while ((SYS_CSVSTAT & ((SYS_CSDIS ^ 0xFFU) & 0xFFU)) != ((SYS_CSDIS ^ 0xFFU) & 0xFFU))
    {
        SYS_CSVSTAT = systemREG1->CSVSTAT;
        SYS_CSDIS = systemREG1->CSDIS;
    } /* Wait */

    /* Now the PLLs are locked and the PLL outputs can be sped up */
    /* The R-divider was programmed to be 0xF. Now this divider is changed to programmed value */
    systemREG1->PLLCTL1 = (systemREG1->PLLCTL1 & 0xE0FFFFFFU) | (uint32_t)((uint32_t)(1U - 1U) << 24U);

    /* Enable/Disable Frequency modulation */
    systemREG1->PLLCTL2 |= 0x00000000U;

    /** - Map device clock domains to desired sources and configure top-level dividers */
    /** - All clock domains are working off the default clock sources until now */
    /** - The below assignments can be easily modified using the HALCoGen GUI */

    /** - Setup GCLK, HCLK and VCLK clock source for normal operation, power down mode and after wakeup */
    systemREG1->GHVSRC = (uint32_t)((uint32_t)SYS_OSC << 24U)
                       | (uint32_t)((uint32_t)SYS_OSC << 16U)
                       | (uint32_t)((uint32_t)SYS_PLL1 << 0U);

    /** - Setup synchronous peripheral clock dividers for VCLK1, VCLK2, VCLK3 */
    systemREG1->CLKCNTL  = (systemREG1->CLKCNTL & 0xF0FFFFFFU)
                         | (uint32_t)((uint32_t)1U << 24U);
    systemREG1->CLKCNTL  = (systemREG1->CLKCNTL & 0xFFF0FFFFU)
                         | (uint32_t)((uint32_t)1U << 16U);

    systemREG2->CLK2CNTL = (systemREG2->CLK2CNTL & 0xFFFFF0FFU)
                         | (uint32_t)((uint32_t)1U << 8U);

    systemREG1->VCLKASRC = (uint32_t)((uint32_t)SYS_VCLK << 8U)
                         | (uint32_t)((uint32_t)SYS_OSC << 0U);
}


void vimSetAllHandlerToPhantomInterrupt()
{
    for (size_t i = 0; i < VIM_CHANNELS; ++i) {
        auto& v = getVimRam()[i];
        v = reinterpret_cast<uint32_t>(phantomInterruptFromIsr);
    }
}


void vim()
{
    getVimParityControl() = 0x5U;

    constexpr size_t esmHighInterruptNumber = 0;
    registerFiqVectorInVim(esmHighInterruptNumber, esmHighInterrupt);

    auto& vim = getVim();

    vim.FIRQPR0 = (SYS_FIQ << 0U)
                    | (SYS_FIQ << 1U)
                    | (SYS_IRQ << 2U)
                    | (SYS_IRQ << 3U)
                    | (SYS_IRQ << 4U)
                    | (SYS_IRQ << 5U)
                    | (SYS_IRQ << 6U)
                    | (SYS_IRQ << 7U)
                    | (SYS_IRQ << 8U)
                    | (SYS_IRQ << 9U)
                    | (SYS_IRQ << 10U)
                    | (SYS_IRQ << 11U)
                    | (SYS_IRQ << 12U)
                    | (SYS_IRQ << 13U)
                    | (SYS_IRQ << 14U)
                    | (SYS_IRQ << 15U)
                    | (SYS_IRQ << 16U)
                    | (SYS_IRQ << 17U)
                    | (SYS_IRQ << 18U)
                    | (SYS_IRQ << 19U)
                    | (SYS_IRQ << 20U)
                    | (SYS_IRQ << 21U)
                    | (SYS_IRQ << 22U)
                    | (SYS_IRQ << 23U)
                    | (SYS_IRQ << 24U)
                    | (SYS_IRQ << 25U)
                    | (SYS_IRQ << 26U)
                    | (SYS_IRQ << 27U)
                    | (SYS_IRQ << 28U)
                    | (SYS_IRQ << 29U)
                    | (SYS_IRQ << 30U)
                    | (SYS_IRQ << 31U);

    vim.FIRQPR1 = (SYS_IRQ << 0U)
                    | (SYS_IRQ << 1U)
                    | (SYS_IRQ << 2U)
                    | (SYS_IRQ << 3U)
                    | (SYS_IRQ << 4U)
                    | (SYS_IRQ << 5U)
                    | (SYS_IRQ << 6U)
                    | (SYS_IRQ << 7U)
                    | (SYS_IRQ << 8U)
                    | (SYS_IRQ << 9U)
                    | (SYS_IRQ << 10U)
                    | (SYS_IRQ << 11U)
                    | (SYS_IRQ << 12U)
                    | (SYS_IRQ << 13U)
                    | (SYS_IRQ << 14U)
                    | (SYS_IRQ << 15U)
                    | (SYS_IRQ << 16U)
                    | (SYS_IRQ << 17U)
                    | (SYS_IRQ << 18U)
                    | (SYS_IRQ << 19U)
                    | (SYS_IRQ << 20U)
                    | (SYS_IRQ << 21U)
                    | (SYS_IRQ << 22U)
                    | (SYS_IRQ << 23U)
                    | (SYS_IRQ << 24U)
                    | (SYS_IRQ << 25U)
                    | (SYS_IRQ << 26U)
                    | (SYS_IRQ << 27U)
                    | (SYS_IRQ << 28U)
                    | (SYS_IRQ << 29U)
                    | (SYS_IRQ << 30U)
                    | (SYS_IRQ << 31U);


    vim.FIRQPR2 = (SYS_IRQ << 0U)
                    | (SYS_IRQ << 1U)
                    | (SYS_IRQ << 2U)
                    | (SYS_IRQ << 3U)
                    | (SYS_IRQ << 4U)
                    | (SYS_IRQ << 5U)
                    | (SYS_IRQ << 6U)
                    | (SYS_IRQ << 7U)
                    | (SYS_IRQ << 8U)
                    | (SYS_IRQ << 9U)
                    | (SYS_IRQ << 10U)
                    | (SYS_IRQ << 11U)
                    | (SYS_IRQ << 12U)
                    | (SYS_IRQ << 13U)
                    | (SYS_IRQ << 14U)
                    | (SYS_IRQ << 15U)
                    | (SYS_IRQ << 16U)
                    | (SYS_IRQ << 17U)
                    | (SYS_IRQ << 18U)
                    | (SYS_IRQ << 19U)
                    | (SYS_IRQ << 20U)
                    | (SYS_IRQ << 21U)
                    | (SYS_IRQ << 22U)
                    | (SYS_IRQ << 23U)
                    | (SYS_IRQ << 24U)
                    | (SYS_IRQ << 25U)
                    | (SYS_IRQ << 26U)
                    | (SYS_IRQ << 27U)
                    | (SYS_IRQ << 28U)
                    | (SYS_IRQ << 29U)
                    | (SYS_IRQ << 30U)
                    | (SYS_IRQ << 31U);

    vim.FIRQPR3 = (SYS_IRQ << 0U)
                    | (SYS_IRQ << 1U)
                    | (SYS_IRQ << 2U)
                    | (SYS_IRQ << 3U)
                    | (SYS_IRQ << 4U)
                    | (SYS_IRQ << 5U)
                    | (SYS_IRQ << 6U)
                    | (SYS_IRQ << 7U)
                    | (SYS_IRQ << 8U)
                    | (SYS_IRQ << 9U)
                    | (SYS_IRQ << 10U)
                    | (SYS_IRQ << 11U)
                    | (SYS_IRQ << 12U)
                    | (SYS_IRQ << 13U)
                    | (SYS_IRQ << 14U)
                    | (SYS_IRQ << 15U)
                    | (SYS_IRQ << 16U)
                    | (SYS_IRQ << 17U)
                    | (SYS_IRQ << 18U)
                    | (SYS_IRQ << 19U)
                    | (SYS_IRQ << 20U)
                    | (SYS_IRQ << 21U)
                    | (SYS_IRQ << 22U)
                    | (SYS_IRQ << 23U)
                    | (SYS_IRQ << 24U)
                    | (SYS_IRQ << 25U)
                    | (SYS_IRQ << 26U)
                    | (SYS_IRQ << 27U)
                    | (SYS_IRQ << 28U)
                    | (SYS_IRQ << 29U)
                    | (SYS_IRQ << 30U)
                    | (SYS_IRQ << 31U);

    vim.REQMASKSET0 = (1U << 0U)
                        | (1U << 15U)
                        | (1U << 21U)
                        | (1U << 23U)
                        | (1U << 24U)
                        | (1U << 26U)
                        | (1U << 27U);

    vim.REQMASKSET1 = (1U << 6U);
    vim.REQMASKSET2 = (1U << 2U);
    vim.REQMASKSET3 = 0;
    vim.CAPEVT = 0;
}

void rti()
{
    mcal::rti::create();
}

void internalAdc1()
{
    mcal::internalAdc1::create((void *)&getAdc1(), (void *)getAdc1Ram());
}

} /* namespace init */



static void updateVectorInVimRam(size_t nIrq, void (*vector)())
{
    getVimRam()[nIrq + 1] = reinterpret_cast<uint32_t>(vector);
}

extern "C" void registerIrqVectorInVim(const int nIrq, void (*vector)())
{
    updateVectorInVimRam(nIrq, vector);
    vimEnableInterrupt(nIrq, SYS_IRQ);
}

extern "C" void registerFiqVectorInVim(const int nIrq, void (*vector)())
{
    updateVectorInVimRam(nIrq, vector);
    vimEnableInterrupt(nIrq, SYS_FIQ);
}
