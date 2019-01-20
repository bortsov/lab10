#ifndef __REG_SCI_H__
#define __REG_SCI_H__

#include <cstdint>
#include "reg_gio.h"

struct SciBase {
    uint32_t GCR0;          /**< 0x0000 Global Control Register 0 */
    uint32_t GCR1;          /**< 0x0004 Global Control Register 1 */
    uint32_t GCR2;         /**< 0x0008 Global Control Register 2. Note: Applicable only to LIN � SCI Compatibility Mode,Reserved for standalone SCI*/
    uint32_t SETINT;       /**< 0x000C Set Interrupt Enable Register */
    uint32_t CLEARINT;      /**< 0x0010 Clear Interrupt Enable Register */
    uint32_t SETINTLVL;    /**< 0x0014 Set Interrupt Level Register */
    uint32_t CLEARINTLVL;   /**< 0x0018 Set Interrupt Level Register */
    uint32_t FLR;           /**< 0x001C Interrupt Flag Register */
    uint32_t INTVECT0;      /**< 0x0020 Interrupt Vector Offset 0 */
    uint32_t INTVECT1;      /**< 0x0024 Interrupt Vector Offset 1 */
    uint32_t FORMAT;        /**< 0x0028 Format Control Register */
    uint32_t BRS;           /**< 0x002C Baud Rate Selection Register */
    uint32_t ED;            /**< 0x0030 Emulation Register */
    uint32_t RD;            /**< 0x0034 Receive Data Buffer */
    uint32_t TD;            /**< 0x0038 Transmit Data Buffer */
    GioPort  port;          /**< 0x003C-0x005C GIO Port registers */
    uint32_t rsdv2[12U];    /**< 0x0060: Reserved                               */
    uint32_t IODFTCTRL;     /**< 0x0090: I/O Error Enable Register */
};

volatile SciBase& getSci();
volatile SciBase& getScilin();
volatile GioPort& getSciPort();
volatile GioPort& getScilinPort();

#endif
