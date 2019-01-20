/*----------------------------------------------------------------------------*/
/* sys_link.ld                                                              */
/*                                                                            */
/* (c) Texas Instruments 2009-2014, All rights reserved.                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* Entry Point */
ENTRY(resetEntry)

SYS_STACK_SIZE = 0;
IRQ_STACK_SIZE = 512;
FIQ_STACK_SIZE = 0;
SVC_STACK_SIZE = 1000;
ABT_STACK_SIZE = 8;
UND_STACK_SIZE = 8;
USR_STACK_SIZE = 8;

/* 2708 байт "кучи" "кушает" программа до точки запуска основного цикла */
HEAPSIZE = 4000;

/* Specify the memory areas */
MEMORY
{
  RAM (rwx)       : ORIGIN = 0x08000000, LENGTH = 128K
}

/* Define output sections */
SECTIONS
{

    .text :
    {
        . = ALIGN(4);
        *(.intvecs)

        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */

        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        *(.rodata)
        *(.rodata*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);


        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);

    } >RAM

    .ARM.extab : 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > RAM

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > RAM
    __exidx_end = .;

    __etext = .;


    .data :
    {
        __data_load = LOADADDR (.data);
        . = ALIGN(4);
        __data_start = .;
        *(vtable)
        *(.data*)
        KEEP(*(.jcr*))
        . = ALIGN(4);
        __data_end = .;
    } > RAM

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    .stacks : {
        . = ALIGN(8);
        __irq_stack_bottom__ = . ;
        . += IRQ_STACK_SIZE;
        . = ALIGN(8);
        __irq_stack_top__ = . ;

        . = ALIGN(8);
        __fiq_stack_bottom__ = . ;
        . += FIQ_STACK_SIZE;
        . = ALIGN(8);
        __fiq_stack_top__ = . ;

        . = ALIGN(8);
        __svc_stack_bottom__ = . ;
        . += SVC_STACK_SIZE;
        . = ALIGN(8);
        __svc_stack_top__ = . ;

        . = ALIGN(8);
        __abt_stack_bottom__ = . ;
        . += ABT_STACK_SIZE;
        . = ALIGN(8);
        __abt_stack_top__ = . ;

        . = ALIGN(8);
        __und_stack_bottom__ = . ;
        . += UND_STACK_SIZE;
        . = ALIGN(8);
        __und_stack_top__ = . ;

        . = ALIGN(8);
        __sys_stack_bottom__ = . ;
        . += SYS_STACK_SIZE;
        . = ALIGN(8);
        __sys_stack_top__ = . ;
    } >RAM

    .heap : {
        heapstart = .;
        . += HEAPSIZE;
        heapend = .;
    } >RAM

  
  .ARM.attributes 0 : { *(.ARM.attributes) }
}
