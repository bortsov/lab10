
/* Include Files */

#include "reg_esm.h"
#include "esm.h"
#include "sys_vim.h"


/** @fn void esmHighInterrupt(void)
*   @brief High Level Interrupt for ESM
*/
/* SourceId : ESM_SourceId_018 */
/* DesignId : ESM_DesignId_018 */
/* Requirements : HL_SR21, HL_SR22 */
void esmHighInterrupt(void) __attribute__((target("arm"),interrupt("FIQ")));
void esmHighInterrupt(void)
{
    auto& esm = getEsm();
    uint32_t vec = esm.IOFFHR - 1U;

    if (vec < 32U) {
        esm.SR1[0U] = 1U << vec;
        esmGroup1Notification(vec);
    } else if (vec < 64U) {
        esm.SR1[1U] = 1U << (vec - 32U);
        esmGroup2Notification(vec - 32U);
    } else if (vec < 96U) {
        esm.SR4[0U] = 1U << (vec - 64U);
        esmGroup1Notification(vec - 32U);
    } else {
        esm.SR4[0U] = 0xFFFFFFFFU;
        esm.SR1[1U] = 0xFFFFFFFFU;
        esm.SR1[0U] = 0xFFFFFFFFU;
    }

}




