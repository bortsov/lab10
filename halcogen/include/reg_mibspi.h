
#ifndef __REG_MIBSPI_H__
#define __REG_MIBSPI_H__

#include <cstdint>

#include "reg_gio.h"

struct MibspiBase
{
    uint32_t  GCR0;                 /**< 0x0000: Global Control 0 */
    uint32_t  GCR1;                 /**< 0x0004: Global Control 1 */
    uint32_t  INT0;                 /**< 0x0008: Interrupt Register */
    uint32_t  LVL;                  /**< 0x000C: Interrupt Level */
    uint32_t  FLG;                  /**< 0x0010: Interrupt flags */
    uint32_t  PC0;                /**< 0x0014: Function Pin Enable */
    uint32_t  PC1;                /**< 0x0018: Pin Direction */
    uint32_t  PC2;                /**< 0x001C: Pin Input Latch */
    uint32_t  PC3;               /**< 0x0020: Pin Output Latch */
    uint32_t  PC4;                /**< 0x0024: Output Pin Set */
    uint32_t  PC5;                /**< 0x0028: Output Pin Clr */
    uint32_t  PC6;                /**< 0x002C: Open Drain Output Enable */
    uint32_t  PC7;                /**< 0x0030: Pullup/Pulldown Disable */
    uint32_t  PC8;                /**< 0x0034: Pullup/Pulldown Selection */
    uint32_t  DAT0;                 /**< 0x0038: Transmit Data */
    uint32_t  DAT1;                 /**< 0x003C: Transmit Data with Format and Chip Select */
    uint32_t  BUF;                  /**< 0x0040: Receive Buffer */
    uint32_t  EMU;                  /**< 0x0044: Emulation Receive Buffer */
    uint32_t  DELAY;                /**< 0x0048: Delays */
    uint32_t  DEF;                /**< 0x004C: Default Chip Select */
    uint32_t  FMT0;                 /**< 0x0050: Data Format 0 */
    uint32_t  FMT1;                 /**< 0x0054: Data Format 1 */
    uint32_t  FMT2;                 /**< 0x0058: Data Format 2 */
    uint32_t  FMT3;                 /**< 0x005C: Data Format 3 */
    uint32_t  INTVECT0;             /**< 0x0060: Interrupt Vector 0 */
    uint32_t  INTVECT1;             /**< 0x0064: Interrupt Vector 1 */
    uint32_t  SRSEL;                /**< 0x0068: Slew Rate Select */
    uint32_t  PMCTRL;               /**< 0x006C: Parallel Mode Control */
    uint32_t  MIBSPIE;              /**< 0x0070: Multi-buffer Mode Enable  */
    uint32_t  TGITENST;             /**< 0x0074: TG Interrupt Enable Set */
    uint32_t  TGITENCR;             /**< 0x0078: TG Interrupt Enable Clear */
    uint32_t  TGITLVST;             /**< 0x007C: Transfer Group Interrupt Level Set */
    uint32_t  TGITLVCR;             /**< 0x0080: Transfer Group Interrupt Level Clear */
    uint32_t  TGINTFLG;             /**< 0x0084: Transfer Group Interrupt Flag */
    uint32_t    rsvd1[2U];           /**< 0x0088: Reserved */
    uint32_t  TICKCNT;              /**< 0x0090: Tick Counter */
    uint32_t  LTGPEND;              /**< 0x0090: Last TG End Pointer */
    uint32_t  TGCTRL[16U];          /**< 0x0098 - 0x00D4: Transfer Group Control */
    uint32_t  DMACTRL[8U];          /**< 0x00D8 - 0x00F4: DMA Control */
    uint32_t  DMACOUNT[8U];         /**< 0x00F8 - 0x0114: DMA Count */
    uint32_t  DMACNTLEN;            /**< 0x0118 - 0x0114: DMA Control length */
    uint32_t    rsvd2;              /**< 0x011C: Reserved */
    uint32_t  UERRCTRL;             /**< 0x0120: Multi-buffer RAM Uncorrectable Parity Error Control */
    uint32_t  UERRSTAT;             /**< 0x0124: Multi-buffer RAM Uncorrectable Parity Error Status */
    uint32_t  UERRADDRRX;           /**< 0x0128: RXRAM Uncorrectable Parity Error Address */
    uint32_t  UERRADDRTX;           /**< 0x012C: TXRAM Uncorrectable Parity Error Address */
    uint32_t  RXOVRN_BUF_ADDR;      /**< 0x0130: RXRAM Overrun Buffer Address */
    uint32_t  IOLPKTSTCR;           /**< 0x0134: IO loopback */
    uint32_t  EXT_PRESCALE1;        /**< 0x0138: */
    uint32_t  EXT_PRESCALE2;        /**< 0x013C: */
};


struct MibspiRam
{
    struct
    {
     uint16_t  data;     /**< tx buffer data    */
     uint16_t  control;  /**< tx buffer control */
    } tx[128];
    struct
    {
     uint16_t data;     /**< rx buffer data  */
	 uint16_t flags;    /**< rx buffer flags */
    } rx[128];
};


volatile MibspiRam& getMibspi1Ram();
volatile MibspiRam& getMibspi3Ram();
volatile MibspiRam& getMibspi5Ram();

volatile uint32_t* getMibspi1ParityRam();
volatile uint32_t* getMibspi3ParityRam();
volatile uint32_t* getMibspi5ParityRam();

volatile MibspiBase& getMibspi1();
volatile MibspiBase& getMibspi3();
volatile MibspiBase& getMibspi5();

volatile GioPort& getMibspi1Port();
volatile GioPort& getMibspi3Port();
volatile GioPort& getMibspi5Port();




#endif
