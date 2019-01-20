#include "reg_adc.h"

static volatile AdcBase * const adcREG1 = reinterpret_cast<volatile AdcBase *>(0xFFF7C000U);
static volatile AdcBase * const adcREG2 = reinterpret_cast<volatile AdcBase *>(0xFFF7C200U);

static volatile uint32_t * const adcRAM1 = reinterpret_cast<volatile uint32_t*>(0xFF3E0000U);
static volatile uint32_t * const adcRAM2 = reinterpret_cast<volatile uint32_t*>(0xFF3A0000U);

static volatile uint32_t * const adcPARRAM1 = reinterpret_cast<volatile uint32_t*>(adcRAM1 + 0x1000U);
static volatile uint32_t * const adcPARRAM2 = reinterpret_cast<volatile uint32_t*>(adcRAM2 + 0x1000U);

volatile AdcBase& getAdc1() {return *adcREG1;}
volatile AdcBase& getAdc2() {return *adcREG2;}

volatile uint32_t* getAdc1Ram() {return adcRAM1;}
volatile uint32_t* getAdc2Ram() {return adcRAM2;}

volatile uint32_t* getAdc1ParityRam() {return adcPARRAM1;}
volatile uint32_t* getAdc2ParityRam() {return adcPARRAM2;}
