/*
 * mw.cpp
 *
 *  Created on: 30 сент. 2018 г.
 *      Author: Виталий
 */

#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"


namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Set electrical angle speed";
static constexpr char strParams[] = "w";
static constexpr char strParamsDecription[] =
        "\t[w: electrical angle speed [1/min]]";
static constexpr Command cmd(
        "mw",
        strHelp,
        strParams,
        strParamsDecription,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    const auto p = getSignedFloatValueFromCstring(cmdpar);
    if (isStrEmpty(cmdpar)) {
        cb << "Ошибка: задайте правильное значение";
        return false;
    }
    motorNew::set::angleSpeed_epm(p);
    return true;
}


namespace mw {

const Command& getCommand()
{
    return cmd;
}

} /* namespace mw */

} /* namespace mon */
