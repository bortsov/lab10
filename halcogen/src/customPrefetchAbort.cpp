#include "jlink.h"
#include "uart_baseclass.h"
#include "monitor.h"

extern "C" void prefetchEntry_c();

extern "C" void prefetchEntry_c()
{
    uart::Uart& cb = *jlink::get(jlink::TERMINAL);
    mon::setRedFont(cb);
    cb << "\nПродетектирован Prefetch Abort!!!";
    mon::setNormalFont(cb);
    for (;;) {
        jlink::process();
    }
}
