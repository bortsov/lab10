    .syntax unified

    .cpu cortex-r4
    .fpu vfpv3-d16
    .arm
    .section .text, "ax"

    .global vPortSaveVfp
    .global vPortRestoreVfp

    .type vPortSaveVfp,     %function
    .type vPortRestoreVfp,  %function


/* Сохранение контекста VFP в указанном месте */
vPortSaveVfp:
    VSTM    R0!, {D0-D15}
    VMRS    R1, FPSCR
    STR     R1, [R0]
    bx      lr
    .size vPortSaveVfp, . - vPortSaveVfp

/* Восстановление контекста VFP из указанного места */
vPortRestoreVfp:
    VLDM    R0!, {D0-D15}
    LDR     R0, [R0]
    VMSR    FPSCR, R0
    bx      lr
    .size vPortRestoreVfp, . - vPortRestoreVfp

