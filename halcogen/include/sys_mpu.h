#ifndef __SYS_MPU_H__
#define __SYS_MPU_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define mpuREGION1 0U
#define mpuREGION2 1U
#define mpuREGION3 2U
#define mpuREGION4 3U
#define mpuREGION5 4U
#define mpuREGION6 5U
#define mpuREGION7 6U
#define mpuREGION8 7U
#define mpuREGION9 8U
#define mpuREGION10 9U
#define mpuREGION11 10U
#define mpuREGION12 11U

#define mpuREGION_ENABLE 1U
#define mpuREGION_DISABLE 0U

#define mpuSUBREGION0_DISABLE 0x100U
#define mpuSUBREGION1_DISABLE 0x200U
#define mpuSUBREGION2_DISABLE 0x400U
#define mpuSUBREGION3_DISABLE 0x800U
#define mpuSUBREGION4_DISABLE 0x1000U
#define mpuSUBREGION5_DISABLE 0x2000U
#define mpuSUBREGION6_DISABLE 0x4000U
#define mpuSUBREGION7_DISABLE 0x8000U



/** @enum mpuRegionAccessPermission
*   @brief Alias names for mpu region access permissions
*
*   This enumeration is used to provide alias names for the mpu region access permission:
*     - MPU_PRIV_NA_USER_NA_EXEC no access in privileged mode, no access in user mode and execute
*     - MPU_PRIV_RW_USER_NA_EXEC read/write in privileged mode, no access in user mode and execute
*     - MPU_PRIV_RW_USER_RO_EXEC read/write in privileged mode, read only in user mode and execute
*     - MPU_PRIV_RW_USER_RW_EXEC read/write in privileged mode, read/write in user mode and execute
*     - MPU_PRIV_RO_USER_NA_EXEC read only in privileged mode, no access in user mode and execute
*     - MPU_PRIV_RO_USER_RO_EXEC read only in privileged mode, read only in user mode and execute
*     - MPU_PRIV_NA_USER_NA_NOEXEC no access in privileged mode, no access in user mode and no execution
*     - MPU_PRIV_RW_USER_NA_NOEXEC read/write in privileged mode, no access in user mode and no execution
*     - MPU_PRIV_RW_USER_RO_NOEXEC read/write in privileged mode, read only in user mode and no execution
*     - MPU_PRIV_RW_USER_RW_NOEXEC read/write in privileged mode, read/write in user mode and no execution
*     - MPU_PRIV_RO_USER_NA_NOEXEC read only in privileged mode, no access in user mode and no execution
*     - MPU_PRIV_RO_USER_RO_NOEXEC read only in privileged mode, read only in user mode and no execution
*
*/
enum mpuRegionAccessPermission
{
    MPU_PRIV_NA_USER_NA_EXEC   = 0x0000U, /**< Alias no access in privileged mode, no access in user mode and execute */
    MPU_PRIV_RW_USER_NA_EXEC   = 0x0100U, /**< Alias no read/write in privileged mode, no access in user mode and execute */
    MPU_PRIV_RW_USER_RO_EXEC   = 0x0200U, /**< Alias no read/write in privileged mode, read only in user mode and execute */
    MPU_PRIV_RW_USER_RW_EXEC   = 0x0300U, /**< Alias no read/write in privileged mode, read/write in user mode and execute */
    MPU_PRIV_RO_USER_NA_EXEC   = 0x0500U, /**< Alias no read only in privileged mode, no access in user mode and execute */
    MPU_PRIV_RO_USER_RO_EXEC   = 0x0600U, /**< Alias no read only in privileged mode, read only in user mode and execute */
    MPU_PRIV_NA_USER_NA_NOEXEC = 0x1000U, /**< Alias no access in privileged mode, no access in user mode and no execution */
    MPU_PRIV_RW_USER_NA_NOEXEC = 0x1100U, /**< Alias no read/write in privileged mode, no access in user mode and no execution */
    MPU_PRIV_RW_USER_RO_NOEXEC = 0x1200U, /**< Alias no read/write in privileged mode, read only in user mode and no execution */
    MPU_PRIV_RW_USER_RW_NOEXEC = 0x1300U, /**< Alias no read/write in privileged mode, read/write in user mode and no execution */
    MPU_PRIV_RO_USER_NA_NOEXEC = 0x1500U, /**< Alias no read only in privileged mode, no access in user mode and no execution */
    MPU_PRIV_RO_USER_RO_NOEXEC = 0x1600U  /**< Alias no read only in privileged mode, read only in user mode and no execution */
};

