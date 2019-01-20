/*
 * mlogshow.cpp
 *
 *  Created on: 12 окт. 2018 г.
 *      Author: bortsov
 */

#include "monitorCmd.h"

#include "uart_baseclass.h"
#include "Motor/motorNew.h"
#include "LogPoint.h"
#include "SciLog.h"
#include "SciDmaUart.h"

namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Непрерывный вывод лога мотора";
static constexpr Command cmd("mlogshow", strHelp, func);



static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';

    const auto nAvg = logpoint::getAverages();
    const auto nCurr = logpoint::getCurrentSize();
    const auto nMax = logpoint::getMaxSize();
    int idxMax = nCurr;
    if (nAvg >= 50) { /* if number of averages is more than 50 then do real-time out */
        ::logpoint::setSize(1);
        idxMax = 1;
    }

    logpoint::enable();
    for (;;) {
        if (logpoint::isLogFilled()) {
            if (idxMax == 1) {
                const auto p = ::logpoint::get(0);
                logpoint::enable();
                linscidma::waitForEndTransfer();
                logSci::log12(
                        p.v[0],
                        p.v[1],
                        p.v[2],
                        p.v[3]);
            } else {
                for (int i = 0; i < idxMax; ++i) {
                    const auto& p = ::logpoint::get(i);
                    linscidma::waitForEndTransfer();
                    logSci::log12(
                            p.v[0],
                            p.v[1],
                            p.v[2],
                            p.v[3]);
                    cb.process();
                    if (cb.isNewChar()) {
                        break;
                    }
                }
                logpoint::enable();
            }
        }
        if (cb.isNewChar()) {
            cb.getChar();
            break;
        }
        cb.process();
    }
    ::logpoint::setSize(nMax);

    return true;
}

namespace mlogshow {
const Command& getCommand()
{
    return cmd;
}

} /* namespace resetangles */

} /* namespace mon */


