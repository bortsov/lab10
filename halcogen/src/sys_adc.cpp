/*
 * sys_adc.cpp
 *
 *  Created on: 26 дек. 2017 г.
 *      Author: bortsov
 */

#include "sys_adc.h"
#include "vclk1.h"
#include "userExitCodes.h"

#include <cmath>


namespace internalAdc {

uint32_t& getLinkToSourceReg(
        volatile AdcBase& adc,
        const GROUPS group)
{
    return (group == EVENT) ?
            (uint32_t&) adc.EVSRC :
            ((group == GROUP1) ? (uint32_t&) adc.G1SRC : (uint32_t&) adc.G2SRC);
}


uint32_t& getLinkToSampleReg(
        volatile AdcBase& adc,
        const GROUPS group)
{
    return (group == EVENT) ?
            (uint32_t&) adc.EVSAMP :
            ((group == GROUP1) ? (uint32_t&) adc.G1SAMP : (uint32_t&) adc.G2SAMP);
}

uint32_t& getLinkToSampledisenReg(
        volatile AdcBase& adc,
        const GROUPS group)
{
    return (group == EVENT) ?
            (uint32_t&) adc.EVSAMPDISEN :
            ((group == GROUP1) ?
                    (uint32_t&) adc.G1SAMPDISEN : (uint32_t&) adc.G2SAMPDISEN);
}


void reset(volatile AdcBase& adc)
{
    adc.RSTCR = 1;
    adc.RSTCR = 0;
}


void setNumberOfConversationsBeforeGeneratingTresholdCounterGroupIrq(
        volatile AdcBase& adc,
        const GROUPS numberGroup,
        const int numberConversations)
{
    adc.GxINTCR[numberGroup] = numberConversations;
}


void startGroupConversationForSelectedChannels(
        volatile AdcBase& adc,
        const GROUPS group,
        const int bitMaskForEnabledChannels)
{
    adc.GxSEL[group] = bitMaskForEnabledChannels;
}


void enable12BitMode(volatile AdcBase& adc)
{
    adc.OPMODECR |= 1UL << 31;
}

void setAdclk(
        volatile AdcBase& adc,
        const uint32_t speedInHZ)
{
    const float periodInSeconds = 1.0F / speedInHZ;
    const float periodInNS = 1e9 * periodInSeconds;
    const float t = vclk1::getTicksFor_ns(periodInNS);
    if (t > 32.0F) {
        exit(EXIT_ADC_INVALID_CLOCKS);
    }
    adc.CLOCKCR = std::ceil(t);
}


float getAdclkTicksFor_ns(
        volatile AdcBase& adc,
        const float sampleTime_ns)
{
    const float sampleTime_vclkTicks = vclk1::getTicksFor_ns(sampleTime_ns);
    const float numVclkTicksInOneADCLK = adc.CLOCKCR + 1;
    return sampleTime_vclkTicks / numVclkTicksInOneADCLK;
}


void setInputChannelAcquizionTime_ns(
        volatile AdcBase& adc,
        const GROUPS  group,
        const float sampleTime_ns)
{
    float numADCLKcycles = getAdclkTicksFor_ns(adc, sampleTime_ns);
    constexpr float MINIMUM_SW_VALUE = 3.0F; /* see TRM, "ADC Event Sampling Time Configuration Register" */
    if (numADCLKcycles < MINIMUM_SW_VALUE) {
        numADCLKcycles = MINIMUM_SW_VALUE;
    } else if (numADCLKcycles > 4097.0F) {
        exit(EXIT_ADC_INVALID_CHANNEL_ACQUIZION_CLOCKS);
    }
    getLinkToSampleReg(adc, group) = numADCLKcycles - 2.0F;
}


void ignoreRamOverrun(
        volatile AdcBase& adc,
        const GROUPS group)
{
    adc.GxMODECR[group] |= 1 << 4;
}

void enableAdc(volatile AdcBase& adc)
{
    adc.OPMODECR |= 1;
}

void setHardwareTriggerSource(
        volatile AdcBase& adc,
        const GROUPS group,
        const DEFAULT_EVENTS event)
{
    uint32_t& srcReg = getLinkToSourceReg(adc, group);
    uint32_t copy = srcReg;
    copy &= ~7;
    copy |= event | 8;
    srcReg = copy;
}


void setHardwareTriggerSource(
        volatile AdcBase& adc,
        const GROUPS  group,
        const OPTIONA_EVENTS event)
{
    setHardwareTriggerSource(adc, group, static_cast<DEFAULT_EVENTS>(event));
}


void enableHardwareTrigger(
        volatile AdcBase& adc,
        const GROUPS group)
{
    adc.GxMODECR[group] |= 1 << 3;
}


void disableHardwareTrigger(
        volatile AdcBase& adc,
        const GROUPS group)
{
    adc.GxMODECR[group] &= ~(1 << 3);
}



void setupBuffers(
        volatile AdcBase& adc,
        const int nBufForEventGroup,
        const int nBufForTG1,
        const int totalBufs)
{
    if (!(totalBufs == 16 || totalBufs == 32 || totalBufs == 64)) {
        exit(EXIT_ADC_INVALID_SETUP_BUFFERS);
    }
    if ((nBufForEventGroup & 1) || (nBufForTG1 & 1)) {
        exit(EXIT_ADC_INVALID_SETUP_BUFFERS);
    }
    if (nBufForEventGroup + nBufForTG1 > totalBufs) {
        exit(EXIT_ADC_INVALID_SETUP_BUFFERS);
    }
    const int ev = nBufForEventGroup / 2;
    const int tg1 = nBufForTG1 /2;
    adc.BNDCR = (ev << 16) | (tg1 + ev);
    adc.BNDEND = (totalBufs == 16) ? 0 : (totalBufs == 32 ? 1 : 2);
}


void enableCapacitorDischarge(
        volatile AdcBase& adc,
        const GROUPS group)
{
    getLinkToSampledisenReg(adc, group) |= 1;
}


void disableCapacitorDischarge(
        volatile AdcBase& adc,
        const GROUPS group)
{
    getLinkToSampledisenReg(adc, group) &= ~1;
}


void setCapacitorDischargeTime_ns(
        volatile AdcBase& adc,
        const GROUPS group,
        const float t_ns)
{
    auto& reg = getLinkToSampledisenReg(adc, group);
    auto numADCLKcycles = getAdclkTicksFor_ns(adc, t_ns);
    if (numADCLKcycles > 255.0) {
        exit(EXIT_ADC_INVALID_CAPACITOR_DISCHARGE_CLOCKS);
    }
    uint32_t copy = reg;
    copy &= ~(0xFF << 8);
    copy |= static_cast<int>(std::ceil(numADCLKcycles)) << 8;
    reg = copy;
}

void waitForBufferInitializationComplete(volatile AdcBase& adc)
{
    while (((adc.BNDEND & 0xFFFF0000U) >> 16U) != 0U) {
        ;
    }
}


void disableParity(volatile AdcBase& adc)
{
    adc.PARCR = 0x00000005U;
}


float convertCodeToVoltage(const int32_t code)
{
    constexpr float REFERENCE_VOLTAGE = 3.3F;
    constexpr float FULL_SCALE_CODE = 0xFFF;
    return static_cast<float>(code) * REFERENCE_VOLTAGE / FULL_SCALE_CODE;
}


} /* namespace internalAdc */
