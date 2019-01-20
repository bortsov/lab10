/*
 * irq.cpp
 *
 *  Created on: 23 сент. 2014 г.
 *      Author: bortsov
 */

#include "irq.h"
#include "sys_core.h"
#include "sys_vim.h"
#include "init.h"
#include "userExitCodes.h"


extern "C" void vPortSaveVfp(irq::VfpGuard*);
extern "C" void vPortRestoreVfp(irq::VfpGuard*);

namespace irq {

static IrqCapable* registeredObjects[VIM_CHANNELS] = { nullptr };

static bool callIsr(IRQNUMBER n) {return registeredObjects[n]->handlerIrq();}

static __attribute__((target("arm"), interrupt("IRQ"))) void rtiCompare0Interrupt()  {callIsr(IRQ_RTI_COMPARE_0);}
static __attribute__((target("arm"), interrupt("IRQ"))) void rtiCompare1Interrupt()  {callIsr(IRQ_RTI_COMPARE_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void rtiCompare2Interrupt()  {callIsr(IRQ_RTI_COMPARE_2);}
static __attribute__((target("arm"), interrupt("IRQ"))) void rtiCompare3Interrupt()  {callIsr(IRQ_RTI_COMPARE_3);}
static __attribute__((target("arm"), interrupt("IRQ"))) void rtiOverflow0Interrupt() {callIsr(IRQ_RTI_OVERFLOW_0);}
static __attribute__((target("arm"), interrupt("IRQ"))) void rtiOverflow1Interrupt() {callIsr(IRQ_RTI_OVERFLOW_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void rtiTimebaseInterrupt()  {callIsr(IRQ_RTI_TIMEBASE);}
static __attribute__((target("arm"), interrupt("IRQ"))) void linLevel0Interrupt()    {callIsr(IRQ_LIN_LEVEL_0);}
static __attribute__((target("arm"), interrupt("IRQ"))) void adc1Group1Interrupt()   {callIsr(IRQ_ADC1_GROUP_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void adc2EventInterrupt()    {callIsr(IRQ_MIBADC2_EVENT);}
static __attribute__((target("arm"), interrupt("IRQ"))) void adc2Group1Interrupt()   {callIsr(IRQ_MIBADC2_GROUP_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void adc2Group2Interrupt()   {callIsr(IRQ_MIBADC2_GROUP_2);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can1Level0Interrupt()   {callIsr(IRQ_CAN1_LEVEL_0);}
static __attribute__((target("arm"), interrupt("IRQ"))) void het1Level2Interrupt()   {callIsr(IRQ_HET1_LEVEL_2);}
static __attribute__((target("arm"), interrupt("IRQ"))) void mibspi1Level1Interrupt(){callIsr(IRQ_MIBSPI1_LEVEL_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void linLevel1Interrupt()    {callIsr(IRQ_LIN_LEVEL_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can1Level1Interrupt()   {callIsr(IRQ_CAN1_LEVEL_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can2Level0Interrupt()   {callIsr(IRQ_CAN2_LEVEL_0);}
static __attribute__((target("arm"), interrupt("IRQ"))) void mibspi3Level1Interrupt(){callIsr(IRQ_MIBSPI3_LEVEL_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can2Level1Interrupt()   {callIsr(IRQ_CAN2_LEVEL_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can1IF3Interrupt()      {callIsr(IRQ_CAN1_IF3);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can3Level0Interrupt()   {callIsr(IRQ_CAN3_LEVEL_0);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can2IF3Interrupt()      {callIsr(IRQ_CAN2_IF3);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can3Level1Interrupt()   {callIsr(IRQ_CAN3_LEVEL_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void mibspi5Level1Interrupt(){callIsr(IRQ_MIBSPI5_LEVEL_1);}
static __attribute__((target("arm"), interrupt("IRQ"))) void can3IF3Interrupt()      {callIsr(IRQ_CAN3_IF3);}
static __attribute__((target("arm"), interrupt("IRQ"))) void i2cInterrupt()          {callIsr(IRQ_I2C);}
static __attribute__((target("arm"), interrupt("IRQ"))) void het2Level2Interrupt()   {callIsr(IRQ_HET2_LEVEL_2);}

using Function = void (*)();

static Function getVectorFromIrqNumber(IRQNUMBER n)
{
    switch (n) {
        case IRQ_RTI_COMPARE_0:   return reinterpret_cast<Function>(rtiCompare0Interrupt);
        case IRQ_RTI_COMPARE_1:   return reinterpret_cast<Function>(rtiCompare1Interrupt);
        case IRQ_RTI_COMPARE_2:   return reinterpret_cast<Function>(rtiCompare2Interrupt);
        case IRQ_RTI_COMPARE_3:   return reinterpret_cast<Function>(rtiCompare3Interrupt);
        case IRQ_RTI_OVERFLOW_0:  return reinterpret_cast<Function>(rtiOverflow0Interrupt);
        case IRQ_RTI_OVERFLOW_1:  return reinterpret_cast<Function>(rtiOverflow1Interrupt);
        case IRQ_RTI_TIMEBASE:    return reinterpret_cast<Function>(rtiTimebaseInterrupt);
        case IRQ_LIN_LEVEL_0:     return reinterpret_cast<Function>(linLevel0Interrupt);
        case IRQ_ADC1_GROUP_1:    return reinterpret_cast<Function>(adc1Group1Interrupt);
        case IRQ_CAN1_LEVEL_0:    return reinterpret_cast<Function>(can1Level0Interrupt);
        case IRQ_HET1_LEVEL_2:    return reinterpret_cast<Function>(het1Level2Interrupt);
        case IRQ_MIBSPI1_LEVEL_1: return reinterpret_cast<Function>(mibspi1Level1Interrupt);
        case IRQ_LIN_LEVEL_1:     return reinterpret_cast<Function>(linLevel1Interrupt);
        case IRQ_CAN1_LEVEL_1:    return reinterpret_cast<Function>(can1Level1Interrupt);
        case IRQ_CAN2_LEVEL_0:    return reinterpret_cast<Function>(can2Level0Interrupt);
        case IRQ_MIBSPI3_LEVEL_1: return reinterpret_cast<Function>(mibspi3Level1Interrupt);
        case IRQ_CAN2_LEVEL_1:    return reinterpret_cast<Function>(can2Level1Interrupt);
        case IRQ_CAN1_IF3:        return reinterpret_cast<Function>(can1IF3Interrupt);
        case IRQ_CAN3_LEVEL_0:    return reinterpret_cast<Function>(can3Level0Interrupt);
        case IRQ_CAN2_IF3:        return reinterpret_cast<Function>(can2IF3Interrupt);
        case IRQ_MIBADC2_EVENT:   return reinterpret_cast<Function>(adc2EventInterrupt);
        case IRQ_MIBADC2_GROUP_1: return reinterpret_cast<Function>(adc2Group1Interrupt);
        case IRQ_CAN3_LEVEL_1:    return reinterpret_cast<Function>(can3Level1Interrupt);
        case IRQ_MIBSPI5_LEVEL_1: return reinterpret_cast<Function>(mibspi5Level1Interrupt);
        case IRQ_MIBADC2_GROUP_2: return reinterpret_cast<Function>(adc2Group2Interrupt);
        case IRQ_CAN3_IF3:        return reinterpret_cast<Function>(can3IF3Interrupt);
        case IRQ_I2C:             return reinterpret_cast<Function>(i2cInterrupt);
        case IRQ_HET2_LEVEL_2:    return reinterpret_cast<Function>(het2Level2Interrupt);
        default:
            exit(EXIT_VIM_INVALID_IRQ_NUMBER);
    }
}

IrqCapable::IrqCapable(IRQNUMBER irqNumber, IRQTYPE irqtype) :
        irqNumber(irqNumber), irqType(irqtype)
{
    if (registeredObjects[irqNumber]) {
        exit(EXIT_VIM_IRQ_IS_ALREADY_REGISTERED);
    }
    registeredObjects[irqNumber] = this;
    auto handler = (void (*)())getVectorFromIrqNumber(irqNumber);
    if (IRQ == irqtype) {
        registerIrqVectorInVim(irqNumber, handler);
    } else {
        registerFiqVectorInVim(irqNumber, handler);
    }
}

IRQNUMBER IrqCapable::getIrqNumber() const
{
    return irqNumber;
}

IRQTYPE IrqCapable::getIrqType() const
{
    return irqType;
}

IrqCapable::~IrqCapable()
{
    size_t idx = 0;
    while (registeredObjects[idx] != this) {
        ++idx;
    }
    registeredObjects[idx] = nullptr;
}

IrqDisabler::IrqDisabler(const IrqCapable& object) :
                object(object),
                flagEnabled(vimGetFlagInterruptEnabled(object.getIrqNumber()))
{
    if (flagEnabled) {
        vimDisableInterrupt(object.getIrqNumber());
    }
}

IrqDisabler::~IrqDisabler()
{
    if (flagEnabled) {
        vimEnableInterrupt(
                object.getIrqNumber(),
                (IRQ == object.getIrqType()) ? SYS_IRQ : SYS_FIQ);
    }
}

VfpGuard::VfpGuard()
{
    vPortSaveVfp(this);
}
VfpGuard::~VfpGuard()
{
    vPortRestoreVfp(this);
}

IrqGuard::IrqGuard() :
        CPSR(_getCpsrValue_())
{
    _disableIrqInterrupt_();
}
IrqGuard::~IrqGuard()
{
    if (!(CPSR & (1 << 7)))
        _enableIrqInterrupt_();
}

namespace test {
void emulateIrq(IRQNUMBER n)
{
    registeredObjects[n]->handlerIrq();
}
} /* namespace test */

} /* namespace irq */

