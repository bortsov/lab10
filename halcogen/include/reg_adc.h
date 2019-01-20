
#ifndef __REG_ADC_H__
#define __REG_ADC_H__

#include <cstdint>

struct AdcBase
{
    uint32_t RSTCR;              /**< 0x0000: Reset control register                            */
    uint32_t OPMODECR;           /**< 0x0004: Operating mode control register                   */
    uint32_t CLOCKCR;            /**< 0x0008: Clock control register                            */
    uint32_t CALCR;              /**< 0x000C: Calibration control register                      */
    uint32_t GxMODECR[3U];       /**< 0x0010,0x0014,0x0018: Group 0-2 mode control register     */
    uint32_t EVSRC;              /**< 0x001C: Group 0 trigger source control register           */
    uint32_t G1SRC;              /**< 0x0020: Group 1 trigger source control register           */
    uint32_t G2SRC;              /**< 0x0024: Group 2 trigger source control register           */
    uint32_t GxINTENA[3U];       /**< 0x0028,0x002C,0x0030: Group 0-2 interrupt enable register */
    uint32_t GxINTFLG[3U];       /**< 0x0034,0x0038,0x003C: Group 0-2 interrupt flag register   */
    uint32_t GxINTCR[3U];        /**< 0x0040-0x0048: Group 0-2 interrupt threshold register     */
    uint32_t EVDMACR;            /**< 0x004C: Group 0 DMA control register                      */
    uint32_t G1DMACR;            /**< 0x0050: Group 1 DMA control register                      */
    uint32_t G2DMACR;            /**< 0x0054: Group 2 DMA control register                      */
    uint32_t BNDCR;              /**< 0x0058: Buffer boundary control register                  */
    uint32_t BNDEND;             /**< 0x005C: Buffer boundary end register                      */
    uint32_t EVSAMP;             /**< 0x0060: Group 0 sample window register                    */
    uint32_t G1SAMP;             /**< 0x0064: Group 1 sample window register                    */
    uint32_t G2SAMP;             /**< 0x0068: Group 2 sample window register                    */
    uint32_t EVSR;               /**< 0x006C: Group 0 status register                           */
    uint32_t G1SR;               /**< 0x0070: Group 1 status register                           */
    uint32_t G2SR;               /**< 0x0074: Group 2 status register                           */
    uint32_t GxSEL[3U];          /**< 0x0078-0x007C: Group 0-2 channel select register          */
    uint32_t CALR;               /**< 0x0084: Calibration register                              */
    uint32_t SMSTATE;            /**< 0x0088: State machine state register                      */
    uint32_t LASTCONV;           /**< 0x008C: Last conversion register                          */
    struct
    {
        uint32_t BUF0;           /**< 0x0090,0x00B0,0x00D0: Group 0-2 result buffer 1 register  */
        uint32_t BUF1;           /**< 0x0094,0x00B4,0x00D4: Group 0-2 result buffer 1 register  */
        uint32_t BUF2;           /**< 0x0098,0x00B8,0x00D8: Group 0-2 result buffer 2 register  */
        uint32_t BUF3;           /**< 0x009C,0x00BC,0x00DC: Group 0-2 result buffer 3 register  */
        uint32_t BUF4;           /**< 0x00A0,0x00C0,0x00E0: Group 0-2 result buffer 4 register  */
        uint32_t BUF5;           /**< 0x00A4,0x00C4,0x00E4: Group 0-2 result buffer 5 register  */
        uint32_t BUF6;           /**< 0x00A8,0x00C8,0x00E8: Group 0-2 result buffer 6 register  */
        uint32_t BUF7;           /**< 0x00AC,0x00CC,0x00EC: Group 0-2 result buffer 7 register  */
    } GxBUF[3U];
    uint32_t EVEMUBUFFER;        /**< 0x00F0: Group 0 emulation result buffer                   */
    uint32_t G1EMUBUFFER;        /**< 0x00F4: Group 1 emulation result buffer                   */
    uint32_t G2EMUBUFFER;        /**< 0x00F8: Group 2 emulation result buffer                   */
    uint32_t EVTDIR;             /**< 0x00FC: Event pin direction register                      */
    uint32_t EVTOUT;             /**< 0x0100: Event pin digital output register                 */
    uint32_t EVTIN;              /**< 0x0104: Event pin digital input register                  */
    uint32_t EVTSET;             /**< 0x0108: Event pin set register                            */
    uint32_t EVTCLR;             /**< 0x010C: Event pin clear register                          */
    uint32_t EVTPDR;             /**< 0x0110: Event pin open drain register                     */
    uint32_t EVTDIS;             /**< 0x0114: Event pin pull disable register                   */
    uint32_t EVTPSEL;            /**< 0x0118: Event pin pull select register                    */
    uint32_t EVSAMPDISEN;        /**< 0x011C: Group 0 sample discharge register                 */
    uint32_t G1SAMPDISEN;        /**< 0x0120: Group 1 sample discharge register                 */
    uint32_t G2SAMPDISEN;        /**< 0x0124: Group 2 sample discharge register                 */
    uint32_t MAGINTCR1;          /**< 0x0128: Magnitude interrupt control register 1            */
    uint32_t MAGINT1MASK;        /**< 0x012C: Magnitude interrupt mask register 1               */
    uint32_t MAGINTCR2;          /**< 0x0130: Magnitude interrupt control register 2            */
    uint32_t MAGINT2MASK;        /**< 0x0134: Magnitude interrupt mask register 2               */
    uint32_t MAGINTCR3;          /**< 0x0138: Magnitude interrupt control register 3            */
    uint32_t MAGINT3MASK;        /**< 0x013C: Magnitude interrupt mask register 3               */
    uint32_t   rsvd1;            /**< 0x0140: Reserved                                          */
    uint32_t   rsvd2;            /**< 0x0144: Reserved                                          */
    uint32_t   rsvd3;            /**< 0x0148: Reserved                                          */
    uint32_t   rsvd4;            /**< 0x014C: Reserved                                          */
    uint32_t   rsvd5;            /**< 0x0150: Reserved                                          */
    uint32_t   rsvd6;            /**< 0x0154: Reserved                                          */
    uint32_t MAGTHRINTENASET;    /**< 0x0158: Magnitude interrupt set register                  */
    uint32_t MAGTHRINTENACLR;    /**< 0x015C: Magnitude interrupt clear register                */
    uint32_t MAGTHRINTFLG;       /**< 0x0160: Magnitude interrupt flag register                 */
    uint32_t MAGTHRINTOFFSET;    /**< 0x0164: Magnitude interrupt offset register               */
    uint32_t GxFIFORESETCR[3U];  /**< 0x0168,0x016C,0x0170: Group 0-2 fifo reset register       */
    uint32_t EVRAMADDR;          /**< 0x0174: Group 0 RAM pointer register                      */
    uint32_t G1RAMADDR;          /**< 0x0178: Group 1 RAM pointer register                      */
    uint32_t G2RAMADDR;          /**< 0x017C: Group 2 RAM pointer register                      */
    uint32_t PARCR;              /**< 0x0180: Parity control register                           */
    uint32_t PARADDR;            /**< 0x0184: Parity error address register                     */
    uint32_t PWRUPDLYCTRL;       /**< 0x0188: Power-Up delay control register                   */
};


volatile AdcBase& getAdc1();
volatile AdcBase& getAdc2();

volatile uint32_t* getAdc1Ram();
volatile uint32_t* getAdc2Ram();

volatile uint32_t* getAdc1ParityRam();
volatile uint32_t* getAdc2ParityRam();


#endif
