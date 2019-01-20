
#ifndef __SYS_PMU_H__
#define __SYS_PMU_H__

#include <stdint.h>

/*
*	Use this macro as a parameter 'counters' in APIs _pmuStartCounters_ and _pmuStopCounters_
*/
#define pmuCOUNTER0 0x00000001U
#define pmuCOUNTER1 0x00000002U
#define pmuCOUNTER2 0x00000004U
#define pmuCYCLE_COUNTER 0x80000000U

enum pmuEvent
{
    PMU_INST_CACHE_MISS                     = 0x01U,
    PMU_DATA_CACHE_MISS                     = 0x03U,
    PMU_DATA_CACHE_ACCESS                   = 0x04U,
    PMU_DATA_READ_ARCH_EXECUTED             = 0x06U,
    PMU_DATA_WRITE_ARCH_EXECUTED            = 0x07U,
    PMU_INST_ARCH_EXECUTED                  = 0x08U,
    PMU_EXCEPTION_TAKEN                     = 0x09U,
    PMU_EXCEPTION_RETURN_ARCH_EXECUTED      = 0x0AU,
    PMU_CHANGE_TO_CONTEXT_ID_EXECUTED       = 0x0BU,
    PMU_SW_CHANGE_OF_PC_ARCH_EXECUTED       = 0x0CU,
    PMU_BRANCH_IMM_INST_ARCH_EXECUTED       = 0x0DU,
    PMU_PROC_RETURN_ARCH_EXECUTED           = 0x0EU,
    PMU_UNALIGNED_ACCESS_ARCH_EXECUTED      = 0x0FU,
    PMU_BRANCH_MISSPREDICTED                = 0x10U,
    PMU_CYCLE_COUNT                         = 0x11U,
    PMU_PREDICTABLE_BRANCHES                = 0x12U,
    PMU_INST_BUFFER_STALL                   = 0x40U,
    PMU_DATA_DEPENDENCY_INST_STALL          = 0x41U,
    PMU_DATA_CACHE_WRITE_BACK               = 0x42U,
    PMU_EXT_MEMORY_REQUEST                  = 0x43U,
    PMU_LSU_BUSY_STALL                      = 0x44U,
    PMU_FORCED_DRAIN_OFSTORE_BUFFER         = 0x45U,
    PMU_FIQ_DISABLED_CYCLE_COUNT            = 0x46U,
    PMU_IRQ_DISABLED_CYCLE_COUNT            = 0x47U,
    PMU_ETMEXTOUT_0                         = 0x48U,
    PMU_ETMEXTOUT_1                         = 0x49U,
    PMU_INST_CACHE_TAG_ECC_ERROR            = 0x4AU,
    PMU_INST_CACHE_DATA_ECC_ERROR           = 0x4BU,
    PMU_DATA_CACHE_TAG_ECC_ERROR            = 0x4CU,
    PMU_DATA_CACHE_DATA_ECC_ERROR           = 0x4DU,
    PMU_TCM_FATAL_ECC_ERROR_PREFETCH        = 0x4EU,
    PMU_TCM_FATAL_ECC_ERROR_LOAD_STORE      = 0x4FU,
    PMU_STORE_BUFFER_MERGE                  = 0x50U,
    PMU_LSU_STALL_STORE_BUFFER_FULL         = 0x51U,
    PMU_LSU_STALL_STORE_QUEUE_FULL          = 0x52U,
    PMU_INTEGER_DIV_EXECUTED                = 0x53U,
    PMU_STALL_INTEGER_DIV                   = 0x54U,
    PMU_PLD_INST_LINE_FILL                  = 0x55U,
    PMU_PLD_INST_NO_LINE_FILL               = 0x56U,
    PMU_NON_CACHEABLE_ACCESS_AXI_MASTER     = 0x57U,
    PMU_INST_CACHE_ACCESS                   = 0x58U,
    PMU_DOUBLE_DATA_CACHE_ISSUE             = 0x59U,
    PMU_DUAL_ISSUE_CASE_A                   = 0x5AU,
    PMU_DUAL_ISSUE_CASE_B1_B2_F2_F2D        = 0x5BU,
    PMU_DUAL_ISSUE_OTHER                    = 0x5CU,
    PMU_DP_FLOAT_INST_EXCECUTED             = 0x5DU,
    PMU_DUAL_ISSUED_PAIR_INST_ARCH_EXECUTED = 0x5EU,
    PMU_DATA_CACHE_DATA_FATAL_ECC_ERROR     = 0x60U,
    PMU_DATA_CACHE_TAG_FATAL_ECC_ERROR      = 0x61U,
    PMU_PROCESSOR_LIVE_LOCK                 = 0x62U,
    PMU_ATCM_MULTI_BIT_ECC_ERROR            = 0x64U,
    PMU_B0TCM_MULTI_BIT_ECC_ERROR           = 0x65U,
    PMU_B1TCM_MULTI_BIT_ECC_ERROR           = 0x66U,
    PMU_ATCM_SINGLE_BIT_ECC_ERROR           = 0x67U,
    PMU_B0TCM_SINGLE_BIT_ECC_ERROR          = 0x68U,
    PMU_B1TCM_SINGLE_BIT_ECC_ERROR          = 0x69U,
    PMU_TCM_COR_ECC_ERROR_LOAD_STORE        = 0x6AU,
    PMU_TCM_COR_ECC_ERROR_PREFETCH          = 0x6BU,
    PMU_TCM_FATAL_ECC_ERROR_AXI_SLAVE       = 0x6CU,
    PMU_TCM_COR_ECC_ERROR_AXI_SLAVE         = 0x6DU
};

