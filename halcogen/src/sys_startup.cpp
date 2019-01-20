#include <string.h>
#include <cstdint>

#include "system.h"
#include "sys_vim.h"
#include "sys_core.h"
#include "sys_selftest.h"
#include "reg_esm.h"

/* External Functions */
int main();
extern "C" void exit(int _status);
extern "C" void __libc_init_array();

extern uint32_t *__data_load, *__data_start, *__data_end;
extern uint32_t *__bss_start__, *__bss_end__;


/* Startup Routine */
extern "C" void _c_int00(void) __attribute__((naked));

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

extern "C" void _c_int00(void)
{
    _coreInitRegisters_(); /* Initialize Core Registers to avoid CCM Error */
    _coreInitStackPointer_();

    /* Work Around for Errata DEVICE#140: ( Only on Rev A silicon)
     *
     * Errata Description:
     *            The Core Compare Module(CCM-R4) may cause nERROR to be asserted after a cold power-on
     * Workaround:
     *            Clear ESM Group2 Channel 2 error in ESMSR2 and Compare error in CCMSR register */
    if (getID() == 0x802AAD05U) {
        _esmCcmErrorsClear_();
    }

    /* Enable CPU Event Export */
    /* This allows the CPU to signal any single-bit or double-bit errors detected
     * by its ECC logic for accesses to program flash or data RAM.
     */
    _coreEnableEventBusExport_();

    /* Workaround for Errata CORTEXR4 66 */
    _errata_CORTEXR4_66_();

    /* Workaround for Errata CORTEXR4 57 */
    _errata_CORTEXR4_57_();
    /* Reset handler: the following instructions read from the system exception status register
     * to identify the cause of the CPU reset.
     */

    /* check for power-on reset condition */
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    if ((getSysException() & POWERON_RESET) != 0U) {
        /* clear all reset status flags */
        getSysException() = 0xFFFFU;

        /* continue with normal start-up sequence */
    }
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    else if ((getSysException() & OSC_FAILURE_RESET) != 0U) {
        /* Reset caused due to oscillator failure.
         Add user code here to handle oscillator failure */

    }
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    else if ((getSysException() & WATCHDOG_RESET) != 0U) {
        /* Reset caused due
         *  1) windowed watchdog violation - Add user code here to handle watchdog violation.
         *  2) ICEPICK Reset - After loading code via CCS / System Reset through CCS
         */
        /* Check the WatchDog Status register */
        if (*WATCHDOG_STATUS != 0U) {
            /* Add user code here to handle watchdog violation. */

            /* Clear the Watchdog reset flag in Exception Status register */
            getSysException() = WATCHDOG_RESET;

        } else {
            /* Clear the ICEPICK reset flag in Exception Status register */
            getSysException() = ICEPICK_RESET;
        }
    }
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    else if ((getSysException() & CPU_RESET) != 0U) {
        /* Reset caused due to CPU reset.
         CPU reset can be caused by CPU self-test completion, or
         by toggling the "CPU RESET" bit of the CPU Reset Control Register. */
        /* clear all reset status flags */
        getSysException() = CPU_RESET;
    }
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    else if ((getSysException() & SW_RESET) != 0U) {
        /* Reset caused due to software reset.
         Add user code to handle software reset. */
    } else {
        /* Reset caused by nRST being driven low externally.
         Add user code to handle external reset. */
    }

    /* Check if there were ESM group3 errors during power-up.
     * These could occur during eFuse auto-load or during reads from flash OTP
     * during power-up. Device operation is not reliable and not recommended
     * in this case.
     * An ESM group3 error only drives the nERROR pin low. An external circuit
     * that monitors the nERROR pin must take the appropriate action to ensure that
     * the system is placed in a safe state, as determined by the application.
     */
    if ((getEsm().SR1[2]) != 0U) {
        for (;;) {
        }/* Wait */
    }

    memcpy(&__data_start, &__data_load, (&__data_end - &__data_start) * sizeof(int));
    memset(&__bss_start__, 0, (&__bss_end__ - &__bss_start__) * sizeof(int));

    __libc_init_array();

    exit(main());
}
#pragma GCC diagnostic pop
