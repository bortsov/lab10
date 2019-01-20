#ifndef __REG_GIO_H__
#define __REG_GIO_H__

#include <cstdint>

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
    uint32_t EMU1;      /**< 0x002C: Emulation 1 Register */
	uint32_t EMU2;      /**< 0x0030: Emulation 2 Register */
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


volatile GioBase& getGio();
volatile GioPort& getPortA();
volatile GioPort& getPortB();



#endif