extern "C" void _pmuInit_();

/** @fn void _pmuEnableCountersGlobal_(void)
*   @brief Enable and reset cycle counter and all 3 event counters
*/
extern "C" void _pmuEnableCountersGlobal_();

/** @fn void _pmuDisableCountersGlobal_(void)
*   @brief Disable cycle counter and all 3 event counters
*/
extern "C" void _pmuDisableCountersGlobal_();

/** @fn void _pmuResetCycleCounter_(void)
*   @brief Reset cycle counter
*/
extern "C" void _pmuResetCycleCounter_();

/** @fn void _pmuResetEventCounters_(void)
*   @brief Reset event counters 0-2
*/
extern "C" void _pmuResetEventCounters_();

/** @fn void _pmuResetCounters_(void)
*   @brief Reset cycle counter and event counters 0-2
*/
extern "C" void _pmuResetCounters_();

/** @fn void _pmuStartCounters_(uint32_t counters)
*   @brief Starts selected counters
*   @param[in] counters - Counter mask
*/
extern "C" void _pmuStartCounters_(uint32_t counters);

/** @fn void _pmuStopCounters_(uint32_t counters)
*   @brief Stops selected counters
*   @param[in] counters - Counter mask
*/
extern "C" void _pmuStopCounters_(uint32_t counters);

/** @fn void _pmuSetCountEvent_(uint32_t counter, uint32_t event)
*   @brief Set event counter count event
*   @param[in] counter - Counter select 0..2
*   @param[in] event   - Count event
*/
extern "C" void _pmuSetCountEvent_(uint32_t counter, uint32_t event);

/** @fn uint32_t _pmuGetCycleCount_(void)
*   @brief Returns current cycle counter value
*
*   @return cycle count.
*/
extern "C" uint32_t _pmuGetCycleCount_(void);

/** @fn uint32_t _pmuGetEventCount_(uint32_t counter)
*   @brief Returns current event counter value
*   @param[in] counter - Counter select 0..2
*
*   @return event counter count.
*/
extern "C" uint32_t _pmuGetEventCount_(uint32_t counter);

/** @fn uint32_t _pmuGetOverflow_(void)
*   @brief Returns current overflow register and clear flags
*
*   @return overflow flags.
*/
extern "C" uint32_t _pmuGetOverflow_(void);


#endif
