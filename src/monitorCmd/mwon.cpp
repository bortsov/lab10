/*
 * mwon.cpp
 *
 *  Created on: 30 сент. 2018 г.
 *      Author: Виталий
 */


#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"
#include "LogPoint.h"

namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Замыкание контура поддержания угловой скорости и/или "
                                  "старт лога для ЭД";
static constexpr Command cmd("mwon", strHelp, func);



static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';

    logpoint::enable();
    ::motorNew::set::runMode(motorNew::MODE_CLOSE_SPEED_LOOP);
    return true;
}

namespace mwon {
const Command& getCommand()
{
    return cmd;
}

} /* namespace mwon */

} /* namespace mon */

