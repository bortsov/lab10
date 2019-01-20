/*
 * uart_baseclass.cpp
 *
 *  Created on: 27 янв. 2015 г.
 *      Author: bortsov
 */

#include <cstdio>
#include <cmath>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "uart_baseclass.h"


namespace uart {


class EmptyUart : public Uart::Uart {
public:
    virtual bool isNewChar() const override {return false;}
    virtual long putChar(uint8_t ch) override {return 0;}
    virtual long putArray(const uint8_t* str, size_t len) override {return 0;}
    virtual uint8_t getChar() override {return 0;}
    virtual void process() override {}
    virtual void sync() override {}
    EmptyUart() : Uart::Uart() {}
} emptyUART;


Uart&  operator << (Uart&  s, uint32_t binary)
{
    uint32_t temp;
    uint32_t binc, atemp;
    bool encountered_only_zeros=true;

    atemp = 0;
    temp = 1000000000;
    while (binary >= temp) {
        atemp++;
        binary -= temp;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    atemp = 0;
    temp = 100000000;
    while (binary >= temp) {
        atemp++;
        binary -= temp;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    atemp = 0;
    temp = 10000000;
    while (binary >= temp) {
        atemp++;
        binary -= temp;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    atemp = 0;
    temp = 1000000;
    while (binary >= temp) {
        atemp++;
        binary -= temp;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    atemp = 0;
    temp = 100000;
    while (binary >= temp) {
        atemp++;
        binary -= temp;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    atemp = 0;
    temp = 10000;
    while (binary >= temp) {
        atemp++;
        binary -= temp;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    atemp = 0;
    temp = 1000;
    while (binary >= temp) {
        atemp++;
        binary -= temp;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    atemp = 0;
    temp = 100;
    while (binary >= temp) {
        atemp++;
        binary -= temp;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    atemp = 0;
    binc = (char)binary;
    while (binc >= 10) {
        atemp++;
        binc -= 10;
    }
    if (atemp) encountered_only_zeros = false;
    if (!encountered_only_zeros) s.putChar(atemp + '0');

    s.putChar(binc + '0');
    return s;
}


Uart&  operator<< (Uart& s, const char* str)
{
    s.putString(str);
    return s;
}

Uart&  operator<< (Uart& s, char ch)
{
    s.putChar(ch);
    return s;
}

Uart&  operator<< (Uart& s, int v)
{
    uint32_t temp;
    if (v < 0) {
        s << '-';
        temp = -v;
    } else temp = v;
    s << temp;
    return s;
}


Uart&  operator << (Uart& s, const std::bitset<32>& v32)
{
    for (int i = 31; i >= 0; --i) s << static_cast<char>(v32[i] + '0');
    return s;
}


Uart&  operator<< (Uart& s, float v)
{
    const float absValue = fabsf(v);
    if (absValue >= 1.0F) {
        return s << static_cast<int>(v)
                << '.'
                << (static_cast<int>(absValue * 10.0F) % 10);
    } else if (absValue >= 0.1F) {
        return s << (v >= 0.0F ? "0." : "-0.")
                << (static_cast<int>(absValue * 100.0F) % 100);
    } else if (absValue >= 0.01F) {
        return s << (v >= 0.0F ? "0.0" : "-0.0")
                << (static_cast<int>(absValue * 1000.0F) % 1000);
    } else if (absValue >= 0.001F) {
        return s << (v >= 0.0F ? "0.00" : "-0.00")
                << (static_cast<int>(absValue * 10000.0F) % 10000);
    } else if (absValue >= 0.0001F) {
        return s << (v >= 0.0F ? "0.000" : "-0.000")
                << (static_cast<int>(absValue * 100000.0F) % 100000);
    } else {
        return s << "0.0";
    }
}


Uart& getEmptyUART()
{
    return emptyUART;
}

} /* namespace uart */



