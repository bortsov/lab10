/*
 * mlogcnt.cpp
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
static constexpr char strHelp[] = "Выбор периода лога";
static constexpr char strParams[] = "n";
static constexpr char strParamsDecription[] =
        "\t[n: число циклов ШИМ для записи лога]";
static constexpr Command cmd(
        "mlogcnt",
        strHelp,
        strParams,
        strParamsDecription,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    const auto p = readSignedIntegerParameter(cmdpar);
    if (isStrEmpty(cmdpar) || p <= 0) {
        cb << "Ошибка: задайте правильное значение";
        return false;
    }
    logpoint::setAverages(p);
    return true;
}


namespace mlogcnt {

const Command& getCommand()
{
    return cmd;
}

} /* namespace mlogcnt */

} /* namespace mon */
