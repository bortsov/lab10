
#ifndef __REG_ESM_H__
#define __REG_ESM_H__

#include <cstdint>

struct EsmBase
{
    uint32_t EEPAPR1;           /* 0x0000                 */
    uint32_t DEPAPR1;           /* 0x0004                 */
    uint32_t IESR1;             /* 0x0008                 */
    uint32_t IECR1;             /* 0x000C                 */
    uint32_t ILSR1;             /* 0x0010                 */
    uint32_t ILCR1;             /* 0x0014                 */
    uint32_t SR1[3U];           /* 0x0018, 0x001C, 0x0020 */
    uint32_t EPSR;              /* 0x0024                 */
    uint32_t IOFFHR;            /* 0x0028                 */
    uint32_t IOFFLR;            /* 0x002C                 */
    uint32_t LTCR;              /* 0x0030                 */
    uint32_t LTCPR;             /* 0x0034                 */
    uint32_t EKR;               /* 0x0038                 */
    uint32_t SSR2;              /* 0x003C                 */
    uint32_t IEPSR4;            /* 0x0040                 */
    uint32_t IEPCR4;            /* 0x0044                 */
    uint32_t IESR4;             /* 0x0048                 */
    uint32_t IECR4;             /* 0x004C                 */
    uint32_t ILSR4;             /* 0x0050                 */
    uint32_t ILCR4;             /* 0x0054                 */
    uint32_t SR4[3U];           /* 0x0058, 0x005C, 0x0060 */
};


volatile EsmBase& getEsm();


#endif
