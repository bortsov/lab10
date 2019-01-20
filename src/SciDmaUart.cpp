/*
 * SciDmaUart.cpp
 *
 *  Created on: 8 янв. 2018 г.
 *      Author: Виталий
 */

#include "SciDmaUart.h"
#include "sys_dma.h"
#include "reg_dma.h"
#include "reg_sci.h"
#include "Sci.h"
#include "init.h"
#include "irq.h"

namespace linscidma {

static constexpr DMA_CHANNEL USED_DMA_CHANNEL = DMA_CHANNEL_0;
static constexpr uint32_t REQUEST_NUMBER_FOR_LINSCI_TX = 29;


/*
 * Функция сделана именно такой для максимального быстродействия,
 * вместо обращения к mcal::sci::disableTxDma():
 * 8 инструкций с двумя индексными обращениями
 * в противовес 11 инструкциям с 4 индексными обращениями
 */
static void disableTxDmaInterruptForLinsci2()
{
    constexpr uint32_t addressRegisterSci2ClearInt = 0xFFF7E410U;
    *reinterpret_cast<volatile uint32_t*>(addressRegisterSci2ClearInt) = 1 << 16;
}

/*
 * Функция сделана именно такой для максимального быстродействия,
 * вместо обращения к mcal::sci::enableTxDma():
 */
static void enableTxDmaInterruptForLinsci2()
{
    constexpr uint32_t addressRegisterSci2SetInt = 0xFFF7E40CU;
    *reinterpret_cast<volatile uint32_t*>(addressRegisterSci2SetInt) = 1 << 16;
}


static void acknowledgeDmaBtcaIrq()
{
    getDmaBase().BTCAOFFSET; /* just read from peripheral register */
}


/* not always PEND bit is cleared after end of block transfer,
 * therefore PEND.0 is cleared by modify control packet
 */
static void clearPendBitForChannel0BecauseOfBugInChip()
{
    volatile uint32_t* isaddr = reinterpret_cast<volatile uint32_t*>(0xFFF80000U);
    *isaddr = *isaddr; // @suppress("Assignment to itself")
}


static __attribute__((target("arm"), interrupt("IRQ"))) void dmaBtcaIrq()
{
    acknowledgeDmaBtcaIrq();
    clearPendBitForChannel0BecauseOfBugInChip();
    disableTxDmaInterruptForLinsci2();
}


void create()
{

    dmaEnable();
    dmaSetPortAssignmentRegister(USED_DMA_CHANNEL);
    dmaSetElementCount(USED_DMA_CHANNEL, 1);
    dmaAssignRequestToChannel(USED_DMA_CHANNEL, REQUEST_NUMBER_FOR_LINSCI_TX);
    dmaSetChannelControlRegister(USED_DMA_CHANNEL,
                                 DMA_CHAIN_TYPE_DISABLE,
                                 DMA_ACCESS_WIDTH_8_BIT,
                                 DMA_ACCESS_WIDTH_8_BIT,
                                 DMA_FRAME_TRANSFER,
                                 DMA_ADDRESS_TYPE_INC1,
                                 DMA_ADDRESS_TYPE_FIXED,
                                 DMA_AUTOINIT_OFF);
    dmaSetDestinationAddress(USED_DMA_CHANNEL,
                             reinterpret_cast<uint32_t>(&getScilin().TD));
    dmaEnableInterrupt(USED_DMA_CHANNEL, DMA_INTERRUPT_BLOCK_TRANSFER_COMPLETE);

    registerIrqVectorInVim(irq::IRQ_DMA_BTCA, dmaBtcaIrq);
}



void setTxPointer(const uint8_t* data)
{
    dmaSetSourceAddress(USED_DMA_CHANNEL,
                             reinterpret_cast<uint32_t>(data));
}


const uint8_t* getTxPointer()
{
	return reinterpret_cast<const uint8_t*>(dmaGetSourceAddress(USED_DMA_CHANNEL));
}


void setTxSize(size_t size)
{
    dmaSetFrameCount(USED_DMA_CHANNEL, size);
}


void startTx()
{
    dmaEnableChannelWithHardwareRequest(USED_DMA_CHANNEL);
    enableTxDmaInterruptForLinsci2();
}


bool isLastTxDone()
{
    return !dmaIsChannelActiveForHardwareRequest(USED_DMA_CHANNEL);
}


void waitForEndTransfer()
{
    while (isLastTxDone() == false) {
        ;
    }
}

} /* namespace linscidma */


