#include "jlink.h"
#include "uart_baseclass.h"
#include "monitor.h"

extern "C" void undefEntry_c();

extern "C" void undefEntry_c()
{
    uart::Uart& cb = *jlink::get(jlink::TERMINAL);
    mon::setRedFont(cb);
    cb << "\nПродетектирован Undefined instruction Abort!!!";
    mon::setNormalFont(cb);
    for (;;) {
        jlink::process();
    }
}
