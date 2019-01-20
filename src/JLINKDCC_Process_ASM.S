    .syntax unified

    .section .text, "ax"
    .cpu cortex-r4
    .fpu vfpv3-d16
    .arm

    .global  CP14_ReadDCCStat
    .global  CP14_ReadDCC
    .global  CP14_WriteDCC
    .global  CP14_BlockWriteDCC

    .type CP14_ReadDCCStat, %function
    .type CP14_ReadDCC, %function
    .type CP14_WriteDCC, %function
    .type CP14_BlockWriteDCC, %function


CP14_ReadDCCStat:
    mrc p14, #0, R0, c0, c1, #0
    bx lr
    .size CP14_ReadDCCStat, . - CP14_ReadDCCStat


CP14_ReadDCC:
    mrc p14, #0, R0, c0, c5, #0
    bx lr
    .size CP14_ReadDCC, . - CP14_ReadDCC


CP14_WriteDCC:
    mcr p14, #0, r0, c0, c5, #0
    bx lr
    .size CP14_WriteDCC, . - CP14_WriteDCC


CP14_BlockWriteDCC:
    mrc 	p14, 0, PC, c0, c1, 0
    beq     CP14_BlockWriteDCC
    mcr 	p14, 0, r0, c0, c5, 0
    bx      lr
    .size CP14_BlockWriteDCC, . - CP14_BlockWriteDCC

