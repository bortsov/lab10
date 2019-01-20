    .syntax unified

    .section .text, "ax"
    .cpu cortex-r4
    .arm

    .global _pmuInit_
    .global _pmuEnableCountersGlobal_
    .global _pmuDisableCountersGlobal_
    .global _pmuResetCycleCounter_
    .global _pmuResetEventCounters_
    .global _pmuResetCounters_
    .global _pmuStartCounters_
    .global _pmuStopCounters_
    .global _pmuSetCountEvent_
    .global _pmuGetCycleCount_
    .global _pmuGetEventCount_
    .global _pmuGetOverflow_

    .type _pmuInit_,                    %function
    .type _pmuEnableCountersGlobal_,    %function
    .type _pmuDisableCountersGlobal_,   %function
    .type _pmuResetCycleCounter_,       %function
    .type _pmuResetEventCounters_,      %function
    .type _pmuResetCounters_,           %function
    .type _pmuStartCounters_,           %function
    .type _pmuStopCounters_,            %function
    .type _pmuSetCountEvent_,           %function
    .type _pmuGetCycleCount_,           %function
    .type _pmuGetEventCount_,           %function
    .type _pmuGetOverflow_,             %function



/*-------------------------------------------------------------------------------*/
@ Initialize Pmu
@ Note: It will reset all counters
@ SourceId : PMU_SourceId_001
@ DesignId : PMU_DesignId_001
@ Requirements : HL_SR484
_pmuInit_:
    stmfd sp!, {r0}
    @ set control register
    mrc   p15, #0, r0, c9, c12, #0
    orr   r0,  r0, #(1 << 4) + 6 + 1
    mcr   p15, #0, r0, c9, c12, #0
    @ clear flags
    mov   r0,  #0
    sub   r0,  r0,  #1
    mcr   p15, #0, r0, c9, c12, #3
    @ select counter 0 event
	mov   r0,  #0
    mcr   p15, #0, r0, c9, c12, #5 @ select counter
    mov   r0,  #0x11
    mcr   p15, #0, r0, c9, c13, #1 @ select event
    @ select counter 1 event
    mov   r0,  #1
    mcr   p15, #0, r0, c9, c12, #5 @ select counter
    mov   r0,  #0x11
    mcr   p15, #0, r0, c9, c13, #1 @ select event
    @ select counter 2 event
    mov   r0,  #2
    mcr   p15, #0, r0, c9, c12, #5 @ select counter
    mov   r0,  #0x11
    mcr   p15, #0, r0, c9, c13, #1 @ select event
    ldmfd sp!, {r0}
    bx    lr
    .size _pmuInit_, . -_pmuInit_


/*-------------------------------------------------------------------------------*/
@ Enable Counters Global [Cycle, Event [0..2]]
@ Note: It will reset all counters
@ SourceId : PMU_SourceId_002
@ DesignId : PMU_DesignId_002
@ Requirements : HL_SR485
_pmuEnableCountersGlobal_:
    stmfd sp!, {r0}
    mrc   p15, #0, r0, c9, c12, #0
    orr   r0,  r0, #7
    mcr   p15, #0, r0, c9, c12, #0
    ldmfd sp!, {r0}
    bx    lr
    .size _pmuEnableCountersGlobal_, . - _pmuEnableCountersGlobal_


/*-------------------------------------------------------------------------------*/
@ Disable Counters Global [Cycle, Event [0..2]]
@ SourceId : PMU_SourceId_003
@ DesignId : PMU_DesignId_003
@ Requirements : HL_SR485
_pmuDisableCountersGlobal_:
    stmfd sp!, {r0}
    mrc   p15, #0, r0, c9, c12, #0
    bic   r0,  r0, #1
    mcr   p15, #0, r0, c9, c12, #0
    ldmfd sp!, {r0}
    bx    lr
    .size _pmuDisableCountersGlobal_, . - _pmuDisableCountersGlobal_


