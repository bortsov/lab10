
#ifndef __REG_HET_H__
#define __REG_HET_H__

#include <cstdint>

#include "reg_gio.h"



struct HetBase
{
    uint32_t GCR;     /**< 0x0000: Global control register              */
    uint32_t PFR;     /**< 0x0004: Prescale factor register             */
    uint32_t ADDR;    /**< 0x0008: Current address register             */
    uint32_t OFF1;    /**< 0x000C: Interrupt offset register 1          */
    uint32_t OFF2;    /**< 0x0010: Interrupt offset register 2          */
    uint32_t INTENAS; /**< 0x0014: Interrupt enable set register        */
    uint32_t INTENAC; /**< 0x0018: Interrupt enable clear register      */
    uint32_t EXC1;    /**< 0x001C: Exception control register 1          */
    uint32_t EXC2;    /**< 0x0020: Exception control register 2          */
    uint32_t PRY;     /**< 0x0024: Interrupt priority register          */
    uint32_t FLG;     /**< 0x0028: Interrupt flag register              */
    uint32_t AND;     /**< 0x002C: AND share control register         */
    uint32_t   rsvd1; /**< 0x0030: Reserved                             */
    uint32_t HRSH;    /**< 0x0034: High resolution share register        */
    uint32_t XOR;     /**< 0x0038: XOR share register                   */
    uint32_t REQENS;  /**< 0x003C: Request enable set register          */
    uint32_t REQENC;  /**< 0x0040: Request enable clear register        */
    uint32_t REQDS;   /**< 0x0044: Request destination select register  */
    uint32_t   rsvd2; /**< 0x0048: Reserved                             */
    uint32_t DIR;     /**< 0x004C: Direction register                   */
    uint32_t DIN;     /**< 0x0050: Data input register                  */
    uint32_t DOUT;    /**< 0x0054: Data output register                 */
    uint32_t DSET;    /**< 0x0058: Data output set register             */
    uint32_t DCLR;    /**< 0x005C: Data output clear register           */
    uint32_t PDR;     /**< 0x0060: Open drain register                  */
    uint32_t PULDIS;  /**< 0x0064: Pull disable register                */
    uint32_t PSL;     /**< 0x0068: Pull select register                 */
    uint32_t   rsvd3; /**< 0x006C: Reserved                             */
    uint32_t   rsvd4; /**< 0x0070: Reserved                             */
    uint32_t PCR;   /**< 0x0074: Parity control register              */
    uint32_t PAR;     /**< 0x0078: Parity address register              */
    uint32_t PPR;     /**< 0x007C: Parity pin select register           */
    uint32_t SFPRLD;  /**< 0x0080: Suppression filter preload register  */
    uint32_t SFENA;   /**< 0x0084: Suppression filter enable register   */
    uint32_t   rsvd5; /**< 0x0088: Reserved                             */
    uint32_t LBPSEL;  /**< 0x008C: Loop back pair select register       */
    uint32_t LBPDIR;  /**< 0x0090: Loop back pair direction register    */
    uint32_t PINDIS;  /**< 0x0094: Pin disable register                 */
};


struct HetInstructionBase
{
    uint32_t Program;
    uint32_t Control;
    uint32_t Data;
    uint32_t   rsvd1;
};


struct HetRam
{
    HetInstructionBase Instruction[160U];
};



volatile HetBase& getHet1();
volatile HetBase& getHet2();

volatile GioPort& getHet1Port();
volatile GioPort& getHet2Port();

volatile HetRam& getHet1Ram();
volatile HetRam& getHet2Ram();

volatile uint32_t* getHet1ParityRam();
volatile uint32_t* getHET2ParityRam();

volatile uint32_t* getHet1RamAddress();
volatile uint32_t* getHet2RamAddress();


#endif
