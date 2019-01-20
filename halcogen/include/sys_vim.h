#ifndef __SYS_VIM_H__
#define __SYS_VIM_H__

#include <cstddef>
#include <cstdint>


enum SYSTEM_INTERRUPT
{
    SYS_IRQ = 0U, /**< Alias for IRQ interrupt */
    SYS_FIQ = 1U  /**< Alias for FIQ interrupt */
};

constexpr size_t VIM_CHANNELS = 96;


extern "C" void vimEnableInterrupt(uint32_t channel, SYSTEM_INTERRUPT inttype);
extern "C" void vimDisableInterrupt(uint32_t channel);
extern "C" bool vimGetFlagInterruptEnabled(uint32_t channel);


volatile uint32_t& getVimParityFlag();
volatile uint32_t& getVimParityControl();
volatile uint32_t& getVimAdder();
volatile uint32_t& getVimFbparerr();

volatile uint32_t * getVimRamParity();
volatile uint32_t * getVimRam();


#endif
