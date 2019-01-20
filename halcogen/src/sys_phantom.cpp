#include <cstdint>

#include "sys_vim.h"

extern "C" void phantomInterruptFromIsr() __attribute__ ((target("arm"), interrupt("IRQ")));
void phantomInterrupt();

/* Phantom Interrupt Handler */
extern "C" void phantomInterruptFromIsr()
{
}

void phantomInterrupt()
{
}
