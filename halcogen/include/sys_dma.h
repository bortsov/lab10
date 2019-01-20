#ifndef __DMA_H__
#define __DMA_H__

#include <cstdint>
#include <cstddef>

enum DMA_TRANSFER_TYPE {
    DMA_FRAME_TRANSFER,
    DMA_BLOCK_TRANSFER
};

enum DMA_ADDRESS_TYPE {
    DMA_ADDRESS_TYPE_FIXED,
    DMA_ADDRESS_TYPE_INC1,
    DMA_ADDRESS_OFFSET = 3
};

enum DMA_AUTOINIT_TYPE {
    DMA_AUTOINIT_OFF,
    DMA_AUTOINIT_ON
};

enum DMA_CHAIN_TYPE {
    DMA_CHAIN_TYPE_DISABLE,
    DMA_CHAIN_TYPE_0,
    DMA_CHAIN_TYPE_15 = 4
};



/** @def INTERRUPT_ENABLE
*   @brief Alias name for Interrupt enable
*   @note @note This value should be used for API argument @a intenable
*/
#define INTERRUPT_ENABLE	1U

/** @def INTERRUPT_DISABLE
*   @brief Alias name for Interrupt disable
*   @note @note This value should be used for API argument @a intenable
*/
#define INTERRUPT_DISABLE	0U


/** @def DMA_GCTRL_BUSBUSY
*   @brief Bit mask for BUS BUSY in GCTRL Register
*   @note @note This value should be used for API argument @a intenable
*/
#define DMA_GCTRL_BUSBUSY	(0x00004000U)


enum DMA_REQUEST_TYPE
{
    DMA_REQUEST_HW = 0x0U,  /**<  Hardware trigger  */
    DMA_REQUEST_SW = 0x1U   /**<  Software trigger  */
};


enum DMA_CHANNEL
{
    DMA_CHANNEL_0  = 0x00U,
    DMA_CHANNEL_1  = 0x01U,
    DMA_CHANNEL_2  = 0x02U,
    DMA_CHANNEL_3  = 0x03U,
    DMA_CHANNEL_4  = 0x04U,
    DMA_CHANNEL_5  = 0x05U,
    DMA_CHANNEL_6  = 0x06U,
    DMA_CHANNEL_7  = 0x07U,
    DMA_CHANNEL_8  = 0x08U,
    DMA_CHANNEL_9  = 0x09U,
    DMA_CHANNEL_10 = 0x0AU,
    DMA_CHANNEL_11 = 0x0BU,
    DMA_CHANNEL_12 = 0x0CU,
    DMA_CHANNEL_13 = 0x0DU,
    DMA_CHANNEL_14 = 0x0EU,
    DMA_CHANNEL_15 = 0x0FU,
    DMA_CHANNEL_16 = 0x10U,
    DMA_CHANNEL_17 = 0x11U,
    DMA_CHANNEL_18 = 0x12U,
    DMA_CHANNEL_19 = 0x13U,
    DMA_CHANNEL_20 = 0x14U,
    DMA_CHANNEL_21 = 0x15U,
    DMA_CHANNEL_22 = 0x16U,
    DMA_CHANNEL_23 = 0x17U,
    DMA_CHANNEL_24 = 0x18U,
    DMA_CHANNEL_25 = 0x19U,
    DMA_CHANNEL_26 = 0x1AU,
    DMA_CHANNEL_27 = 0x1BU,
    DMA_CHANNEL_28 = 0x1CU,
    DMA_CHANNEL_29 = 0x1DU,
    DMA_CHANNEL_30 = 0x1EU,
    DMA_CHANNEL_31 = 0x1FU
};


enum DMA_ACCESS_WIDTH
{
	DMA_ACCESS_WIDTH_8_BIT = 0U,
	DMA_ACCESS_WIDTH_16_BIT = 1U,
	DMA_ACCESS_WIDTH_32_BIT = 2U,
	DMA_ACCESS_WIDTH_64_BIT = 3U
};



enum DMA_PRIORITY
{
	DMA_PRIORITY_LOW = 0U,
	DMA_PRIORITY_HIGH = 1U
};


enum DMA_MPU_REGION
{
	DMA_MPU_REGION0 = 0U,
	DMA_MPU_REGION1 = 1U,
	DMA_MPU_REGION2 = 2U,
	DMA_MPU_REGION3 = 3U
};


enum DMA_MPU_REGION_ACCESS
{
	DMA_MPU_REGION_FULLACCESS = 0U,
	DMA_MPU_REGION_READONLY   = 1U,
	DMA_MPU_REGION_WRITEONLY  = 2U,
	DMA_MPU_REGION_NOACCESS   = 3U
};



enum DMA_INTERRUPT
{
	DMA_INTERRUPT_FRAME_TRANSFER_COMPLETE = 1U, /**<  Frame transfer complete Interrupt      */
	DMA_INTERRUPT_LAST_FRAME_TRANSFER_COMPLETE = 2U, /**<  Last frame transfer started Interrupt  */
	DMA_INTERRUPT_FIRST_HALF_BLOCK_TRANSFER_COMPLETE = 3U, /**<  First half of block complete Interrupt */
	DMA_INTERRUPT_BLOCK_TRANSFER_COMPLETE = 4U /**<  Block transfer complete Interrupt      */
};


void dmaEnable();
void dmaDisable();

void dmaSetPortAssignmentRegister(DMA_CHANNEL channel);
void dmaSetChannelControlRegister(DMA_CHANNEL channel,
                                  DMA_CHAIN_TYPE chainType,
                                  DMA_ACCESS_WIDTH readElementSize,
                                  DMA_ACCESS_WIDTH writeElementSize,
                                  DMA_TRANSFER_TYPE transferType,
                                  DMA_ADDRESS_TYPE addressingModeForRead,
                                  DMA_ADDRESS_TYPE addressingModeForWrite,
                                  DMA_AUTOINIT_TYPE autoinit);
void dmaSetElementCount(DMA_CHANNEL channel, size_t nElements);
void dmaSetFrameCount(DMA_CHANNEL channel, size_t nFrames);
void dmaSetDestinationAddress(DMA_CHANNEL channel, uint32_t address);
void dmaSetSourceAddress(DMA_CHANNEL channel, uint32_t address);
void dmaEnableChannelWithHardwareRequest(DMA_CHANNEL channel);
void dmaDisableChannel(DMA_CHANNEL channel);
void dmaEnableChannelWithSoftwareRequest(DMA_CHANNEL channel);
bool dmaIsChannelActiveForHardwareRequest(DMA_CHANNEL channel);


void dmaEnableChannel(DMA_CHANNEL channel, DMA_REQUEST_TYPE type);
void dmaAssignRequestToChannel(DMA_CHANNEL channel, uint32_t reqline);
uint32_t dmaGetReqestForChannel(DMA_CHANNEL channel);
void dmaSetPriority(DMA_CHANNEL channel, DMA_PRIORITY priority);
void dmaEnableInterrupt(DMA_CHANNEL channel, DMA_INTERRUPT inttype);
void dmaDisableInterrupt(DMA_CHANNEL channel, DMA_INTERRUPT inttype);
void dmaDefineRegion(DMA_MPU_REGION region, uint32_t start_add,
		uint32_t end_add);
void dmaEnableRegion(DMA_MPU_REGION region, DMA_MPU_REGION_ACCESS access,
		bool intenable);
void dmaDisableRegion(DMA_MPU_REGION region);
void dmaEnableParityCheck();
void dmaDisableParityCheck();

uint32_t dmaGetSourceAddress(DMA_CHANNEL channel);

#endif