/*-------------------------------------------------------------------------------*/
@ Reset Cycle Counter
@ SourceId : PMU_SourceId_004
@ DesignId : PMU_DesignId_004
@ Requirements : HL_SR485
_pmuResetCycleCounter_:
    stmfd sp!, {r0}
    mrc   p15, #0, r0, c9, c12, #0
    orr   r0,  r0, #4
    mcr   p15, #0, r0, c9, c12, #0
    ldmfd sp!, {r0}
    bx    lr
    .size _pmuResetCycleCounter_, . - _pmuResetCycleCounter_


/*-------------------------------------------------------------------------------*/
@ Reset Event Counters [0..2]
@ SourceId : PMU_SourceId_005
@ DesignId : PMU_DesignId_005
@ Requirements : HL_SR485
_pmuResetEventCounters_:
    stmfd sp!, {r0}
    mrc   p15, #0, r0, c9, c12, #0
    orr   r0,  r0, #2
    mcr   p15, #0, r0, c9, c12, #0
    ldmfd sp!, {r0}
    bx    lr
    .size _pmuResetEventCounters_, . - _pmuResetEventCounters_


/*-------------------------------------------------------------------------------*/
@ Reset Cycle Counter abd Event Counters [0..2]
@ SourceId : PMU_SourceId_006
@ DesignId : PMU_DesignId_006
@ Requirements : HL_SR485
_pmuResetCounters_:
    stmfd sp!, {r0}
    mrc   p15, #0, r0, c9, c12, #0
    orr   r0,  r0, #6
    mcr   p15, #0, r0, c9, c12, #0
    ldmfd sp!, {r0}
    bx    lr
    .size _pmuResetCounters_, . - _pmuResetCounters_


/*-------------------------------------------------------------------------------*/
@ Start Counters [Cycle, 0..2]
@ SourceId : PMU_SourceId_007
@ DesignId : PMU_DesignId_007
@ Requirements : HL_SR485
_pmuStartCounters_:
    mcr   p15, #0, r0, c9, c12, #1
    bx    lr
    .size _pmuStartCounters_, . - _pmuStartCounters_


/*-------------------------------------------------------------------------------*/
@ Stop Counters [Cycle, 0..2]
@ SourceId : PMU_SourceId_008
@ DesignId : PMU_DesignId_008
@ Requirements : HL_SR485
_pmuStopCounters_:
    mcr   p15, #0, r0, c9, c12, #2
    bx    lr
    .size _pmuStopCounters_, . - _pmuStopCounters_


/*-------------------------------------------------------------------------------*/
@ Set Count event
@ SourceId : PMU_SourceId_009
@ DesignId : PMU_DesignId_009
@ Requirements : HL_SR485
_pmuSetCountEvent_:
    mcr   p15, #0, r0, c9, c12, #5 @ select counter
    mcr   p15, #0, r1, c9, c13, #1 @ select event
    bx    lr
    .size _pmuSetCountEvent_, . - _pmuSetCountEvent_


/*-------------------------------------------------------------------------------*/
@ Get Cycle Count
@ SourceId : PMU_SourceId_010
@ DesignId : PMU_DesignId_010
@ Requirements : HL_SR486
_pmuGetCycleCount_:
    mrc   p15, #0, r0, c9, c13, #0
    bx    lr
    .size _pmuGetCycleCount_, . - _pmuGetCycleCount_


/*-------------------------------------------------------------------------------*/
@ Get Event Counter Count Value
@ SourceId : PMU_SourceId_011
@ DesignId : PMU_DesignId_011
@ Requirements : HL_SR486
_pmuGetEventCount_:
    mcr   p15, #0, r0, c9, c12, #5 @ select counter
    mrc   p15, #0, r0, c9, c13, #2 @ read event counter
    bx    lr
    .size _pmuGetEventCount_, . - _pmuGetEventCount_


/*-------------------------------------------------------------------------------*/
@ Get Overflow Flags
@ SourceId : PMU_SourceId_012
@ DesignId : PMU_DesignId_012
@ Requirements : HL_SR486
_pmuGetOverflow_:
    mrc   p15, #0, r0, c9, c12, #3 @ read overflow
    mov   r1,  #0
    sub   r1,  r1,  #1
    mcr   p15, #0, r1, c9, c12, #3 @ clear flags
    bx    lr
    .size _pmuGetOverflow_, . - _pmuGetOverflow_

