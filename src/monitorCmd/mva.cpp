/*
 * mva.cpp
 *
 *  Created on: 17 мая 2018 г.
 *      Author: bortsov
 */


#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"
#include <cmath>


namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Set Valpha";
static constexpr char strParams[] = "p";
static constexpr char strParamsDescription[] =
        "\t[p: [%] from power supply votage]";
static constexpr Command cmd(
        "mva",
        strHelp,
        strParams,
        strParamsDescription,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';

    const auto p = getSignedFloatValueFromCstring(cmdpar);
    if (isStrEmpty(cmdpar) || fabsf(p) > 100.0F) {
        return false;
    }

    constexpr float SCALE_PERC = 0.01F;
    ::motorNew::set::debugVoltageAlfaAxis(p * SCALE_PERC);
    return true;
}


namespace mva {
const Command& getCommand()
{
    return cmd;
}

} /* namespace mva */

} /* namespace mon */

