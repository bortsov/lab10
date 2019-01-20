#include "sys_vim.h"
#include "reg_vim.h"
#include "system.h"


static volatile uint32_t * const VIM_PARFLG   = reinterpret_cast<volatile uint32_t*>(0xFFFFFDECU);
static volatile uint32_t * const VIM_PARCTL   = reinterpret_cast<volatile uint32_t*>(0xFFFFFDF0U);
static volatile uint32_t * const VIM_ADDERR   = reinterpret_cast<volatile uint32_t*>(0xFFFFFDF4U);
static volatile uint32_t * const VIM_FBPARERR = reinterpret_cast<volatile uint32_t*>(0xFFFFFDF8U);

static volatile uint32_t * const VIMRAMPARLOC = reinterpret_cast<volatile uint32_t*>(0xFFF82400U);
static volatile uint32_t * const VIMRAMLOC    = reinterpret_cast<volatile uint32_t*>(0xFFF82000U);

volatile uint32_t& getVimParityFlag() {return *VIM_PARFLG;}
volatile uint32_t& getVimParityControl() {return *VIM_PARCTL;}
volatile uint32_t& getVimAdder()  {return *VIM_ADDERR;}
volatile uint32_t& getVimFbparerr() {return *VIM_FBPARERR;}

volatile uint32_t * getVimRamParity() {return VIMRAMPARLOC;}
volatile uint32_t * getVimRam() {return VIMRAMLOC;}




/** @fn void vimEnableInterrupt(uint32_t channel, SYSTEM_INTERRUPT inttype)
*   @brief Enable interrupt for the the selected channel
*
*    @param[in] channel: VIM Channel number 2..127
*    @param[in] inttype: Interrupt type
*                        - SYS_IRQ: Selected channel will be enabled as IRQ
*                        - SYS_FIQ: Selected channel will be enabled as FIQ
*
*   This function will enable interrupt for the selected channel.
*
*/
void vimEnableInterrupt(uint32_t channel, SYSTEM_INTERRUPT inttype)
{
	auto& vim = getVim();
    if (channel >= 96U) {
        if (inttype == SYS_IRQ) {
            vim.FIRQPR3 &= ~(1U << (channel-96U));
        } else {
            vim.FIRQPR3 |= (1U << (channel-96U));
        }
        vim.REQMASKSET3 = 1U << (channel-96U);
    } else if (channel >= 64U) {
        if (inttype == SYS_IRQ) {
            vim.FIRQPR2 &= ~(1U << (channel-64U));
        } else {
            vim.FIRQPR2 |= (1U << (channel-64U));
        }
        vim.REQMASKSET2 = 1U << (channel-64U);
    } else if (channel >= 32U) {
        if (inttype == SYS_IRQ) {
            vim.FIRQPR1 &= ~(1U << (channel-32U));
        } else {
            vim.FIRQPR1 |= (1U << (channel-32U));
        }
        vim.REQMASKSET1 = 1U << (channel-32U);
    } else if (channel >= 2U) {
        if (inttype == SYS_IRQ) {
            vim.FIRQPR0 &= ~(1U << channel);
        } else {
            vim.FIRQPR0 |= (1U << channel);
        }
        vim.REQMASKSET0 = 1U << channel;
    } else {
		/* Empty */
    }
}

/** @fn void vimDisableInterrupt(uint32_t channel)
*   @brief Disable interrupt for the the selected channel
*
*    @param[in] channel: VIM Channel number 2..127
*
*   This function will disable interrupt for the selected channel.
*
*/
void vimDisableInterrupt(uint32_t channel)
{
    auto& vim = getVim();
    if (channel >= 96U) {
        vim.REQMASKCLR3 = 1U << (channel-96U);
    } else if (channel >= 64U) {
        vim.REQMASKCLR2 = 1U << (channel-64U);
    } else if (channel >=32U) {
        vim.REQMASKCLR1 = 1U << (channel-32U);
    } else if (channel >= 2U) {
        vim.REQMASKCLR0 = 1U << channel;
    } else {
		/* Empty */
    }
}


bool vimGetFlagInterruptEnabled(uint32_t channel)
{
    auto& vim = getVim();
    if (channel >= 96U) {
        return vim.REQMASKSET3 & (1 << (channel - 96));
    } else if (channel >= 64U) {
        return vim.REQMASKSET2 & (1 << (channel - 64));
    } else if (channel >=32U) {
        return vim.REQMASKCLR1 & (1 << (channel - 32));
    } else if (channel >= 2U) {
        return vim.REQMASKCLR0 & (1 << channel);
    } else {
        return false;
    }
}

