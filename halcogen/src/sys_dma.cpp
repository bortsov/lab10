#include "reg_dma.h"
#include "sys_dma.h"
#include "sys_vim.h"


void dmaEnable()
{
	auto& dmaREG = getDmaBase();
    dmaREG.GCTRL  = 0x00010000U; /* enable dma      */
    dmaREG.GCTRL |= 0x00000300U; /* stop at suspend */
}


void dmaDisable()
{
	auto& dmaREG = getDmaBase();

    while((dmaREG.GCTRL & DMA_GCTRL_BUSBUSY) != 0U)
    {
        ;
    }
    dmaREG.GCTRL = 0U;
}


void dmaAssignRequestToChannel(DMA_CHANNEL channel, uint32_t reqline)
{
	auto& dmaREG = getDmaBase();

    const uint32_t i = channel >> 2; /* Find the register to configure */
    uint32_t j = channel - (i << 2); /* Find the offset of the type    */
    j = 3U - j;                      /* reverse the byte order         */
    j = j << 3U;                     /* find the bit location          */

    /* mapping channel 'i' to request line 'j' */
    dmaREG.DREQASI[i] &= ~(uint32_t)((uint32_t)0xFFU << j);
    dmaREG.DREQASI[i] |= (reqline << j);
}



uint32_t dmaGetReqestForChannel(DMA_CHANNEL channel)
{
	auto& dmaREG = getDmaBase();

    const uint32_t i = channel >> 2; /* Find the register to configure */
    uint32_t j = channel - (i << 2); /* Find the offset of the type    */
    j = 3U - j;                      /* reverse the byte order         */
    j = j << 3U;                     /* find the bit location          */

    return ((dmaREG.DREQASI[i] >> j) & 0xFFU);
}


void dmaSetChannelControlRegister(DMA_CHANNEL channel,
                                  DMA_CHAIN_TYPE chainType,
                                  DMA_ACCESS_WIDTH readElementSize,
                                  DMA_ACCESS_WIDTH writeElementSize,
                                  DMA_TRANSFER_TYPE transferType,
                                  DMA_ADDRESS_TYPE addressingModeForRead,
                                  DMA_ADDRESS_TYPE addressingModeForWrite,
                                  DMA_AUTOINIT_TYPE autoinit)
{
    uint32_t data = chainType << 16;
    data |= readElementSize << 14;
    data |= writeElementSize << 12;
    data |= transferType << 8;
    data |= addressingModeForRead << 3;
    data |= addressingModeForWrite << 1;
    data |= autoinit;

    getDmaRamBase().PCP[channel].CHCTRL = data;
}


void dmaSetElementCount(DMA_CHANNEL channel, size_t nElements)
{
    auto& reg = getDmaRamBase().PCP[channel].ITCOUNT;
    uint32_t data = reg & ~(0x1FFF);
    data |= nElements;
    reg = data;
}


void dmaSetFrameCount(DMA_CHANNEL channel, size_t nFrames)
{
    auto& reg = getDmaRamBase().PCP[channel].ITCOUNT;
    uint32_t data = reg & ~(0x1FFF0000);
    data |= nFrames << 16;
    reg = data;
}


void dmaSetDestinationAddress(DMA_CHANNEL channel, uint32_t address)
{
    getDmaRamBase().PCP[channel].IDADDR = address;
}


void dmaSetSourceAddress(DMA_CHANNEL channel, uint32_t address)
{
    getDmaRamBase().PCP[channel].ISADDR = address;
}


uint32_t dmaGetSourceAddress(DMA_CHANNEL channel)
{
	return getDmaRamBase().PCP[channel].ISADDR;
}



void dmaSetPortAssignmentRegister(DMA_CHANNEL channel)
{
    constexpr uint32_t port = 4; /* use PORTB only */
    auto& dmaREG = getDmaBase();

    const uint32_t i = channel >> 3U; /* Find the register to write                    */
    uint32_t j = channel - (i << 3U); /* Find the offset of the 4th bit                */
    j = 7U - j;                       /* Reverse the order of the 4th bit offset       */
    j = j << 2U;                      /* Find the bit location of the 4th bit to write */

    dmaREG.PAR[i] &= ~(uint32_t)((uint32_t)0xFU << j);
    dmaREG.PAR[i] |= (port << j);
}


void dmaEnableChannel(DMA_CHANNEL channel, DMA_REQUEST_TYPE type)
{
    if (type == DMA_REQUEST_HW) {
        dmaEnableChannelWithHardwareRequest(channel);
    } else {
        dmaEnableChannelWithSoftwareRequest(channel);
    }
}


