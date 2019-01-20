#include <cstdio>
#include <errno.h>

#include "jlink.h"
#include "monitor.h"
#include "system.h"

extern "C" void _exit(int code);

extern "C" void _exit(int code)
{
    auto& terminal = *jlink::get(jlink::TERMINAL);

    terminal << "\nВызов exit(). Исправьте ошибку в программе.\n";
    terminal << "errno=" << errno << "\nexit code = " << code << '\n';
    terminal << "Нажатие любой клавиши приведёт к перегрузке\n";
    for (;;) {
        jlink::process();
        if (terminal.isNewChar()) {
            doReset();
        }
    }
}
