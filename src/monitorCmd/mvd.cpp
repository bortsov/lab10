/*
 * mvd.cpp
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
static constexpr char strHelp[] = "Установка напряжения Vd";
static constexpr char strParams[] = "p";
static constexpr char strParamsDecription[] =
        "\t[p: напряжение [% от питания] обмоток статора, от 0 до 100]";
static constexpr Command cmd(
        "mvd",
        strHelp,
        strParams,
        strParamsDecription,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';

    const auto p = getSignedFloatValueFromCstring(cmdpar);
    if (isStrEmpty(cmdpar) || fabsf(p) > 100.0F) {
        return false;
    }

    constexpr float SCALE_PERC = 0.01F;
    ::motorNew::set::debugVoltageDAxis(p * SCALE_PERC);
    cb << "Задано: " << p << "%";
    return true;
}


namespace mvd {
const Command& getCommand()
{
    return cmd;
}

} /* namespace mvd */

} /* namespace mon */

