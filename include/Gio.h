/*
 * gio_.h
 *
 *  Created on: 24 сент. 2014 г.
 *      Author: bortsov
 */

#ifndef GIO__H_
#define GIO__H_

#include <cstddef>
#include "irq.h"


namespace gio {

enum GIO_PORT {PORTA, PORTB};

struct GioBase {
    uint32_t GCR0;      /**< 0x0000: Global Control Register */
    uint32_t   rsvd;    /**< 0x0004: Reserved*/
    uint32_t INTDET;    /**< 0x0008: Interrupt Detect Register*/
    uint32_t POL;       /**< 0x000C: Interrupt Polarity Register */
    uint32_t ENASET;    /**< 0x0010: Interrupt Enable Set Register */
    uint32_t ENACLR;    /**< 0x0014: Interrupt Enable Clear Register */
    uint32_t LVLSET;    /**< 0x0018: Interrupt Priority Set Register */
    uint32_t LVLCLR;    /**< 0x001C: Interrupt Priority Clear Register */
    uint32_t FLG;       /**< 0x0020: Interrupt Flag Register */
    uint32_t OFF1;      /**< 0x0024: Interrupt Offset A Register */
    uint32_t OFF2;      /**< 0x0028: Interrupt Offset B Register */
};


struct GioPort {
    uint32_t DIR;    /**< 0x0000: Data Direction Register */
    uint32_t DIN;    /**< 0x0004: Data Input Register */
    uint32_t DOUT;   /**< 0x0008: Data Output Register */
    uint32_t DSET;   /**< 0x000C: Data Output Set Register */
    uint32_t DCLR;   /**< 0x0010: Data Output Clear Register */
    uint32_t PDR;    /**< 0x0014: Open Drain Register */
    uint32_t PULDIS; /**< 0x0018: Pullup Disable Register */
    uint32_t PSL;    /**< 0x001C: Pull Up/Down Selection Register */
};


class UserIrqCapable {
public:
    virtual void handlerGioIrq(const size_t bits)=0;
    virtual ~UserIrqCapable(){};
};

class Gio : private ::irq::IrqCapable {
    void callIrqHandler(uint32_t off) const;
    void init();
public:
    volatile GioBase& base;
    volatile GioPort& porta;
    volatile GioPort& portb;
    virtual bool handlerIrq();
    static const size_t MAX_NUMBER_IRQ_HANDLERS = 2;
    struct IrqHandler {
        UserIrqCapable* irqHandler;
        GIO_PORT port;
        size_t   bit;
    } irqHandlerArray[MAX_NUMBER_IRQ_HANDLERS];
    Gio(volatile GioBase* base, volatile GioPort* porta, volatile GioPort* portb);
    void registerIrqHanlder(UserIrqCapable* handler, GIO_PORT port, const size_t bit);
    void enableAllIrq();
    virtual ~Gio();
};


void create(volatile GioBase* base, volatile GioPort* gioa, volatile GioPort* giob);
void destroy();
Gio &getGio();

} /* namespace gio */



#endif /* GIO__H_ */
