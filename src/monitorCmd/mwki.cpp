/*
 * mwki.cpp
 *
 *  Created on: 30 сент. 2018 г.
 *      Author: Виталий
 */

#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"


namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Set Ki for speed loop";
static constexpr char strParams[] = "ki";
static constexpr Command cmd(
        "mwki",
        strHelp,
        strParams,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    const auto prevKi = motorNew::get::angleSpeedKi();
    cb << "Was: ";
    cb.outFloat6P(prevKi);
    const auto p = getSignedFloatValueFromCstring(cmdpar);
    if (isStrEmpty(cmdpar)) {
        return true;
    }
    motorNew::set::angleSpeedKi(p);
    return true;
}


namespace mwki {

const Command& getCommand()
{
    return cmd;
}

} /* namespace mwki */

} /* namespace mon */
