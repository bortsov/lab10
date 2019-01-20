
#ifndef __REG_PBIST_H__
#define __REG_PBIST_H__

#include <cstdint>


struct PbistBase
{
    uint32_t RAMT;            /* 0x0160: RAM Configuration Register */
    uint32_t DLR;             /* 0x0164: Datalogger Register */
    uint32_t   rsvd1[6U];     /* 0x0168 */
    uint32_t PACT;            /* 0x0180: PBIST Activate Register */
    uint32_t PBISTID;         /* 0x0184: PBIST ID Register */
    uint32_t OVER;            /* 0x0188: Override Register */
    uint32_t   rsvd2;         /* 0x018C */
    uint32_t FSRF0;           /* 0x0190: Fail Status Fail Register 0 */
    uint32_t rsvd5;           /* 0x0194 */
    uint32_t FSRC0;           /* 0x0198: Fail Status Count Register 0 */
    uint32_t FSRC1;           /* 0x019C: Fail Status Count Register 1 */
    uint32_t FSRA0;           /* 0x01A0: Fail Status Address 0 Register */
    uint32_t FSRA1;           /* 0x01A4: Fail Status Address 1 Register */
    uint32_t FSRDL0;          /* 0x01A8: Fail Status Data Register 0 */
    uint32_t   rsvd3;         /* 0x01AC */
    uint32_t FSRDL1;          /* 0x01B0: Fail Status Data Register 1 */
    uint32_t   rsvd4[3U];     /* 0x01B4 */
    uint32_t ROM;             /* 0x01C0: ROM Mask Register */
    uint32_t ALGO;            /* 0x01C4: Algorithm Mask Register */
    uint32_t RINFOL;          /* 0x01C8: RAM Info Mask Lower Register */
    uint32_t RINFOU;          /* 0x01CC: RAM Info Mask Upper Register */
};


volatile PbistBase& getPbist();

#endif