/** @enum mpuRegionType
*   @brief Alias names for mpu region type
*
*   This enumeration is used to provide alias names for the mpu region type:
*     - MPU_STRONGLYORDERED_SHAREABLE Memory type strongly ordered and sharable
*     - MPU_DEVICE_SHAREABLE          Memory type device and sharable
*     - MPU_NORMAL_OIWTNOWA_NONSHARED Memory type normal outer and inner write-through, no write allocate and non shared
*     - MPU_NORMAL_OIWTNOWA_SHARED    Memory type normal outer and inner write-through, no write allocate and shared
*     - MPU_NORMAL_OIWBNOWA_NONSHARED Memory type normal outer and inner write-back, no write allocate and non shared
*     - MPU_NORMAL_OIWBNOWA_SHARED    Memory type normal outer and inner write-back, no write allocate and shared
*     - MPU_NORMAL_OINC_NONSHARED     Memory type normal outer and inner non-cachable and non shared
*     - MPU_NORMAL_OINC_SHARED        Memory type normal outer and inner non-cachable and shared
*     - MPU_NORMAL_OIWBWA_NONSHARED   Memory type normal outer and inner write-back, write allocate and non shared
*     - MPU_NORMAL_OIWBWA_SHARED      Memory type normal outer and inner write-back, write allocate and shared
*     - MPU_DEVICE_NONSHAREABLE       Memory type device and non sharable
*/
enum mpuRegionType
{
    MPU_STRONGLYORDERED_SHAREABLE = 0x0000U, /**< Memory type strongly ordered and sharable */
    MPU_DEVICE_SHAREABLE          = 0x0001U, /**< Memory type device and sharable */
    MPU_NORMAL_OIWTNOWA_NONSHARED = 0x0002U, /**< Memory type normal outer and inner write-through, no write allocate and non shared */
    MPU_NORMAL_OIWBNOWA_NONSHARED = 0x0003U, /**< Memory type normal outer and inner write-back, no write allocate and non shared */
    MPU_NORMAL_OIWTNOWA_SHARED    = 0x0006U, /**< Memory type normal outer and inner write-through, no write allocate and shared */
    MPU_NORMAL_OIWBNOWA_SHARED    = 0x0007U, /**< Memory type normal outer and inner write-back, no write allocate and shared */
    MPU_NORMAL_OINC_NONSHARED     = 0x0008U, /**< Memory type normal outer and inner non-cachable and non shared */
    MPU_NORMAL_OIWBWA_NONSHARED   = 0x000BU, /**< Memory type normal outer and inner write-back, write allocate and non shared */
    MPU_NORMAL_OINC_SHARED        = 0x000CU, /**< Memory type normal outer and inner non-cachable and shared */
    MPU_NORMAL_OIWBWA_SHARED      = 0x000FU, /**< Memory type normal outer and inner write-back, write allocate and shared */
    MPU_DEVICE_NONSHAREABLE       = 0x0010U  /**< Memory type device and non sharable */
};

