/*
 * mlogsave.cpp
 *
 *  Created on: 30 сент. 2018 г.
 *      Author: Виталий
 */

#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"
#include "LogPoint.h"
#include "SciLog.h"
#include "SciDmaUart.h"

namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Вывод лога ЭД в последовательный порт "
        "связанный с XDS100v2";
static constexpr Command cmd("mlogsave", strHelp, func);



static void showLogParameters(uart::Uart& cb)
{
    const auto max = logpoint::getCurrentSize();
    const auto avg = logpoint::getAverages();
    cb << "Число точек в логе=" << max << '\n';
    cb << "Число усреднений для каждой точки=" << avg << '\n';
    cb.sync();
}


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';

    showLogParameters(cb);
    cb.sync();

    if (logpoint::isLogFilled() == false) {
        cb << "Буфер лога не заполнен до конца, ждите ещё";
        return true;
    }

    for (int i = 0; i < logpoint::getCurrentSize(); ++i) {
        linscidma::waitForEndTransfer();
        const auto& p = logpoint::get(i);
        logSci::log12(
                p.v[0],
                p.v[1],
                p.v[2],
                p.v[3]);
        cb.process();
        if (cb.isNewChar()) {
            cb << "Останов пользователем";
            break;
        }
    }

    return true;
}


namespace mlogsave {
const Command& getCommand()
{
    return cmd;
}

} /* namespace mlogsave */

} /* namespace mon */


