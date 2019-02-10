/*
 * mrefiq.cpp
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Vit
 */

#include "monitorCmd.h"
#include "uart_baseclass.h"
#include "Motor/motorNew.h"

namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Установка целевого тока Iq";
static constexpr char strParams[] = "i -- целевой ток Iq [A]";
static constexpr Command cmd(
        "mrefiq",
        strHelp,
        strParams,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    if (isStrEmpty(cmdpar)) {
        return false;
    }
    const auto p = getSignedFloatValueFromCstring(cmdpar);
    motorNew::set::Iq(p);
    return true;
}


namespace mrefiq {

const Command& getCommand()
{
    return cmd;
}

} /* namespace mrefiq */
} /* namespace mon */


