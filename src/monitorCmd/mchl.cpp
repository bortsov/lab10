/*
 * mchl.cpp
 *
 *  Created on: 30 сент. 2018 г.
 *      Author: Виталий
 */

#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"

namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Настройка связи между каналом лога"
        " и потоком данных";
static constexpr char strParams[] = "x y";
static constexpr char strParamsDescription[] =
        "\t[x: канал лога от 0 до 3, y: номер потока данных]"
        "\n\ty=0: Ia"
        "\n\ty=1: Ib"
        "\n\ty=2: Ic"
        "\n\ty=3: Id"
        "\n\ty=4: Iq"
        "\n\ty=5: Id_ref"
        "\n\ty=6: Iq_ref"
        "\n\ty=7: Ud"
        "\n\ty=8: Uq"
        "\n\ty=9: mechanical angle from encoder"
        "\n\ty=10: electrical angle from encoder"
        "\n\ty=11: electrical angle from angle generator"
        "\n\ty=12: torque"
        "\n\ty=13: Is_alfa"
        "\n\ty=14: Is_beta"
        "\n\ty=15: |Is|"
        "\n\ty=16: target speed"
        "\n\ty=17: unfiltered angle speed"
        "\n\ty=18: deep filtered angle speed"
        "\n\ty=19: integrator from angle speed loop"
        "\n\ty=20: integrator Id"
        "\n\ty=21: integrator Iq"
        "\n\ty=22: low filtered angle speed"
        ;
static constexpr Command cmd(
        "mchl",
        strHelp,
        strParams,
        strParamsDescription,
        func);


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';

    if (isStrEmpty(cmdpar)) {
        return false;
    }
    char buf[20];
    cmdpar = readOneParameter(buf, cmdpar, 20);
    const auto nChannel = readSignedIntegerParameter(buf);
    if (isStrEmpty(cmdpar)) {
        return false;
    }
    readOneParameter(buf, cmdpar, 20);
    if (isStrEmpty(buf)) {
        return false;
    }
    const auto nStream = readSignedIntegerParameter(buf);

    motorNew::set::linkLogChannels(nChannel, nStream);

    return true;
}


namespace mchl {

const Command& getCommand()
{
    return cmd;
}

} /* namespace mchl */

} /* namespace mon */



