/*
 * mf.cpp
 *
 *  Created on: 17 мая 2018 г.
 *      Author: bortsov
 */


#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"


namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Установка электрической скорости вращения "
        "магнитного поля статора";
static constexpr char strParams[] = "a";
static constexpr char strParamsDecription[] =
        "\t[a: электрическая частота вращения, целое знаковое число [об/мин]]";
static constexpr Command cmd(
        "mf",
        strHelp,
        strParams,
        strParamsDecription,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';

    const auto p = readSignedIntegerParameter(cmdpar);
    if (isStrEmpty(cmdpar)) {
        return false;
    }
    constexpr float SCALE_EPM_TO_HZ = 1.0F / 60.0F;
    ::motorNew::set::ectricalAngleSpeed_Hz(p * SCALE_EPM_TO_HZ);

    return true;
}


namespace mf {
const Command& getCommand()
{
    return cmd;
}

} /* namespace mf */

} /* namespace mon */

