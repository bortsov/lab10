/*
 * drv8301.cpp
 *
 *  Created on: 02 сент. 2015 г.
 *      Author: bortsov
 */

#include "monitorCmd.h"

#include "softwareSpi.h"
#include "uart_baseclass.h"

#include <bitset>
#include <cstring>


namespace mon {

static bool func(const char* cmdpar, uart::Uart& cb);
static constexpr char strHelp[] = "Чтение регистров DRV8301";
static constexpr Command cmd("drv8301", strHelp, func);

static bool flagFirstRead = true;

static void print16bitRegister(uart::Uart& cb, const uint16_t r, const char* msg)
{
    constexpr size_t maxBits = 16;
    std::bitset<maxBits> st(r);
    cb << msg;
    auto s = st.to_string();
    cb << s.c_str();
    cb << '\n';
}


static bool func(const char* cmdpar, uart::Uart& cb)
{
    cb << '\n' << strHelp << '\n';
    if (flagFirstRead) {
        flagFirstRead = false;
        softspi::doExchange16bit(0x8000); /* instruction to out STAT1 register at next SPI exchange */
    }
    const auto stat1 = softspi::doExchange16bit(0x8800);
    const auto stat2 = softspi::doExchange16bit(0x9000);
    const auto conf1 = softspi::doExchange16bit(0x9800);
    const auto conf2 = softspi::doExchange16bit(0x8000);
    cb << " завершено!\n";
    print16bitRegister(cb, stat1, "Status1=");
    print16bitRegister(cb, stat2, "Status2=");
    print16bitRegister(cb, conf1, "Config1=");
    print16bitRegister(cb, conf2, "Config2=");
    return true;
}



namespace drv8301 {
const Command& getCommand()
{
    return cmd;
}
} /* namespace drv8301 */


} /* namespace mon */
