#ifndef __REG_HTU_H__
#define __REG_HTU_H__

#include <cstdint>

struct HtuBase
{
    uint32_t GC;				/** 0x00 */ 
    uint32_t CPENA;			/** 0x04 */ 
    uint32_t BUSY0;			/** 0x08 */ 
    uint32_t BUSY1;			/** 0x0C */ 
    uint32_t BUSY2;			/** 0x10 */ 
    uint32_t BUSY3;			/** 0x14 */ 
	uint32_t ACPE;			/** 0x18 */ 
    uint32_t rsvd1;    		/** 0x1C */ 
    uint32_t RLBECTRL;		/** 0x20 */ 
    uint32_t BFINTS;			/** 0x24 */ 
    uint32_t BFINTC;			/** 0x28 */ 
    uint32_t INTMAP;			/** 0x2C */ 
    uint32_t rsvd2;  			/** 0x30 */ 
    uint32_t INTOFF0;			/** 0x34 */ 
    uint32_t INTOFF1;			/** 0x38 */ 
    uint32_t BIM;				/** 0x3C */ 
    uint32_t RLOSTFL;			/** 0x40 */ 
    uint32_t BFINTFL;			/** 0x44 */ 
    uint32_t BERINTFL;		/** 0x48 */ 
    uint32_t MP1S;			/** 0x4C */ 
    uint32_t MP1E;			/** 0x50 */ 
    uint32_t DCTRL;			/** 0x54 */ 
    uint32_t WPR;				/** 0x58 */ 
    uint32_t WMR;				/** 0x5C */ 
    uint32_t ID;				/** 0x60 */ 
    uint32_t PCR;				/** 0x64 */ 
    uint32_t PAR;				/** 0x68 */ 
    uint32_t rsvd3; 			/** 0x6C */ 
    uint32_t MPCS;			/** 0x70 */ 
    uint32_t MP0S;			/** 0x74 */ 
    uint32_t MP0E;			/** 0x78 */ 
};                        


struct HtuRamBase
{
    struct DCP /* 0x00-0x7C */
    {
        uint32_t IFADDRA;
        uint32_t IFADDRB;
        uint32_t IHADDRCT;
        uint32_t ITCOUNT;
    } DCP[8U];

    struct /* 0x80-0xFC */
    {
        uint32_t res[32U];
    } RESERVED;

    struct /* 0x100-0x17C */
    {
        uint32_t CFADDRA;
        uint32_t CFADDRB;
        uint32_t CFCOUNT;
        uint32_t rsvd4;
    } CDCP[8U];
};


volatile HtuBase& getHtu1();
volatile HtuBase& getHtu2();

volatile HtuRamBase& getHtu1Ram();
volatile HtuRamBase& getHtu2Ram();


#endif
