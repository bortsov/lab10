/*
 * mmode.cpp
 *
 *  Created on: 30 сент. 2018 г.
 *      Author: Виталий
 */

#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"
#include "main.h"


namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Выбор алгоритма управления ЭД";
static constexpr char strParams[] = "m";
static constexpr char strParamsDecription[] =
        "\t[m: код алгоритма управления]"
        "\n\tm=0: MODE_OFF"
        "\n\tm=1: MODE_ENCODER_CALIBRATION"
        "\n\tm=2: MODE_GENERATE_ANGLE"
        "\n\tm=3: MODE_TRANSLATE_I_ALPHA_BETA"
        "\n\tm=4: MODE_TRANSLATE_V_DQ"
        "\n\tm=5: MODE_CLOSE_CURRENT_LOOP"
        "\n\tm=6: MODE_CLOSE_SPEED_LOOP"
        ;
static constexpr Command cmd(
        "mmode",
        strHelp,
        strParams,
        strParamsDecription,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    const auto p = readSignedIntegerParameter(cmdpar);
    if (isStrEmpty(cmdpar)) {
        cb << "Ошибка: задайте правильное значение";
        return false;
    }
    motorNew::set::runMode(static_cast<motorNew::WORK_MODE>(p));
    if (p == 0) {
        disableLogCurrents();
    }
    return true;
}


namespace mmode {

const Command& getCommand()
{
    return cmd;
}

} /* namespace mmode */

} /* namespace mon */
