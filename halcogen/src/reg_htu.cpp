#include "reg_htu.h"

static volatile HtuBase * const htuREG1 = reinterpret_cast<volatile HtuBase*>(0xFFF7A400U);
static volatile HtuBase * const htuREG2 = reinterpret_cast<volatile HtuBase*>(0xFFF7A500U);

static volatile HtuRamBase * const htuRAM1 = reinterpret_cast<volatile HtuRamBase*>(0xFF4E0000U);
static volatile HtuRamBase * const htuRAM2 = reinterpret_cast<volatile HtuRamBase*>(0xFF4C0000U);

volatile HtuBase& getHtu1()       {return *htuREG1;}
volatile HtuBase& getHtu2()       {return *htuREG2;}

volatile HtuRamBase& getHtu1Ram() {return *htuRAM1;}
volatile HtuRamBase& getHtu2Ram() {return *htuRAM2;}
