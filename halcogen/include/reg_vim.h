#ifndef __REG_VIM_H__
#define __REG_VIM_H__

#include <cstdint>

struct VimBase
{
    uint32_t      IRQINDEX;         /* 0x0000       */
    uint32_t      FIQINDEX;         /* 0x0004       */
    uint32_t        rsvd1;          /* 0x0008       */
    uint32_t        rsvd2;          /* 0x000C       */
    uint32_t      FIRQPR0;          /* 0x0010       */
    uint32_t      FIRQPR1;          /* 0x0014       */
    uint32_t      FIRQPR2;          /* 0x0018       */
    uint32_t      FIRQPR3;          /* 0x001C       */
    uint32_t      INTREQ0;          /* 0x0020       */
    uint32_t      INTREQ1;          /* 0x0024       */
    uint32_t      INTREQ2;          /* 0x0028       */
    uint32_t      INTREQ3;          /* 0x002C       */
    uint32_t      REQMASKSET0;      /* 0x0030       */
    uint32_t      REQMASKSET1;      /* 0x0034       */
    uint32_t      REQMASKSET2;      /* 0x0038       */
    uint32_t      REQMASKSET3;      /* 0x003C       */
    uint32_t      REQMASKCLR0;      /* 0x0040       */
    uint32_t      REQMASKCLR1;      /* 0x0044       */
    uint32_t      REQMASKCLR2;      /* 0x0048       */
    uint32_t      REQMASKCLR3;      /* 0x004C       */
    uint32_t      WAKEMASKSET0;     /* 0x0050       */
    uint32_t      WAKEMASKSET1;     /* 0x0054       */
    uint32_t      WAKEMASKSET2;     /* 0x0058       */
    uint32_t      WAKEMASKSET3;     /* 0x005C       */
    uint32_t      WAKEMASKCLR0;     /* 0x0060       */
    uint32_t      WAKEMASKCLR1;     /* 0x0064       */
    uint32_t      WAKEMASKCLR2;     /* 0x0068       */
    uint32_t      WAKEMASKCLR3;     /* 0x006C       */
    uint32_t      IRQVECREG;        /* 0x0070       */
    uint32_t      FIQVECREG;        /* 0x0074       */
    uint32_t      CAPEVT;           /* 0x0078       */
    uint32_t        rsvd3;          /* 0x007C       */
    uint32_t      CHANCTRL[24U];    /* 0x0080-0x0DC */
};


volatile VimBase& getVim();

#endif
