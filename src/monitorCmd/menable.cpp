/*
 * menable.cpp
 *
 *  Created on: 20 окт. 2015 г.
 *      Author: bortsov
 */


#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"

namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Быстрый рестарт драйвера мотора";
static constexpr Command cmd("menable", strHelp, func);



static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    motorNew::resetMotorDriver();
    return true;
}

namespace menable {
const Command& getCommand()
{
    return cmd;
}

} /* namespace menable */

} /* namespace mon */