/** @enum mpuRegionSize
*   @brief Alias names for mpu region type
*
*   This enumeration is used to provide alias names for the mpu region type:
*     - MPU_STRONGLYORDERED_SHAREABLE Memory type strongly ordered and sharable
*     - MPU_32_BYTES Memory size in bytes
*     - MPU_64_BYTES Memory size in bytes
*     - MPU_128_BYTES Memory size in bytes
*     - MPU_256_BYTES Memory size in bytes
*     - MPU_512_BYTES Memory size in bytes
*     - MPU_1_KB Memory size in kB
*     - MPU_2_KB Memory size in kB
*     - MPU_4_KB Memory size in kB
*     - MPU_8_KB Memory size in kB
*     - MPU_16_KB Memory size in kB
*     - MPU_32_KB Memory size in kB
*     - MPU_64_KB Memory size in kB
*     - MPU_128_KB Memory size in kB
*     - MPU_256_KB  Memory size in kB
*     - MPU_512_KB Memory size in kB
*     - MPU_1_MB Memory size in MB
*     - MPU_2_MB Memory size in MB
*     - MPU_4_MB Memory size in MB
*     - MPU_8_MBv Memory size in MB
*     - MPU_16_MB Memory size in MB
*     - MPU_32_MB Memory size in MB
*     - MPU_64_MB Memory size in MB
*     - MPU_128_MB Memory size in MB
*     - MPU_256_MB Memory size in MB
*     - MPU_512_MB Memory size in MB
*     - MPU_1_GB Memory size in GB
*     - MPU_2_GB Memory size in GB
*     - MPU_4_GB Memory size in GB
*/
enum mpuRegionSize
{
    MPU_32_BYTES  = 0x04U << 1U, /**< Memory size in bytes */
    MPU_64_BYTES  = 0x05U << 1U, /**< Memory size in bytes */
    MPU_128_BYTES = 0x06U << 1U, /**< Memory size in bytes */
    MPU_256_BYTES = 0x07U << 1U, /**< Memory size in bytes */
    MPU_512_BYTES = 0x08U << 1U, /**< Memory size in bytes */
    MPU_1_KB      = 0x09U << 1U, /**< Memory size in kB */
    MPU_2_KB      = 0x0AU << 1U, /**< Memory size in kB */
    MPU_4_KB      = 0x0BU << 1U, /**< Memory size in kB */
    MPU_8_KB      = 0x0CU << 1U, /**< Memory size in kB */
    MPU_16_KB     = 0x0DU << 1U, /**< Memory size in kB */
    MPU_32_KB     = 0x0EU << 1U, /**< Memory size in kB */
    MPU_64_KB     = 0x0FU << 1U, /**< Memory size in kB */
    MPU_128_KB    = 0x10U << 1U, /**< Memory size in kB */
    MPU_256_KB    = 0x11U << 1U, /**< Memory size in kB */
    MPU_512_KB    = 0x12U << 1U, /**< Memory size in kB */
    MPU_1_MB      = 0x13U << 1U, /**< Memory size in MB */
    MPU_2_MB      = 0x14U << 1U, /**< Memory size in MB */
    MPU_4_MB      = 0x15U << 1U, /**< Memory size in MB */
    MPU_8_MB      = 0x16U << 1U, /**< Memory size in MB */
    MPU_16_MB     = 0x17U << 1U, /**< Memory size in MB */
    MPU_32_MB     = 0x18U << 1U, /**< Memory size in MB */
    MPU_64_MB     = 0x19U << 1U, /**< Memory size in MB */
    MPU_128_MB    = 0x1AU << 1U, /**< Memory size in MB */
    MPU_256_MB    = 0x1BU << 1U, /**< Memory size in MB */
    MPU_512_MB    = 0x1CU << 1U, /**< Memory size in MB */
    MPU_1_GB      = 0x1DU << 1U, /**< Memory size in GB */
    MPU_2_GB      = 0x1EU << 1U, /**< Memory size in GB */
    MPU_4_GB      = 0x1FU << 1U  /**< Memory size in GB */
};

void _mpuInit_(void);
void _mpuEnable_(void);
void _mpuDisable_(void);
void _mpuEnableBackgroundRegion_(void);
void _mpuDisableBackgroundRegion_(void);

/** @fn uint32_t _mpuGetNumberOfRegions_(void)
*   @brief Returns number of implemented Mpu regions
*   @return Number of implemented mpu regions
*
*   This function returns the number of implemented mpu regions.
*/
uint32_t _mpuGetNumberOfRegions_(void);

/** @fn uint32_t _mpuAreRegionsSeparate_(void)
*   @brief Returns the type of the implemented mpu regions
*   @return Mpu type of regions
*
*   This function returns 0 when mpu regions are of type unified otherwise regions are of type separate.
*/
uint32_t _mpuAreRegionsSeparate_(void);

/** @fn void _mpuSetRegion_(uint32_t region)
*   @brief Set mpu region number
*   @param[in] region Region number: mpuREGION1..mpuREGION12
*
*   This function selects one of the implemented mpu regions.
*/
void _mpuSetRegion_(uint32_t region);

/** @fn uint32_t _mpuGetRegion_(void)
*   @brief Returns the currently selected mpu region
*   @return Mpu region number
*
*   This function returns currently selected mpu region number.
*/
uint32_t _mpuGetRegion_(void);

/** @fn void _mpuSetRegionBaseAddress_(uint32_t address)
*   @brief Set base address of currently selected mpu region
*   @param[in] address Base address of the MPU region
*   @note The base address must always aligned with region size
*
*   This function sets the base address of currently selected mpu region.
*/
void _mpuSetRegionBaseAddress_(uint32_t address);

/** @fn uint32_t _mpuGetRegionBaseAddress_(void)
*   @brief Returns base address of currently selected mpu region
*   @return Current base address of selected mpu region
*
*   This function returns the base address of currently selected mpu region.
*/
uint32_t _mpuGetRegionBaseAddress_(void);

