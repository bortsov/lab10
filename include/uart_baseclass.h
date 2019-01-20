/*
 * uart.h
 *
 *  Created on: 07 окт. 2014 г.
 *      Author: bortsov
 */

#ifndef UART_H_
#define UART_H_

#include <cstdint>
#include <cstddef>
#include <bitset>

namespace uart {

class Uart {
public:
    virtual bool isNewChar() const = 0;
    virtual long putChar(uint8_t ch) = 0;
    virtual long putArray(const uint8_t* str, size_t len) = 0;
            long putArray(const char    *str, size_t len) {return putArray(reinterpret_cast<const uint8_t*>(str), len);}
    virtual long putString(const uint8_t* str)            {return true;}
            long putString(const char    *str)            {return putString(reinterpret_cast<const uint8_t*>(str));}
    virtual long putDigit(int v)                          {return true;}
    virtual long putTens(int v)                           {return true;}
    virtual long putHundred(int v)                        {return true;}
    virtual long putThousand (int v)                      {return true;}
    virtual uint8_t getChar() = 0;
    virtual void process() = 0;
    virtual void sync() = 0;
    virtual ~Uart(){}
    friend Uart&  operator << (Uart& s, uint32_t v);
    friend Uart&  operator << (Uart& s, int v);
    friend Uart&  operator << (Uart& s, const char* str);
    friend Uart&  operator << (Uart& s, char ch);
    friend Uart&  operator << (Uart& s, const std::bitset<32>& v32);
    friend Uart&  operator << (Uart& s, const uint64_t& v);
    friend Uart&  operator << (Uart& s, float v);
    virtual void outFloat3P(float v){}
    virtual void outFloat6P(float v){}
};


Uart& getEmptyUART();

} /* namespace uart */



#endif /* UART_H_ */
