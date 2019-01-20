/*
 * reg_het.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */

#include "reg_het.h"

static volatile HetBase * const hetREG1 = reinterpret_cast<volatile HetBase *>(0xFFF7B800U);
static volatile HetBase * const hetREG2 = reinterpret_cast<volatile HetBase *>(0xFFF7B900U);

static volatile GioPort * const hetPORT1 = reinterpret_cast<volatile GioPort *>(0xFFF7B84CU);
static volatile GioPort * const hetPORT2 = reinterpret_cast<volatile GioPort *>(0xFFF7B94CU);

static volatile HetRam * const hetRAM1 = reinterpret_cast<volatile HetRam *>(0xFF460000U);
static volatile HetRam * const hetRAM2 = reinterpret_cast<volatile HetRam *>(0xFF440000U);

static volatile uint32_t * const NHET1RAMPARLOC = reinterpret_cast<volatile uint32_t*>(0xFF462000U);
static volatile uint32_t * const NHET2RAMPARLOC = reinterpret_cast<volatile uint32_t*>(0xFF442000U);

static volatile uint32_t * const NHET1RAMLOC = reinterpret_cast<volatile uint32_t*>(0xFF460000U);
static volatile uint32_t * const NHET2RAMLOC = reinterpret_cast<volatile uint32_t*>(0xFF440000U);

volatile HetBase& getHet1()      {return *hetREG1;}
volatile HetBase& getHet2()      {return *hetREG2;}

volatile GioPort& getHet1Port()  {return *hetPORT1;}
volatile GioPort& getHet2Port()  {return *hetPORT2;}

volatile HetRam& getHet1Ram()    {return *hetRAM1;}
volatile HetRam& getHet2Ram()    {return *hetRAM2;}

volatile uint32_t * getHet1ParityRam()  {return NHET1RAMPARLOC;}
volatile uint32_t * getHET2ParityRam()  {return NHET2RAMPARLOC;}

volatile uint32_t * getHet1RamAddress()     {return NHET1RAMLOC;}
volatile uint32_t * getHet2RamAddress()     {return NHET2RAMLOC;}



