
#ifndef __REG_CAN_H__
#define __REG_CAN_H__

#include <cstdint>

struct CanBase {
    uint32_t      CTL;          /**< 0x0000: Control Register                       */
    uint32_t      ES;           /**< 0x0004: Error and Status Register              */
    uint32_t      EERC;         /**< 0x0008: Error Counter Register                 */
    uint32_t      BTR;          /**< 0x000C: Bit Timing Register                    */
    uint32_t      INT;          /**< 0x0010: Interrupt Register                     */
    uint32_t      TEST;         /**< 0x0014: Test Register                          */
    uint32_t      rsvd1;        /**< 0x0018: Reserved                               */
    uint32_t      PERR;         /**< 0x001C: Parity/SECDED Error Code Register      */
    uint32_t      rsvd2[24];    /**< 0x002C - 0x7C: Reserved                        */
    uint32_t      ABOTR;        /**< 0x0080: Auto Bus On Time Register              */
    uint32_t      TXRQX;        /**< 0x0084: Transmission Request X Register        */
    uint32_t      TXRQx[4U];    /**< 0x0088-0x0094: Transmission Request Registers  */
    uint32_t      NWDATX;       /**< 0x0098: New Data X Register                    */
    uint32_t      NWDATx[4U];   /**< 0x009C-0x00A8: New Data Registers              */
    uint32_t      INTPNDX;      /**< 0x00AC: Interrupt Pending X Register           */
    uint32_t      INTPNDx[4U];  /**< 0x00B0-0x00BC: Interrupt Pending Registers     */
    uint32_t      MSGVALX;      /**< 0x00C0: Message Valid X Register               */
    uint32_t      MSGVALx[4U];  /**< 0x00C4-0x00D0: Message Valid Registers         */
    uint32_t        rsvd3;      /**< 0x00D4: Reserved                               */
    uint32_t      INTMUXx[4U];  /**< 0x00D8-0x00E4: Interrupt Multiplexer Registers */
    uint32_t        rsvd4[6];   /**< 0x00E8: Reserved                               */
    uint8_t IF1NO;              /**< 0x0100: IF1 Command Register, Msg Number       */
    uint8_t IF1STAT;            /**< 0x0100: IF1 Command Register, Status           */
    uint8_t IF1CMD;             /**< 0x0100: IF1 Command Register, Command          */
    uint8_t   rsvd9;            /**< 0x0100: IF1 Command Register, Reserved         */
    uint32_t      IF1MSK;       /**< 0x0104: IF1 Mask Register                      */
    uint32_t      IF1ARB;       /**< 0x0108: IF1 Arbitration Register               */
    uint32_t      IF1MCTL;      /**< 0x010C: IF1 Message Control Register           */
    uint8_t IF1DATx[8U];        /**< 0x0110-0x0114: IF1 Data A and B Registers      */
    uint32_t        rsvd5[2];   /**< 0x0118: Reserved                               */
    uint8_t IF2NO;              /**< 0x0120: IF2 Command Register, Msg No           */
    uint8_t IF2STAT;            /**< 0x0120: IF2 Command Register, Status           */
    uint8_t IF2CMD;             /**< 0x0120: IF2 Command Register, Command          */
    uint8_t   rsvd10;           /**< 0x0120: IF2 Command Register, Reserved         */
    uint32_t      IF2MSK;       /**< 0x0124: IF2 Mask Register                      */
    uint32_t      IF2ARB;       /**< 0x0128: IF2 Arbitration Register               */
    uint32_t      IF2MCTL;      /**< 0x012C: IF2 Message Control Register           */
    uint8_t IF2DATx[8U];        /**< 0x0130-0x0134: IF2 Data A and B Registers      */
    uint32_t        rsvd6[2];   /**< 0x0138: Reserved                               */
    uint32_t      IF3OBS;       /**< 0x0140: IF3 Observation Register               */
    uint32_t      IF3MSK;       /**< 0x0144: IF3 Mask Register                      */
    uint32_t      IF3ARB;       /**< 0x0148: IF3 Arbitration Register               */
    uint32_t      IF3MCTL;      /**< 0x014C: IF3 Message Control Register           */
    uint8_t IF3DATx[8U];        /**< 0x0150-0x0154: IF3 Data A and B Registers      */
    uint32_t        rsvd7[2];   /**< 0x0158: Reserved                               */
    uint32_t      IF3UEy[4U];   /**< 0x0160-0x016C: IF3 Update Enable Registers     */
    uint32_t        rsvd8[28];  /**< 0x0170: Reserved                               */
    uint32_t      TIOC;         /**< 0x01E0: TX IO Control Register                 */
    uint32_t      RIOC;         /**< 0x01E4: RX IO Control Register                 */
};


volatile CanBase& getCan1();
volatile CanBase& getCan2();
volatile CanBase& getCan3();


#endif
