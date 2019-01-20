/*
* monitor.h
*
*  Created on: 30.10.2012
*      Author: bortsov
*/

#ifndef MONITOR_H_
#define MONITOR_H_

#include <cstddef>
#include <cstdint>

namespace uart {class Uart;}

namespace mon {

extern void (*create)(uart::Uart* cb);
extern void (*destroy)();

void taskBody(); /* вынесено для работы без RTOS */

class Command {
public:
    const char*  const cmd;
    const char*  const help;
    const char*  const parameters;
    const char*  const parametersDescription;
    bool (* const run)(const char* cmdpar, uart::Uart& cb);
    constexpr Command(const char* const cmd,
                      const char* const desc,
                      const char* const descParams,
                      bool (* const f)(const char* cmdpar, uart::Uart& cb)) :
            cmd(cmd),
            help(desc),
            parameters(descParams),
            parametersDescription(nullptr),
            run(f)
    {
    }
    constexpr Command(const char* const cmd,
                      const char* const desc,
                      bool (* const f)(const char* cmdpar, uart::Uart& cb)) :
            cmd(cmd),
            help(desc),
            parameters(nullptr),
            parametersDescription(nullptr),
            run(f)
    {
    }
    constexpr Command(const char* const cmd,
                      const char* const desc,
                      const char* const descParams,
                      const char* const descParams2,
                      bool (* const f)(const char* cmdpar, uart::Uart& cb)) :
            cmd(cmd),
            help(desc),
            parameters(descParams),
            parametersDescription(descParams2),
            run(f)
    {
    }
};

void registerCmd(const Command& cmd);
const char* readOneParameterWithCapitalConversation(char* to, const char* from, size_t maxBytes);
const char* readOneParameter(char* to, const char* from, size_t maxBytes);
int readUnsignedIntegerParameter(const char* s);
int readSignedIntegerParameter(const char* s);
uint8_t readOneDigitHEXParameter(const char* s);
uint8_t readTwoDigitHEXParameter(const char* s);
uint32_t readSixDigitHEXParameter(const char* s);
bool isStrEmpty(const char* str);
bool checkStrForDigit(const char* s);
int getDigit(const char* s);
int getTens(const char* s);
int getHundred(const char* s);
int getThousand(const char* s);
int getTenThousand(const char* s);
int getHundredThousand(const char* s);
int getMillion(const char* s);
int getTenMillion(const char* s);
int getHundredMillion(const char* s);
float getUnsignedFloatValueFromCstring(const char* buf);
float getSignedFloatValueFromCstring(const char* s);
size_t getInputBufferSize();

void setBlueFont(uart::Uart& cb);
void setNormalFont(uart::Uart& cb);
void setRedFont(uart::Uart& cb);

} /* namespace mon */


#endif /* MONITOR_H_ */