/** @fn void _mpuSetRegionTypeAndPermission_(uint32_t type, uint32_t permission)
*   @brief Set type of currently selected mpu region
*   @param[in] type Region Type
*                     - MPU_STRONGLYORDERED_SHAREABLE : Memory type strongly ordered and sharable
*                     - MPU_DEVICE_SHAREABLE          : Memory type device and sharable
*                     - MPU_NORMAL_OIWTNOWA_NONSHARED : Memory type normal outer and inner write-through, no write allocate and non shared
*                     - MPU_NORMAL_OIWBNOWA_NONSHARED : Memory type normal outer and inner write-back, no write allocate and non shared
*                     - MPU_NORMAL_OIWTNOWA_SHARED    : Memory type normal outer and inner write-through, no write allocate and shared
*                     - MPU_NORMAL_OIWBNOWA_SHARED    : Memory type normal outer and inner write-back, no write allocate and shared
*                     - MPU_NORMAL_OINC_NONSHARED     : Memory type normal outer and inner non-cachable and non shared
*                     - MPU_NORMAL_OIWBWA_NONSHARED   : Memory type normal outer and inner write-back, write allocate and non shared
*                     - MPU_NORMAL_OINC_SHARED        : Memory type normal outer and inner non-cachable and shared
*                     - MPU_NORMAL_OIWBWA_SHARED      : Memory type normal outer and inner write-back, write allocate and shared
*                     - MPU_DEVICE_NONSHAREABLE       : Memory type device and non sharable
*
*   @param[in] permission Region Access permission
*                           - MPU_PRIV_NA_USER_NA_EXEC   : Alias no access in privileged mode, no access in user mode and execute
*                           - MPU_PRIV_RW_USER_NA_EXEC   : Alias no read/write in privileged mode, no access in user mode and execute
*                           - MPU_PRIV_RW_USER_RO_EXEC   : Alias no read/write in privileged mode, read only in user mode and execute
*                           - MPU_PRIV_RW_USER_RW_EXEC   : Alias no read/write in privileged mode, read/write in user mode and execute
*                           - MPU_PRIV_RO_USER_NA_EXEC   : Alias no read only in privileged mode, no access in user mode and execute
*                           - MPU_PRIV_RO_USER_RO_EXEC   : Alias no read only in privileged mode, read only in user mode and execute
*                           - MPU_PRIV_NA_USER_NA_NOEXEC : Alias no access in privileged mode, no access in user mode and no execution
*                           - MPU_PRIV_RW_USER_NA_NOEXEC : Alias no read/write in privileged mode, no access in user mode and no execution
*                           - MPU_PRIV_RW_USER_RO_NOEXEC : Alias no read/write in privileged mode, read only in user mode and no execution
*                           - MPU_PRIV_RW_USER_RW_NOEXEC : Alias no read/write in privileged mode, read/write in user mode and no execution
*                           - MPU_PRIV_RO_USER_NA_NOEXEC : Alias no read only in privileged mode, no access in user mode and no execution
*                           - MPU_PRIV_RO_USER_RO_NOEXEC : Alias no read only in privileged mode, read only in user mode and no execution
*
*   This function sets the type of currently selected mpu region.
*/
void _mpuSetRegionTypeAndPermission_(uint32_t type, uint32_t permission);

/** @fn uint32_t _mpuGetRegionType_(void)
*   @brief Returns the type of currently selected mpu region
*   @return Current type of selected mpu region
*
*   This function returns the type of currently selected mpu region.
*/
uint32_t _mpuGetRegionType_(void);

/** @fn uint32_t _mpuGetRegionPermission_(void)
*   @brief Returns permission of currently selected mpu region
*   @return Current type of selected mpu region
*
*   This function returns permission of currently selected mpu region.
*/
uint32_t _mpuGetRegionPermission_(void);

/** @fn void _mpuSetRegionSizeRegister_(uint32_t value)
*   @brief Set mpu region size register value
*   @param[in] value Value to be written in the MPU Region Size and Enable register
*
*   This function sets mpu region size register value.
*
*   Sample usuage:
*     _mpuSetRegion_(mpuREGION5);
*     _mpuSetRegionSizeRegister_(mpuREGION_ENABLE | MPU_16_KB | mpuSUBREGION3_DISABLE | mpuSUBREGION4_DISABLE);
*/
void _mpuSetRegionSizeRegister_(uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
