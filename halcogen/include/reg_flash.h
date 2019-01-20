
#ifndef __REG_FLASH_H__
#define __REG_FLASH_H__


#include <cstdint>

struct FlashBase
{
    uint32_t FRDCNTL;       /* 0x0000 */
    uint32_t   rsvd1;       /* 0x0004 */
    uint32_t FEDACCTRL1;    /* 0x0008 */
    uint32_t FEDACCTRL2;    /* 0x000C */
    uint32_t FCORERRCNT;    /* 0x0010 */
    uint32_t FCORERRADD;    /* 0x0014 */
    uint32_t FCORERRPOS;    /* 0x0018 */
    uint32_t FEDACSTATUS;   /* 0x001C */
    uint32_t FUNCERRADD;    /* 0x0020 */
    uint32_t FEDACSDIS;     /* 0x0024 */
    uint32_t FPRIMADDTAG;   /* 0x0028 */
    uint32_t FREDUADDTAG;   /* 0x002C */
    uint32_t FBPROT;        /* 0x0030 */
    uint32_t FBSE;          /* 0x0034 */
    uint32_t FBBUSY;        /* 0x0038 */
    uint32_t FBAC;          /* 0x003C */
    uint32_t FBFALLBACK;    /* 0x0040 */
    uint32_t FBPRDY;        /* 0x0044 */
    uint32_t FPAC1;         /* 0x0048 */
    uint32_t FPAC2;         /* 0x004C */
    uint32_t FMAC;          /* 0x0050 */
    uint32_t FMSTAT;        /* 0x0054 */
    uint32_t FEMUDMSW;      /* 0x0058 */
    uint32_t FEMUDLSW;      /* 0x005C */
    uint32_t FEMUECC;       /* 0x0060 */
    uint32_t FLOCK;         /* 0x0064 */
    uint32_t FEMUADDR;      /* 0x0068 */
    uint32_t FDIAGCTRL;     /* 0x006C */
    uint32_t FRAWDATAH;     /* 0x0070 */
    uint32_t FRAWDATAL;     /* 0x0074 */
    uint32_t FRAWECC;       /* 0x0078 */
    uint32_t FPAROVR;       /* 0x007C */
    uint32_t   rsvd2[16U];  /* 0x009C */
    uint32_t FEDACSDIS2;    /* 0x00C0 */
    uint32_t   rsvd3[15U];  /* 0x00C4 */
    uint32_t   rsvd4[13U];  /* 0x0100 */
    uint32_t   rsvd5[85U];  /* 0x0134 */
    uint32_t FSMWRENA;      /* 0x0288 */
    uint32_t   rsvd6[6U];   /* 0x028C */
    uint32_t FSMSECTOR;     /* 0x02A4 */
    uint32_t   rsvd7[4U];   /* 0x02A8 */
    uint32_t EEPROMCONFIG;  /* 0x02B8 */
    uint32_t   rsvd8[19U];  /* 0x02BC */
    uint32_t EECTRL1;       /* 0x0308 */
    uint32_t EECTRL2;       /* 0x030C */
    uint32_t EECORRERRCNT;  /* 0x0310 */
    uint32_t EECORRERRADD;  /* 0x0314 */
    uint32_t EECORRERRPOS;  /* 0x0318 */
    uint32_t EESTATUS;      /* 0x031C */
    uint32_t EEUNCERRADD;   /* 0x0320 */
};


volatile FlashBase& getFlash();



#endif
