/*
 * jlink.cpp
 *
 *  Created on: 05 нояб. 2014 г.
 *      Author: bortsov
 */


#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cmath>

#include "jlink.h"
#include "itoa.h"

#include "JLINK_DCC_Process.h"
#include "jlink_queues.h"
#include "blockingWait.h"


static uart::Uart* dbgChannel = nullptr;
static uint32_t cntReceivedWordsFromPC = 0;


namespace jlink {
static Comm* ptrs[LAST_DBG_CHANNEL] = {nullptr};

static void createChannel(const DEBUG_CHANNELS channel)
{
    if (!ptrs[channel]) {
        ptrs[channel] = new Comm(channel);
    }
}

static void createImpl()
{
    jlink::queues::create();
    for (int channel = 0; channel < LAST_DBG_CHANNEL; ++channel) {
        createChannel(static_cast<DEBUG_CHANNELS>(channel));
    }
    dbgChannel = ptrs[TERMINAL];
}

static void destroyImpl()
{
    for (int i = 0; i < LAST_DBG_CHANNEL; ++i) {
        if (ptrs[i]) {
            delete ptrs[i];
            ptrs[i] = nullptr;
        }
    }
    jlink::queues::destroy();
    dbgChannel = nullptr;
}

static Comm *getImpl(DEBUG_CHANNELS channel_type)
{
    return ptrs[channel_type];
}

static uint32_t DCCIsTxEmpty()
{
    constexpr uint32_t TX_FULL_FLAG_SHIFT = 29;
    const bool DCCbusy = CP14_ReadDCCStat() & (1 << TX_FULL_FLAG_SHIFT);
    return !DCCbusy;
}

static uint32_t DCCIsNewCharRdy()
{
    constexpr uint32_t RX_FULL_FLAG_SHIFT = 30;
    const bool isNewChar = CP14_ReadDCCStat() & (1 << RX_FULL_FLAG_SHIFT);
    return isNewChar;
}

static void processWrites()
{
    using namespace queues;
    if (!DCCIsTxEmpty()) {
        return;
    }
    if (checkEmptyTxQueue()) {
        return;
    }
    CP14_WriteDCC(readTxQueue());
}

static void processRead()
{
    using namespace queues;
    if (!DCCIsNewCharRdy()) return;
    ++cntReceivedWordsFromPC;
    union {
        struct {
            char ch1;
            char ch2;
            char ch3;
            char length:4;
            char channel:4;
        } s;
        uint32_t d;
    } u;
    u.d = CP14_ReadDCC();
    if (u.s.channel >= LAST_DBG_CHANNEL) return;

    if (u.s.length == 1) {
        sendRxQueue(u.s.ch1, static_cast<DEBUG_CHANNELS>(u.s.channel));
    } else if (u.s.length == 2) {
        sendRxQueue(u.s.ch2, static_cast<DEBUG_CHANNELS>(u.s.channel));
        sendRxQueue(u.s.ch1, static_cast<DEBUG_CHANNELS>(u.s.channel));
    } else if (u.s.length == 3) {
        sendRxQueue(u.s.ch3, static_cast<DEBUG_CHANNELS>(u.s.channel));
        sendRxQueue(u.s.ch2, static_cast<DEBUG_CHANNELS>(u.s.channel));
        sendRxQueue(u.s.ch1, static_cast<DEBUG_CHANNELS>(u.s.channel));
    }
}

static void processImpl()
{
    processWrites();
    processRead();
}

static bool wasSomethingReceivedFromPC()
{
    return cntReceivedWordsFromPC;
}


static void flush2Impl()
{
    using namespace queues;
    while (!checkEmptyTxQueue()) {
        if (DCCIsTxEmpty()) {
            CP14_WriteDCC(readTxQueue());
        }
    }
}


static void flushImpl()
{
    if (wasSomethingReceivedFromPC()) {
        flush2Impl();
    }
}



void (*create)() = createImpl;
void (*destroy)() = destroyImpl;
Comm *(*get)(DEBUG_CHANNELS type) = getImpl;
void (*process)() = processImpl;
void (*flush)() = flushImpl;
void (*flush2)() = flush2Impl;

Comm::Comm(DEBUG_CHANNELS _channel) :
        channel(_channel)
{
    ;
}

long Comm::putChar(uint8_t ch)
{
    using namespace queues;
    sendTxQueue((channel << 28) | 0x01000000 | ch);
    return 0;
}

long Comm::sendTwoChars(char ch1, char ch2)
{
    using namespace queues;
    const uint32_t dataToDCC = (channel << 28) | 0x02000000
            | (static_cast<uint8_t>(ch1) << 8)
            | (static_cast<uint8_t>(ch2));
    sendTxQueue(dataToDCC);
    return 0;
}

long Comm::sendThreeChars(char ch1, char ch2, char ch3)
{
    using namespace queues;
    const uint32_t dataToDCC = (channel << 28) | 0x03000000
            | (static_cast<uint8_t>(ch1) << 16)
            | (static_cast<uint8_t>(ch2) << 8)
            | (static_cast<uint8_t>(ch3));
    sendTxQueue(dataToDCC);
    return 0;
}


long Comm::putArray(const uint8_t* buf, size_t length)
{
    constexpr int numberBytesInFullData = 3;
    const int numberFullData = length / numberBytesInFullData;
    const int numberRestData = length - numberFullData * numberBytesInFullData;
    for (int i = 0; i < numberFullData; ++i) {
        sendThreeChars(buf[0], buf[1], buf[2]);
        buf += numberBytesInFullData;
    }
    if (numberRestData == numberBytesInFullData - 1) {
        sendTwoChars(buf[0], buf[1]);
    } else if (numberRestData == 1) {
        putChar(*buf);
    }
    return 0;
}

long Comm::putString(const uint8_t* str)
{
    return putArray(str, strlen(reinterpret_cast<const char*>(str)));
}

uint8_t Comm::getChar(void)
{
    return queues::readRxQueue(channel);
}

bool Comm::isNewChar() const
{
    return !queues::checkEmptyRxQueue(channel);
}

long Comm::putDigit(int v)
{
    return putChar('0' + v);
}

long Comm::putTens(int v)
{
    putDigit((v % 100) / 10);
    return putDigit(v % 10);
}

long Comm::putHundred(int v)
{
    putDigit((v % 1000) / 100);
    return putTens(v);
}

long Comm::putThousand(int v)
{
    putDigit((v % 10000) / 1000);
    return putHundred(v);
}


void Comm::process()
{
    jlink::process();
}


void Comm::sync()
{
    jlink::flush();
}


void Comm::outFloat3P(float v)
{
    char buf[24];
    const size_t s = float3pToA(buf, v);
    putArray(reinterpret_cast<const uint8_t*>(buf), s);
}


void Comm::outFloat6P(float v)
{
    char buf[27];
    const size_t s = float6pToA(buf, v);
    putArray(reinterpret_cast<const uint8_t*>(buf), s);
}





void setDebugChannel(uart::Uart* cb)
{
    dbgChannel = cb;
}

uart::Uart* getDebugChannel()
{
    return dbgChannel;
}

void putArrayToDbg(const char* s, size_t len)
{
    if (dbgChannel) {
        dbgChannel->putArray(s, len);
    }
}

void putStringToDbg(const char* s)
{
    putArrayToDbg(s, strlen(s));
}


void putCharToDbg(char ch)
{
    if (dbgChannel) {
        *dbgChannel << ch;
    }
}


} /* namespace jlink */



