/*
 * irq.h
 *
 *  Created on: 23 сент. 2014 г.
 *      Author: bortsov
 */

#ifndef IRQ_H_
#define IRQ_H_

#include <cstdint>

namespace irq {

enum IRQNUMBER {
    IRQ_RTI_COMPARE_0     = 2,
    IRQ_RTI_COMPARE_1     = 3,
    IRQ_RTI_COMPARE_2     = 4,
    IRQ_RTI_COMPARE_3     = 5,
    IRQ_RTI_OVERFLOW_0    = 6,
    IRQ_RTI_OVERFLOW_1    = 7,
    IRQ_RTI_TIMEBASE      = 8,
    IRQ_LIN_LEVEL_0       = 13,
    IRQ_ADC1_GROUP_1      = 15,
    IRQ_CAN1_LEVEL_0      = 16,
    IRQ_HET1_LEVEL_2      = 24,
    IRQ_MIBSPI1_LEVEL_1   = 26,
    IRQ_LIN_LEVEL_1       = 27,
    IRQ_CAN1_LEVEL_1      = 29,
    IRQ_CAN2_LEVEL_0      = 35,
    IRQ_MIBSPI3_LEVEL_1   = 38,
    IRQ_DMA_BTCA          = 40,
    IRQ_CAN2_LEVEL_1      = 42,
    IRQ_CAN1_IF3          = 44,
    IRQ_CAN3_LEVEL_0      = 45,
    IRQ_CAN2_IF3          = 46,
    IRQ_MIBADC2_EVENT     = 50,
    IRQ_MIBADC2_GROUP_1   = 51,
    IRQ_CAN3_LEVEL_1      = 55,
    IRQ_MIBSPI5_LEVEL_1   = 56,
    IRQ_MIBADC2_GROUP_2   = 57,
    IRQ_CAN3_IF3          = 60,
    IRQ_I2C               = 66,
    IRQ_HET2_LEVEL_2      = 73,
};

enum IRQTYPE {IRQ = 0, FIQ = 1};

class IrqCapable {
    const IRQNUMBER irqNumber;
    const IRQTYPE irqType;
public:
    IrqCapable(IRQNUMBER irqnumber, IRQTYPE irqtype);
    IRQNUMBER getIrqNumber() const;
    IRQTYPE getIrqType() const;
    virtual bool handlerIrq() = 0;
    virtual ~IrqCapable();
};


struct VfpGuard {
private:
    uint32_t regs[16 * 2 + 1]; /* контекст VFP: 16 регистров двойной точности (по 2 слова каждый) и один регистр статуса (размером 1 слово) */
public:
    VfpGuard();
    ~VfpGuard();
};

struct IrqGuard {
private:
    const int CPSR;
public:
    IrqGuard();
    ~IrqGuard();
};


class IrqDisabler {
    const IrqCapable& object;
    const bool flagEnabled;
public:
    IrqDisabler(const IrqCapable& object);
    ~IrqDisabler();
};

namespace test {
void emulateIrq(IRQNUMBER n);
} /* namespace test */

} /* namespace irq */

extern "C" void vPortSaveVfp(irq::VfpGuard*);
extern "C" void vPortRestoreVfp(irq::VfpGuard*);


#endif /* IRQ_H_ */
