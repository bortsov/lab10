
#ifndef __REG_I2C_H__
#define __REG_I2C_H__

#include <cstdint>
#include "reg_gio.h"

struct I2cBase
{

    uint32_t OAR;           /**<  0x0000 I2C Own Address register               */
    uint32_t IMR;           /**<  0x0004 I2C Interrupt Mask/Status register     */
    uint32_t STR;           /**<  0x0008 I2C Interrupt Status register          */
    uint32_t CKL;          /**<  0x000C I2C Clock Divider Low register          */
    uint32_t CKH;          /**<  0x0010 I2C Clock Divider High register         */
    uint32_t CNT;           /**<  0x0014 I2C Data Count register                */
    uint8_t DRR;            /**< 0x0018: I2C Data Receive register,             */
    uint8_t rsvd1;          /**< 0x0018: I2C Data Receive register, Reserved    */
    uint8_t rsvd2;          /**< 0x0018: I2C Data Receive register, Reserved    */
    uint8_t rsvd3;          /**< 0x0018: I2C Data Receive register, Reserved    */
    uint32_t SAR;           /**<  0x001C I2C Slave Address register             */
    uint8_t DXR;            /**< 0x0020: I2C Data Transmit register,            */
    uint8_t rsvd4;          /**< 0x0020: I2C Data Transmit register, Reserved   */
    uint8_t rsvd5;          /**< 0x0020: I2C Data Transmit register, Reserved   */
    uint8_t rsvd6;          /**< 0x0020: I2C Data Transmit register, Reserved   */
    uint32_t MDR;           /**<  0x0024 I2C Mode register                      */
    uint32_t IVR;           /**<  0x0028 I2C Interrupt Vector register          */
    uint32_t EMDR;          /**<  0x002C I2C Extended Mode register             */
    uint32_t PSC;           /**<  0x0030 I2C Prescaler register                 */
    uint32_t PID11;         /**<  0x0034 I2C Peripheral ID register 1           */
    uint32_t PID12;         /**<  0x0038 I2C Peripheral ID register 2           */
    uint32_t DMACR;         /**<  0x003C I2C DMA Control Register               */
    uint32_t rsvd7;         /**<  0x0040 Reserved                               */
    uint32_t rsvd8;         /**<  0x0044 Reserved                               */
    uint32_t PFNC;          /**<  0x0048 Pin Function Register                  */
    uint32_t DIR;           /**<  0x004C Pin Direction Register                 */
    uint32_t DIN;           /**<  0x0050 Pin Data In Register                   */
    uint32_t DOUT;          /**<  0x0054 Pin Data Out Register                  */
    uint32_t SET;           /**<  0x0058 Pin Data Set Register                  */
    uint32_t CLR;           /**<  0x005C Pin Data Clr Register                  */
    uint32_t PDR;           /**<  0x0060 Pin Open Drain Output Enable Register  */
    uint32_t PDIS;          /**<  0x0064 Pin Pullup/Pulldown Disable Register   */
    uint32_t PSEL;          /**<  0x0068 Pin Pullup/Pulldown Selection Register */
    uint32_t PSRS;          /**<  0x006C Pin Slew Rate Select Register          */
};


volatile I2cBase& getI2c();
volatile GioPort& getI2cPort();


#endif