void dmaEnableChannelWithHardwareRequest(DMA_CHANNEL channel)
{
    getDmaBase().HWCHENAS = 1 << channel;
}


void dmaDisableChannel(DMA_CHANNEL channel)
{
    getDmaBase().HWCHENAR = 1 << channel;
    getDmaBase().SWCHENAR = 1 << channel;
}


void dmaEnableChannelWithSoftwareRequest(DMA_CHANNEL channel)
{
    getDmaBase().SWCHENAS = 1 << channel;
}


bool dmaIsChannelActiveForHardwareRequest(DMA_CHANNEL channel)
{
    return getDmaBase().HWCHENAS & (1 << channel);
}



/** @fn void dmaSetPriority(uint32_t channel, dmaPRIORITY_t priority)
*   @brief Assign Priority to the channel
*   @param[in] channel DMA channel
*   @param[in] priority Priority queue to which channel needs to be assigned
*                        - LOWPRIORITY : The selected channel will be assigned to low priority queue
*                        - HIGHPRIORITY: The selected channel will be assigned to high priority queue
*
*   This function assigns the selected priority to the selected channel
*/
/* SourceId : DMA_SourceId_007 */
/* DesignId : DMA_DesignId_007 */
/* Requirements: HL_SR173 */
void dmaSetPriority(DMA_CHANNEL channel, DMA_PRIORITY priority)
{
	auto dmaREG = &getDmaBase();

    if (priority == DMA_PRIORITY_LOW)
    {
        dmaREG->CHPRIOR = (uint32_t)1U << channel;
    }
    else
    {
        dmaREG->CHPRIOS = (uint32_t)1U << channel;
    }
}


/** @fn void dmaEnableInterrupt(uint32_t channel, dmaInterrupt_t inttype)
*   @brief Enable selected interrupt
*   @param[in] channel DMA channel
*   @param[in] inttype Interrupt to be enabled
*                       - FTC: Frame Transfer Complete Interrupt will be disabled for the selected channel
*                       - LFS: Last Frame Transfer Started Interrupt will be disabled for the selected channel
*                       - HBC: First Half Of Block Complete Interrupt will be disabled for the selected channel
*                       - BTC: Block transfer complete Interrupt will be disabled for the selected channel
*                       - BER: Bus Error Interrupt will be disabled for the selected channel
*
*   This function enables the selected interrupt for the selected channel
*/
/* SourceId : DMA_SourceId_008 */
/* DesignId : DMA_DesignId_008 */
/* Requirements: HL_SR174 */
void dmaEnableInterrupt(DMA_CHANNEL channel, DMA_INTERRUPT inttype)
{
    auto& dmaREG = getDmaBase();

    dmaREG.GCHIENAS = (uint32_t) 1U << channel;

    switch (inttype) {
    case DMA_INTERRUPT_FRAME_TRANSFER_COMPLETE:
        dmaREG.FTCINTENAS = (uint32_t) 1U << channel;
        break;
    case DMA_INTERRUPT_LAST_FRAME_TRANSFER_COMPLETE:
        dmaREG.LFSINTENAS = (uint32_t) 1U << channel;
        break;
    case DMA_INTERRUPT_FIRST_HALF_BLOCK_TRANSFER_COMPLETE:
        dmaREG.HBCINTENAS = (uint32_t) 1U << channel;
        break;
    case DMA_INTERRUPT_BLOCK_TRANSFER_COMPLETE:
        dmaREG.BTCINTENAS = (uint32_t) 1U << channel;
        break;
    default:
        break;
    }
}



