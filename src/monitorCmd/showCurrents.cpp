/*
 * showCurrents.cpp
 *
 *  Created on: 15 мая 2018 г.
 *      Author: bortsov
 */

#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "main.h"


namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Переключение вывода тока с нижних плечей "
        "силовых ключей";
static constexpr Command cmd("showcurrents", strHelp, func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    toggleLogCurrents();
    return true;
}


namespace showcurrents {
const Command& getCommand()
{
    return cmd;
}

} /* namespace showcurrents */

} /* namespace mon */


