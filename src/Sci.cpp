/*
 * SCI.cpp
 *
 *  Created on: 11 сент. 2016 г.
 *      Author: bortsov
 */

#include "Sci.h"
#include "reg_sci.h"
#include "irq.h"
#include "vclk1.h"
#include "userExitCodes.h"

#include <cmath>


namespace mcal {
namespace sci {


static class Sci* sci = nullptr;




class Sci: private irq::IrqCapable {
    void (*handlerRx)(int);
    volatile SciBase& base;
    virtual bool handlerIrq() override;
    void enableSci();
    void disableSci();
    void enterSoftwareResetState();
    void exitSoftwareResetState();
    void disableAllInterrupts();
    void disableMultiBufferMode();
    void disableSleepMode();
    void selectSciMode();
    void useInternalClockForSciMode();
    void useOneStopBit();
    void disableParityForSciMode();
    void useAsynhronousMode();
    void setPinForRx();
    void setPinForTx();
    void setBaudrate(int baudrate);
    void setFractionalDivider(int M);
    void setSuperFractionalDivider(int U);
    void setPrescaler(int P);
    void enableRx();
    void enableTx();
    void setNumberOfBits(int nBits);
    void setRxInterruptLevel0();
    void enableRxInterrupt();
    bool isTxRdy() const;
public:
    void setRxHandler(void (*f)(int)) {handlerRx = f;}
    void sendByte(uint8_t byte);
    void enableTxDma();
    void disableTxDma();
    Sci(volatile SciBase& base);
    ~Sci();
};




static void clearBit(volatile uint32_t& reg, int nBit)
{
    reg &= ~(1 << nBit);
}

static void setBit(volatile uint32_t& reg, int nBit)
{
    reg |= 1 << nBit;
}

void Sci::enableSci()
{
    constexpr int nBit_RESET = 0;
    setBit(base.GCR0, nBit_RESET);
}

void Sci::disableSci()
{
    constexpr int nBit_RESET = 0;
    clearBit(base.GCR0, nBit_RESET);
}


void Sci::enterSoftwareResetState()
{
    constexpr int nBit_SWnRST = 7;
    clearBit(base.GCR1, nBit_SWnRST);
}

void Sci::exitSoftwareResetState()
{
    constexpr int nBit_SWnRST = 7;
    setBit(base.GCR1, nBit_SWnRST);
}

void Sci::disableAllInterrupts()
{
    base.CLEARINT = 0xFFFFFFFFU;
    base.CLEARINTLVL = 0xFFFFFFFFU;
}

void Sci::disableMultiBufferMode()
{
    constexpr int nBit_MBUFMODE = 10;
    clearBit(base.GCR1, nBit_MBUFMODE);
}

void Sci::disableSleepMode()
{
    constexpr int nBit_SLEEP = 8;
    clearBit(base.GCR1, nBit_SLEEP);
}

void Sci::selectSciMode()
{
    constexpr int nBit_LINMODE = 6;
    clearBit(base.GCR1, nBit_LINMODE);
}

void Sci::useInternalClockForSciMode()
{
    constexpr int nBit_CLOCK = 5;
    setBit(base.GCR1, nBit_CLOCK);
}

void Sci::useOneStopBit()
{
    constexpr int nBit_STOP = 4;
    clearBit(base.GCR1, nBit_STOP);
}

void Sci::disableParityForSciMode()
{
    constexpr int nBit_PARITY_ENA = 2;
    clearBit(base.GCR0, nBit_PARITY_ENA);
}

void Sci::useAsynhronousMode()
{
    constexpr int nBit_TIMNING_MODE = 1;
    setBit(base.GCR1, nBit_TIMNING_MODE);
}

void Sci::setPinForRx()
{
    auto& port = base.port;
    constexpr int nBit_RX_FUNC = 1;
    setBit(port.DIR, nBit_RX_FUNC);
}

void Sci::setPinForTx()
{
    auto& port = base.port;
    constexpr int nBit_TX_FUNC = 2;
    setBit(port.DIR, nBit_TX_FUNC);
}

void Sci::setFractionalDivider(int M)
{
	base.BRS &= 0xF0FFFFFF;
	base.BRS |= M << 24;
}

void Sci::setSuperFractionalDivider(int U)
{
	base.BRS &= 0x0FFFFFFF;
	base.BRS |= U << 28;
}

void Sci::setPrescaler(int P)
{
	base.BRS &= 0xFF000000;
	base.BRS |= P;
}


void Sci::setBaudrate(int baudrate)
{
	constexpr float maxBaudrateToleranceInPercent = 1.0F;
    constexpr int M = 0;
    constexpr int U = 0;
    const auto vclk1Freq = vclk1::getFreqHz();
    setFractionalDivider(M);
    setSuperFractionalDivider(U);
    int P = roundf(vclk1Freq / (16.0F * baudrate)) - 1;
    if (P == 0)  {
        P = 1;
    }
    const float realBaudrate = vclk1Freq / (16.0F * static_cast<float>(P + 1));
    const float tolerance = 100.0F * (realBaudrate - baudrate) / baudrate;
    if (P < 1) {
        exit(EXIT_SCI_UNABLE_TO_SET_BAUDRATE); /* unable to set P=0 according to TRM; a baudrate with P=0 equals one with P=1 if M = 0 */
    }
    if (tolerance > maxBaudrateToleranceInPercent) {
        exit(EXIT_SCI_BIG_BAUDRATE_ERROR); /* too big tolerance to setup required baudrate */
    }
    setPrescaler(P);
}


void Sci::enableRx()
{
    constexpr int nBit_RXENA = 24;
    setBit(base.GCR1, nBit_RXENA);
}

void Sci::enableTx()
{
    constexpr int nBit_TXENA = 25;
    setBit(base.GCR1, nBit_TXENA);
}

void Sci::setNumberOfBits(int nBits)
{
    base.FORMAT = nBits - 1;
}

void Sci::setRxInterruptLevel0()
{
    constexpr int nBit_RxIntLevel = 9;
    base.CLEARINTLVL = 1 << nBit_RxIntLevel;

}

void Sci::enableRxInterrupt()
{
    constexpr int nBit_RxInt = 9;
    base.SETINT = 1 << nBit_RxInt;
}

bool Sci::isTxRdy() const
{
	constexpr int nBit_TxInt = 8;
    return base.FLR & (1 << nBit_TxInt);
}

void Sci::sendByte(uint8_t byte)
{
	while (isTxRdy() == false) {
		;
	}
    base.TD = byte;

}


void Sci::enableTxDma()
{
     base.SETINT = 1 << 16; /* enable TX DMA requests */
}


void Sci::disableTxDma()
{
     base.CLEARINT = 1 << 16; /* disable TX DMA requests */
}


Sci::Sci(volatile SciBase& base) :
		IrqCapable(irq::IRQ_LIN_LEVEL_0, irq::IRQ), handlerRx(nullptr), base(base)
{
	enableSci();
	enterSoftwareResetState();
	disableAllInterrupts();
	disableMultiBufferMode();
	disableSleepMode();
	selectSciMode();
	useOneStopBit();
	disableParityForSciMode();
	useAsynhronousMode();
	setNumberOfBits(8);
	setPinForRx();
	setPinForTx();
	constexpr int baudrate = 115200; /* max = VLCK1 / 32 = 90 / 32 = 2812500 MHz */
	setBaudrate(baudrate);
	useInternalClockForSciMode();
	setRxInterruptLevel0();
	enableRxInterrupt();
	enableRx();
	enableTx();
	exitSoftwareResetState();
}


Sci::~Sci()
{
    enterSoftwareResetState();
    disableSci();
}

bool Sci::handlerIrq()
{
    int rxChar = base.RD;
    if (handlerRx) {
        handlerRx(rxChar);
    }
    sendByte(rxChar);
    return false;
}


void create()
{
    if (sci) {
        exit(EXIT_SCI_IS_ALREADY_CREATED);
    }
    sci = new Sci(getScilin());
}

void setRxHandler(void (*handler)(int))
{
    if (sci == nullptr) {
    	exit(EXIT_SCI_INVALID_USER_HANDLER_TO_RECEIVE);
    }
    sci->setRxHandler(handler);
}

void sendByte(uint8_t byte)
{
	sci->sendByte(byte);
}


void enableTxDma()
{
    sci->enableTxDma();
}


void disableTxDma()
{
    sci->disableTxDma();
}

} /* namespace sci */
} /* namespace mcal */