/** @fn void dmaDisableInterrupt(uint32_t channel, dmaInterrupt_t inttype)
*   @brief Disable selected interrupt
*   @param[in] channel DMA channel
*   @param[in] inttype Interrupt to be disabled
*                       - FTC: Frame Transfer Complete Interrupt will be disabled for the selected channel
*                       - LFS: Last Frame Transfer Started Interrupt will be disabled for the selected channel
*                       - HBC: First Half Of Block Complete Interrupt will be disabled for the selected channel
*                       - BTC: Block transfer complete Interrupt will be disabled for the selected channel
*                       - BER: Bus Error Interrupt will be disabled for the selected channel
*
*   This function disables the selected interrupt for the selected channel
*/
/* SourceId : DMA_SourceId_009 */
/* DesignId : DMA_DesignId_009 */
/* Requirements: HL_SR175 */
void dmaDisableInterrupt(DMA_CHANNEL channel, DMA_INTERRUPT inttype)
{
    auto dmaREG = &getDmaBase();

    switch (inttype) {
    case DMA_INTERRUPT_FRAME_TRANSFER_COMPLETE:
        dmaREG->FTCINTENAR = (uint32_t) 1U << channel;
        break;
    case DMA_INTERRUPT_LAST_FRAME_TRANSFER_COMPLETE:
        dmaREG->LFSINTENAR = (uint32_t) 1U << channel;
        break;
    case DMA_INTERRUPT_FIRST_HALF_BLOCK_TRANSFER_COMPLETE:
        dmaREG->HBCINTENAR = (uint32_t) 1U << channel;
        break;
    case DMA_INTERRUPT_BLOCK_TRANSFER_COMPLETE:
        dmaREG->BTCINTENAR = (uint32_t) 1U << channel;
        break;
    default:
        break;
    }
}



/** @fn void dmaDefineRegion(dmaREGION_t region, uint32_t start_add, uint32_t end_add)
*   @brief Configure start and end address of the region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*   @param[in] start_add Start address of the the region
*   @param[in] end_add End address of the region
*
*   This function configure start and end address of the selected region
*/
/* SourceId : DMA_SourceId_010 */
/* DesignId : DMA_DesignId_010 */
/* Requirements: HL_SR176 */
void dmaDefineRegion(DMA_MPU_REGION region, uint32_t start_add, uint32_t end_add)
{
	auto dmaREG = &getDmaBase();

    dmaREG->DMAMPR[region].STARTADD = start_add;
    dmaREG->DMAMPR[region].ENDADD = end_add;
}



/** @fn void dmaEnableRegion(dmaREGION_t region, dmaRegionAccess_t access, boolean intenable)
*   @brief Enable the selected region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*   @param[in] access Access permission of the selected region
*                      - FULLACCESS
*                      - READONLY
*                      - WRITEONLY
*                      - NOACCESS
*   @param[in] intenable Interrupt to be enabled or not
*                         - INTERRUPT_ENABLE : Enable interrupt for the selected region
*                         - INTERRUPT_DISABLE: Disable interrupt for the selected region
*
*   This function enables the selected region with selected access permission with or without interrupt enable
*/
/* SourceId : DMA_SourceId_011 */
/* DesignId : DMA_DesignId_011 */
/* Requirements: HL_SR177 */
void dmaEnableRegion(DMA_MPU_REGION region, DMA_MPU_REGION_ACCESS access, bool intenable)
{
	auto dmaREG = &getDmaBase();

    dmaREG->DMAMPCTRL &= ~(uint32_t)((uint32_t)0xFFU << (region*8U));

    /* Enable the region */
    dmaREG->DMAMPCTRL |= (uint32_t)1U << (region*8U);

    /* Set access permission for the region */
    dmaREG->DMAMPCTRL |= (uint32_t)access << ((region*8U) + 1U);

    if(intenable)
    {
        /* Enable interrupt */
        dmaREG->DMAMPCTRL |= (uint32_t)1U << ((region*8U) + 3U);
    }
}



/** @fn void dmaDisableRegion(dmaREGION_t region)
*   @brief Disable the selected region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*
*   This function disables the selected region(no address checking done).
*/
/* SourceId : DMA_SourceId_012 */
/* DesignId : DMA_DesignId_012 */
/* Requirements: HL_SR178 */
void dmaDisableRegion(DMA_MPU_REGION region)
{
	auto dmaREG = &getDmaBase();

    dmaREG->DMAMPCTRL &= ~(uint32_t)((uint32_t)1U << ((uint32_t)region*8U));
}



/** @fn void dmaEnableParityCheck(void)
*   @brief Enable Parity Check
*
*   This function enables parity check
*/
/* SourceId : DMA_SourceId_013 */
/* DesignId : DMA_DesignId_013 */
/* Requirements: HL_SR179 */
void dmaEnableParityCheck(void)
{
	auto dmaREG = &getDmaBase();

    dmaREG->DMAPCR = 0xAU;
}



/** @fn void dmaDisableParityCheck(void)
*   @brief Disable Parity Check
*
*   This function disables parity check
*/
/* SourceId : DMA_SourceId_014 */
/* DesignId : DMA_DesignId_014 */
/* Requirements: HL_SR180 */
void dmaDisableParityCheck(void)
{
	auto dmaREG = &getDmaBase();

    dmaREG->DMAPCR = 0x5U;
}





