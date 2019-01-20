/*
 * SciLog.c
 *
 *  Created on: 21 янв. 2018 г.
 *      Author: Виталий
 *  За 1 прерывание ШИМ модуль SCI сумеет вывести 10 байт
 *  при baudrate=2.500.000 (VCLK1=80.000.000) и частоте ШИМ
 *  20 кГц
 */

#include "SciLog.h"
#include "Sci.h"
#include "SciDmaUart.h"
#include "itoa.h"

#include <stdint.h>


namespace logSci {

static constexpr size_t MAX_SIZE_LOG_BUF = 150;

static uint8_t logbuf[MAX_SIZE_LOG_BUF];


void create(void)
{
    mcal::sci::create();
    linscidma::create();
    linscidma::setTxSize(MAX_SIZE_LOG_BUF);
    linscidma::setTxPointer(logbuf);
}


/* выдача 5 целых чисел в полном диапазоне */
void log5(
        const int32_t v1,
        const int32_t v2,
        const int32_t v3,
        const int32_t v4,
        const int32_t v5)
{
    if (linscidma::isLastTxDone()) {
        size_t cnt = 0;
        cnt += int32_tToA(&logbuf[cnt], v1);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v2);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v3);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v4);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v5);
        logbuf[cnt] = '\n'; ++cnt;
        linscidma::setTxSize(cnt);
        linscidma::startTx();
    }
}


/* выдача 3 целых чисел в полном диапазоне */
void log8(
        const int32_t v1,
        const int32_t v2,
        const int32_t v3)
{
    if (linscidma::isLastTxDone()) {
        size_t cnt = 0;
        cnt += int32_tToA(&logbuf[cnt], v1);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v2);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v3);
        logbuf[cnt] = '\n'; ++cnt;
        linscidma::setTxSize(cnt);
        linscidma::startTx();
    }
}


/* выдача 2 целых чисел в полном диапазоне */
void log9(
        const int32_t v1,
        const int32_t v2)
{
    if (linscidma::isLastTxDone()) {
        size_t cnt = 0;
        cnt += int32_tToA(&logbuf[cnt], v1);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v2);
        logbuf[cnt] = '\n'; ++cnt;
        linscidma::setTxSize(cnt);
        linscidma::startTx();
    }
}

/* выдача беззнакового в формате X.XXX и 3 целых чисел в полном диапазоне */
void log10(
        const float v1,
        const int32_t v2,
        const int32_t v3,
        const int32_t v4)
{
    if (linscidma::isLastTxDone()) {
        size_t cnt = 0;
        cnt += float3pToA(&logbuf[cnt], v1);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v2);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v3);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v4);
        logbuf[cnt] = '\n'; ++cnt;
        linscidma::setTxSize(cnt);
        linscidma::startTx();
    }
}


/* выдача беззнакового в формате X.XXX и 1 целого числа в полном диапазоне */
void log11(
        const float v1,
        const int32_t v2)
{
    if (linscidma::isLastTxDone()) {
        size_t cnt = 0;
        cnt += float3pToA(&logbuf[cnt], v1);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += int32_tToA(&logbuf[cnt], v2);
        logbuf[cnt] = '\n'; ++cnt;
        linscidma::setTxSize(cnt);
        linscidma::startTx();
    }
}


/* выдача 4 чисел с 3 знаками после запятой */
void log12(
        const float v1,
        const float v2,
        const float v3,
        const float v4)
{
    if (linscidma::isLastTxDone()) {
        size_t cnt = 0;
        cnt += float3pToA(&logbuf[cnt], v1);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += float3pToA(&logbuf[cnt], v2);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += float3pToA(&logbuf[cnt], v3);
        logbuf[cnt] = '\t'; ++cnt;
        cnt += float3pToA(&logbuf[cnt], v4);
        logbuf[cnt] = '\n'; ++cnt;
        linscidma::setTxSize(cnt);
        linscidma::startTx();
    }
}


} /* namespace logSci */
