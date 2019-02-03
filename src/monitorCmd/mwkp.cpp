/*
 * mwkp.cpp
 *
 *  Created on: 30 сент. 2018 г.
 *      Author: Виталий
 */

#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"


namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Set Kp for speed loop";
static constexpr char strParams[] = "ki";
static constexpr Command cmd(
        "mwkp",
        strHelp,
        strParams,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    const auto prevKp = motorNew::get::angleSpeedKp();
    cb << "Was: ";
    cb.outFloat6P(prevKp);
    const auto p = getSignedFloatValueFromCstring(cmdpar);
    if (isStrEmpty(cmdpar)) {
        return true;
    }
    motorNew::set::angleSpeedKp(p);
    return true;
}


namespace mwkp {

const Command& getCommand()
{
    return cmd;
}

} /* namespace mwkp */

} /* namespace mon */
