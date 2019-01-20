/*
 * jlink_queues.cpp
 *
 *  Created on: 06 марта 2015 г.
 *      Author: bortsov
 */


#include "jlink_queues.h"
#include "util_circular_buffer.h"
#include "jlink.h"

namespace jlink {

namespace queues {

namespace {
constexpr size_t sizeTxQueue = 0x200;
constexpr size_t sizeRxQueue = 20;
typedef util::circular_buffer<uint32_t, sizeTxQueue> TxBufferType;
typedef util::circular_buffer<uint8_t, sizeRxQueue> RxBufferType;

TxBufferType *txbuf; /* имеется ошибка в компиляторах GHS MULTI и ARMCC: не вызывается конструктор, если происходит размещение статических данных, поэтому приходится вызывать конструктор вручную */
RxBufferType *rxbuf[LAST_DBG_CHANNEL];

} /* anonymous namespace */


void create()
{
    txbuf = new TxBufferType;
    for (int channel = TERMINAL; channel < LAST_DBG_CHANNEL; ++channel) {
        rxbuf[channel] = new RxBufferType;
    }
}

void destroy()
{
    if (txbuf) {
        delete txbuf;
        txbuf = nullptr;
    }
    for (auto& v: rxbuf) {
        if (v) {
            delete v;
        }
        v = nullptr;
    }
}

void sendTxQueue(uint32_t data)
{
    txbuf->in(data);
}

void sendRxQueue(uint8_t data, DEBUG_CHANNELS channel)
{
    rxbuf[channel]->in(data);
}

uint32_t readTxQueue()
{
    return txbuf->out();
}

uint8_t readRxQueue(DEBUG_CHANNELS channel)
{
    return rxbuf[channel]->out();
}

bool checkEmptyTxQueue()
{
    return txbuf->empty();
}

bool checkFullTxQueue()
{
    return txbuf->full();
}


bool checkEmptyRxQueue(DEBUG_CHANNELS channel)
{
    return rxbuf[channel]->empty();
}

size_t getTxBufferSize()
{
    return txbuf->size();
}

size_t getRxBufferSize(DEBUG_CHANNELS channel)
{
    return rxbuf[channel]->size();
}

} /* namespace queues */

} /* namespace jlink */

