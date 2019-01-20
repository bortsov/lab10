#ifndef __REG_DMA_H__
#define __REG_DMA_H__

#include <cstdint>


/* DMA Register Frame Definition */
/** @struct dmaBase
*   @brief DMA Register Frame Definition
*
*   This type is used to access the DMA Registers.
*/
/** @struct dmaBASE_t
*   @brief DMA Register Definition
*
*   This structure is used to access the DMA module egisters.
*/
struct DmaBase
{

    uint32_t GCTRL;       /**<  0x0000: Global Control Register                */
    uint32_t PEND;        /**<  0x0004: Channel Pending Register               */
    uint32_t FBREG;       /**<  0x0008: Fall Back Register                     */
    uint32_t DMASTAT;     /**<  0x000C: Status Register                        */
    uint32_t  rsvd1;      /**<  0x0010: Reserved                               */
    uint32_t HWCHENAS;    /**<  0x0014: HW Channel Enable Set                  */
    uint32_t  rsvd2;      /**<  0x0018: Reserved                               */
    uint32_t HWCHENAR;    /**<  0x001C: HW Channel Enable Reset                */
    uint32_t  rsvd3;      /**<  0x0020: Reserved                               */
    uint32_t SWCHENAS;    /**<  0x0024: SW Channel Enable Set                  */
    uint32_t  rsvd4;      /**<  0x0028: Reserved                               */
    uint32_t SWCHENAR;    /**<  0x002C: SW Channel Enable Reset                */
    uint32_t  rsvd5;      /**<  0x0030: Reserved                               */
    uint32_t CHPRIOS;     /**<  0x0034: Channel Priority Set                   */
    uint32_t  rsvd6;      /**<  0x0038: Reserved                               */
    uint32_t CHPRIOR;     /**<  0x003C: Channel Priority Reset                 */
    uint32_t  rsvd7;      /**<  0x0040: Reserved                               */
    uint32_t GCHIENAS;    /**<  0x0044: Global Channel Interrupt Enable Set    */
    uint32_t  rsvd8;      /**<  0x0048: Reserved                               */
    uint32_t GCHIENAR;    /**<  0x004C: Global Channel Interrupt Enable Reset  */
    uint32_t  rsvd9;      /**<  0x0050: Reserved                               */
    uint32_t DREQASI[8U]; /**<  0x0054 - 0x70: DMA Request Assignment Register */
    uint32_t  rsvd10[8U]; /**<  0x0074 - 0x90: Reserved                        */
    uint32_t PAR[4U];     /**<  0x0094 - 0xA0: Port Assignment Register        */
    uint32_t  rsvd11[4U]; /**<  0x00A4 - 0xB0: Reserved                        */
    uint32_t FTCMAP;      /**<  0x00B4: FTC Interrupt Mapping Register         */
    uint32_t  rsvd12;     /**<  0x00B8: Reserved                               */
    uint32_t LFSMAP;      /**<  0x00BC: LFS Interrupt Mapping Register         */
    uint32_t  rsvd13;     /**<  0x00C0: Reserved                               */
    uint32_t HBCMAP;      /**<  0x00C4: HBC Interrupt Mapping Register         */
    uint32_t  rsvd14;     /**<  0x00C8: Reserved                               */
    uint32_t BTCMAP;      /**<  0x00CC: BTC Interrupt Mapping Register         */
    uint32_t  rsvd15;     /**<  0x00D0: Reserved                               */
    uint32_t BERMAP;      /**<  0x00D4: BER Interrupt Mapping Register         */
    uint32_t  rsvd16;     /**<  0x00D8: Reserved                               */
    uint32_t FTCINTENAS;  /**<  0x00DC: FTC Interrupt Enable Set               */
    uint32_t  rsvd17;     /**<  0x00E0: Reserved                               */
    uint32_t FTCINTENAR;  /**<  0x00E4: FTC Interrupt Enable Reset             */
    uint32_t  rsvd18;     /**<  0x00E8: Reserved                               */
    uint32_t LFSINTENAS;  /**<  0x00EC: LFS Interrupt Enable Set               */
    uint32_t  rsvd19;     /**<  0x00F0: Reserved                               */
    uint32_t LFSINTENAR;  /**<  0x00F4: LFS Interrupt Enable Reset             */
    uint32_t  rsvd20;     /**<  0x00F8: Reserved                               */
    uint32_t HBCINTENAS;  /**<  0x00FC: HBC Interrupt Enable Set               */
    uint32_t  rsvd21;     /**<  0x0100: Reserved                               */
    uint32_t HBCINTENAR;  /**<  0x0104: HBC Interrupt Enable Reset             */
    uint32_t  rsvd22;     /**<  0x0108: Reserved                               */
    uint32_t BTCINTENAS;  /**<  0x010C: BTC Interrupt Enable Set               */
    uint32_t  rsvd23;     /**<  0x0110: Reserved                               */
    uint32_t BTCINTENAR;  /**<  0x0114: BTC Interrupt Enable Reset             */
    uint32_t  rsvd24;     /**<  0x0118: Reserved                               */
    uint32_t GINTFLAG;    /**<  0x011C: Global Interrupt Flag Register         */
    uint32_t  rsvd25;     /**<  0x0120: Reserved                               */
    uint32_t FTCFLAG;     /**<  0x0124: FTC Interrupt Flag Register            */
    uint32_t  rsvd26;     /**<  0x0128: Reserved                               */
    uint32_t LFSFLAG;     /**<  0x012C: LFS Interrupt Flag Register            */
    uint32_t  rsvd27;     /**<  0x0130: Reserved                               */
    uint32_t HBCFLAG;     /**<  0x0134: HBC Interrupt Flag Register            */
    uint32_t  rsvd28;     /**<  0x0138: Reserved                               */
    uint32_t BTCFLAG;     /**<  0x013C: BTC Interrupt Flag Register            */
    uint32_t  rsvd29;     /**<  0x0140: Reserved                               */
    uint32_t BERFLAG;     /**<  0x0144: BER Interrupt Flag Register            */
    uint32_t  rsvd30;     /**<  0x0148: Reserved                               */
    uint32_t FTCAOFFSET;  /**<  0x014C: FTCA Interrupt Channel Offset Register */
    uint32_t LFSAOFFSET;  /**<  0x0150: LFSA Interrupt Channel Offset Register */
    uint32_t HBCAOFFSET;  /**<  0x0154: HBCA Interrupt Channel Offset Register */
    uint32_t BTCAOFFSET;  /**<  0x0158: BTCA Interrupt Channel Offset Register */
    uint32_t BERAOFFSET;  /**<  0x015C: BERA Interrupt Channel Offset Register */
    uint32_t FTCBOFFSET;  /**<  0x0160: FTCB Interrupt Channel Offset Register */
    uint32_t LFSBOFFSET;  /**<  0x0164: LFSB Interrupt Channel Offset Register */
    uint32_t HBCBOFFSET;  /**<  0x0168: HBCB Interrupt Channel Offset Register */
    uint32_t BTCBOFFSET;  /**<  0x016C: BTCB Interrupt Channel Offset Register */
    uint32_t BERBOFFSET;  /**<  0x0170: BERB Interrupt Channel Offset Register */
    uint32_t  rsvd31;     /**<  0x0174: Reserved                               */
    uint32_t PTCRL;       /**<  0x0178: Port Control Register                  */
    uint32_t RTCTRL;      /**<  0x017C: RAM Test Control Register              */
    uint32_t DCTRL;       /**<  0x0180: Debug Control                          */
    uint32_t WPR;         /**<  0x0184: Watch Point Register                   */
    uint32_t WMR;         /**<  0x0188: Watch Mask Register                    */
    uint32_t PAACSADDR;   /**<  0x018C:           */
    uint32_t PAACDADDR;   /**<  0x0190:           */
    uint32_t PAACTC;      /**<  0x0194:           */
    uint32_t PBACSADDR;   /**<  0x0198: Port B Active Channel Source Address Register       */
    uint32_t PBACDADDR;   /**<  0x019C: Port B Active Channel Destination Address Register  */
    uint32_t PBACTC;      /**<  0x01A0: Port B Active Channel Transfer Count Register       */
    uint32_t  rsvd32;     /**<  0x01A4: Reserved                               */
    uint32_t DMAPCR;      /**<  0x01A8: Parity Control Register                */
    uint32_t DMAPAR;      /**<  0x01AC: DMA Parity Error Address Register      */
    uint32_t DMAMPCTRL;   /**<  0x01B0: DMA Memory Protection Control Register */
    uint32_t DMAMPST;     /**<  0x01B4: DMA Memory Protection Status Register  */
    struct
    {
    	uint32_t STARTADD; /**<  0x01B8, 0x01C0, 0x01C8, 0x1D0: DMA Memory Protection Region Start Address Register  */
    	uint32_t ENDADD;   /**<  0x01B8, 0x01C0, 0x01C8, 0x1D0: DMA Memory Protection Region Start Address Register  */
    } DMAMPR[4U];
};


struct DmaRamBase
{

     struct                                     /* 0x000-0x400 */
     {
      uint32_t ISADDR;
      uint32_t IDADDR;
      uint32_t ITCOUNT;
      uint32_t  rsvd1;
      uint32_t CHCTRL;
      uint32_t EIOFF;
      uint32_t FIOFF;
      uint32_t  rsvd2;
     }PCP[32U];

     struct                                     /* 0x400-0x800   */
     {
       uint32_t res[256U];
     } RESERVED;

     struct                                     /* 0x800-0xA00 */
     {
      uint32_t CSADDR;
      uint32_t CDADDR;
      uint32_t CTCOUNT;
      uint32_t  rsvd3;
     }WCP[32U];
};

volatile DmaBase& getDmaBase();
volatile DmaRamBase& getDmaRamBase();

#endif
