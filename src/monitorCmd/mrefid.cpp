/*
 * mrefid.cpp
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Vit
 */

#include "monitorCmd.h"
#include "uart_baseclass.h"
#include "Motor/motorNew.h"

namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Установка целевого тока Id";
static constexpr char strParams[] = "i -- целевой ток Id [A]";
static constexpr Command cmd(
        "mrefid",
        strHelp,
        strParams,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    if (isStrEmpty(cmdpar)) {
        cb << "Ошибка: задайте правильное значение";
        return false;
    }
    const auto p = getSignedFloatValueFromCstring(cmdpar);
    motorNew::set::Id(p);
    return true;
}


namespace mrefid {

const Command& getCommand()
{
    return cmd;
}

} /* namespace mrefid */
} /* namespace mon */


