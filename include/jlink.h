/*
 * jlink.h
 *
 *  Created on: 05 нояб. 2014 г.
 *      Author: bortsov
 */

#ifndef JLINK_H_
#define JLINK_H_

#include "uart_baseclass.h"
#include <cstddef>

namespace jlink {

enum DEBUG_CHANNELS {TERMINAL, EXPERIMENTAL_TASK, RTT, TSI4040, LAST_DBG_CHANNEL};

class Comm : public uart::Uart {
    const DEBUG_CHANNELS channel;
    long sendTwoChars(const char ch1, const char ch2);
    long sendThreeChars(const char ch1, const char ch2, const char ch3);
public:
    virtual bool isNewChar() const override;
    virtual unsigned char getChar(void) override;
    virtual long putString(const unsigned char* str) override;
    long putString(const char* str) {return putString(reinterpret_cast<const unsigned char*>(str));}
    virtual long putChar(unsigned char ch) override;
    virtual long putArray(const unsigned char* buf, size_t length) override;
    virtual long putDigit(int v) override;
    virtual long putTens(int v) override;
    virtual long putHundred(int v) override;
    virtual long putThousand (int v) override;
    virtual void process() override;
    virtual void sync() override;
    virtual void outFloat3P(float v) override;
    virtual void outFloat6P(float v) override;
    Comm(DEBUG_CHANNELS channel);
    virtual ~Comm(){}
};

extern void (*create)();
extern void (*destroy)();
extern Comm *(*get)(DEBUG_CHANNELS type);
extern void (*process)();
extern void (*flush)();
extern void (*flush2)();

void setDebugChannel(uart::Uart* cb);
uart::Uart* getDebugChannel();
void putStringToDbg(const char* s);
void putCharToDbg(char ch);
void putArrayToDbg(const char* s, size_t len);

} /* namespace jlink */

#endif /* JLINK_H_ */
