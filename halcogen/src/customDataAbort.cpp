#include "jlink.h"
#include "uart_baseclass.h"
#include "monitor.h"

extern "C" void custom_dabort();

extern "C" void custom_dabort()
{
    uart::Uart& cb = *jlink::get(jlink::TERMINAL);
    mon::setRedFont(cb);
    cb << "\nПродетектирован Data Abort!!!";
    mon::setNormalFont(cb);
    for (;;) {
        jlink::process();
    }
}
