/*
 * pin.cpp
 *
 *  Created on: 01 сент. 2015 г.
 *      Author: bortsov
 */


#include "pin/pin.h"
#include "reg_het.h"
#include "reg_mibspi.h"
#include "reg_system.h"
#include "userExitCodes.h"



namespace pin {

static void setBit  (uint32_t& address, uint32_t bit) {address |=   1 << bit; }
static void resetBit(uint32_t& address, uint32_t bit) {address &= ~(1 << bit);}
static void onBit   (volatile GioPort& base, size_t bit)     {base.DSET = 1 << bit;}
static void offBit  (volatile GioPort& base, size_t bit)     {base.DCLR = 1 << bit;}
static void setDirOutput(volatile GioPort& base, size_t bit) {setBit((uint32_t&)base.DIR, bit);}
static void resetDir(volatile GioPort& base, size_t bit)     {resetBit((uint32_t&)base.DIR, bit);}
static bool get(volatile GioPort& base, size_t bit)          {return base.DIN & (1 << bit);}
static bool getDir(volatile GioPort& base, size_t bit)       {return base.DIR & (1 << bit);}

static volatile GioPort& getGioPortForHet(het::HET nHet)
{
    return nHet == het::HET1 ? getHet1Port() : getHet2Port();
}

static volatile GioPort& getPortBase(mibspi::MIBSPI nspi)
{
    if (mibspi::MibSPI1 == nspi) {
        return getMibspi1Port();
    } else if (mibspi::MibSPI3 == nspi) {
        return getMibspi3Port();
    } else if (mibspi::MibSPI5 == nspi) {
        return getMibspi5Port();
    } else {
    	exit(EXIT_PINS_INVALID_SPI_NUMBER);
    }
}

static volatile MibspiBase& getBase(mibspi::MIBSPI nspi)
{
    if (mibspi::MibSPI1 == nspi) {
        return getMibspi1();
    } else if (mibspi::MibSPI3 == nspi) {
        return getMibspi3();
    } else if (mibspi::MibSPI5 == nspi) {
        return getMibspi5();
    } else {
    	exit(EXIT_PINS_INVALID_SPI_NUMBER);
    }
}


class PinPortGio : public PinInterface {
    volatile GioPort& base;
    const size_t nbit;
public:
    virtual void setDirectionOutput() override      {setDirOutput   (base, nbit);}
    virtual void setDirectionInput() override       {resetDir       (base, nbit);}
    virtual bool isDirectionOutput() const override {return getDir  (base, nbit);}
    virtual bool getState() const override          {return get     (base, nbit);}
    virtual void on() override                      {onBit          (base, nbit);}
    virtual void off() override                     {offBit         (base, nbit);}
    PinPortGio(volatile GioPort& base, size_t pin) : base(base), nbit(pin) {}
};


class PinCan : public PinInterface {
    uint32_t& ioc;
public:
    virtual void setDirectionOutput() override {ioc |=  4;}
    virtual void setDirectionInput() override {ioc &= ~4;}
    virtual bool isDirectionOutput() const override {return ioc & 4;};
    virtual bool getState() const override {return ioc & 1; }
    virtual void on() override {ioc |=  2;}
    virtual void off() override {ioc &= ~2;}
    PinCan(uint32_t& ioc) : ioc(ioc) {}
};


class PinGio : public PinPortGio {
public:
    virtual bool isHwInitialized() const override  {return getGio().GCR0 & 1;}
    virtual void initHw() override                 {getGio().GCR0 = 1;}
    PinGio(volatile GioPort& base, size_t pin) : PinPortGio(base, pin) {}
};


class PinMibspi : public PinPortGio {
    const mibspi::MIBSPI nSpi;
public:
    virtual bool isHwInitialized() const override {return getBase(nSpi).GCR0 & 1; }
    virtual void initHw() override {getBase(nSpi).GCR0 = 1;}
    PinMibspi(mibspi::MIBSPI nspi, size_t nPin) : PinPortGio(getPortBase(nspi), nPin), nSpi(nspi) {}
};

class PinAdevt: public PinInterface {
    AdcBase& adc;
public:
    PinAdevt(AdcBase& adc) : adc(adc){}
    virtual void setDirectionOutput() override {adc.EVTDIR = 1;}
    virtual void setDirectionInput() override {adc.EVTDIR = 0;}
    virtual bool isDirectionOutput() const override {return adc.EVTDIR;}
    virtual bool getState() const override {return adc.EVTIN & 1;}
    virtual void on() override {adc.EVTSET = 1;}
    virtual void off() override {adc.EVTCLR = 1;}
};


static volatile GioPort& getSciPortBase(volatile SciBase& sciBase)
{
    if (&sciBase == &getScilin()) {
        return getScilinPort();
    } else {
        return getSciPort();;
    }
}

class PinSci : public PinPortGio {
    SciBase& base;
public:
    virtual bool isHwInitialized() const override  {return base.GCR0 & 1;}
    virtual void initHw() override                 {base.GCR0 = 1;}
    PinSci(SciBase& sciBase, size_t pin) : PinPortGio(getSciPortBase(sciBase), pin), base(sciBase) {}
};






PinInterface* createGioPin(volatile GioPort& baseport, size_t npin)
{
    return new PinGio(baseport, npin);
}

PinInterface* createCanRxPin(CanBase& base)
{
    return new PinCan((uint32_t&)base.RIOC);
}

PinInterface* createCanTxPin(CanBase& base)
{
    return new PinCan((uint32_t&)base.TIOC);
}

PinInterface* createHetPin(het::HET nHet, size_t npin)
{
    return new PinPortGio(getGioPortForHet(nHet), npin);
}

PinInterface* createMibspiCsPin(mibspi::MIBSPI nspi, size_t nCS)
{
    return new PinMibspi(nspi, nCS);
}

PinInterface* createMibspiSomiPin(mibspi::MIBSPI nspi, size_t nSOMI)
{
    return new PinMibspi(nspi, (nSOMI == 0) ? 11 : (24 + nSOMI));
}

PinInterface* createMibspiSimoPin(mibspi::MIBSPI nspi, size_t nSIMO)
{
    return new PinMibspi(nspi, (nSIMO == 0) ? 10 : (16 + nSIMO));
}

PinInterface* createMibspiClkPin(mibspi::MIBSPI nspi)
{
    return new PinMibspi(nspi, 9);
}

PinInterface* createMibspiEnaPin(mibspi::MIBSPI nspi)
{
    return new PinMibspi(nspi, 8);
}

PinInterface* createAdevtPin(AdcBase& adc)
{
    return new PinAdevt(adc);
}


PinInterface* createEclkPin()
{
    return new PinPortGio(getSystemPort(), 0);
}

PinInterface* createLinrxPin(SciBase& sciBase)
{
    return new PinSci(sciBase, 1);
}

PinInterface* createLintxPin(SciBase& sciBase)
{
    return new PinSci(sciBase, 2);
}


} /* namespace pin */
