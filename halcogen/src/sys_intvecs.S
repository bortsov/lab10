
    .syntax unified

    .cpu cortex-r4
    .arm

    .section .intvecs,"a",%progbits

    .global resetEntry

    .type resetEntry, %object

    .extern _c_int00
    .extern vPortSWI
    .extern _dabort
    .extern phantomInterruptFromISR
    .extern undefEntry_c
    .extern prefetchEntry_c

/*-------------------------------------------------------------------------------*/
@ interrupt vectors

resetEntry:
    ldr     pc, [pc, #24]     /* адрес 0x00, RESET,    load the secondary vector */
    ldr     pc, [pc, #24]     /* адрес 0x04, UNDEF,    load the secondary vector */
    ldr     pc, [pc, #24]     /* адрес 0x08, SWI,      load the secondary vector */
    ldr     pc, [pc, #24]     /* адрес 0x0C, PREFETCH, load the secondary vector */
    ldr     pc, [pc, #24]     /* адрес 0x10, DABT,     load the secondary vector */
    ldr     pc, [pc, #24]     /* адрес 0x14, PHANTOM,  load the secondary vector */

@    ldr     pc, [pc, #24]    /* адрес 0x18, ISR,      load the secondary vector */
@    ldr     pc, [pc, #24]    /* адрес 0x1C, FIQ,      load the secondary vector */

    ldr     pc, [pc,#-0x1b0]  /* адрес 0x18, IRQ auto-vectorization, требуется __attribute__ ((interrupt("IRQ"), target("arm"))) для обработчиков IRQ */
    ldr     pc, [pc,#-0x1b0]  /* адрес 0x1C, FIQ auto-vectorization, требуется __attribute__ ((interrupt("FIQ"), target("arm"))) для обработчиков FIQ */
    .size resetEntry, .-resetEntry

VECT_secondary_table:
    .word   _c_int01
    .word   undefEntry
    .word   vPortSWI
    .word   prefetchEntry
    .word   _dabort
    .word   phantomInterruptFromIsr
    .word   irqEntry
    .word   fiqEntry

    .section .text
_c_int01:
    blx   _c_int00

undefEntry:
    blx  undefEntry_c

prefetchEntry:
    blx  prefetchEntry_c

vPortSWI:
    b   .

/* must not been called because of IRQ hand-shake between VIM and CPU */
irqEntry:
    b   .


/* must not been called because of no FIQ in working program */
fiqEntry:
    b   .
