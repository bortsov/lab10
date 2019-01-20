/*
 * mfocon.cpp
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
static constexpr char strHelp[] = "Разрешение работы FOC и/или "
                                  "старт лога для ЭД";
static constexpr Command cmd("mfocon", strHelp, func);



static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';

    logpoint::enable();
    ::motorNew::set::runMode(motorNew::MODE_CLOSE_CURRENT_LOOP);
    return true;
}

namespace mfocon {
const Command& getCommand()
{
    return cmd;
}

} /* namespace mfocon */

} /* namespace mon */

