/*
 * sys_adc.h
 *
 *  Created on: 26 дек. 2017 г.
 *      Author: bortsov
 */

#ifndef HALCOGEN_INCLUDE_SYS_ADC_H_
#define HALCOGEN_INCLUDE_SYS_ADC_H_

#include "reg_adc.h"
#include <cstddef>


namespace internalAdc {

enum DEFAULT_EVENTS {
    AD1EVT = 0,
    N2HET1_8,
    N2HET1_10,
    RTICOMPARE0,
    N2HET1_12,
    N2HET1_14,
    GIOB_0,
    GIOB_1
};

enum OPTIONA_EVENTS {
    N2HET2_5 = 1,
    N2HET1_27,
    N2HET1_17 = 4,
    N2HET1_19,
    N2HET1_11,
    N2HET2_13
};

enum GROUPS {
    EVENT,
    GROUP1,
    GROUP2
};


void setNumberOfConversationsBeforeGeneratingTresholdCounterGroupIrq(
        volatile AdcBase& adc,
        const GROUPS g,
        const int numberConversations);
void startGroupConversationForSelectedChannels(
        volatile AdcBase& adc,
        const GROUPS group,
        const int bitMaskForEnabledChannels);
void enableHardwareTrigger(
        volatile AdcBase& adc,
        const GROUPS group);
void disableHardwareTrigger(
        volatile AdcBase& adc,
        const GROUPS group);
void setHardwareTriggerSource(
        volatile AdcBase& adc,
        const GROUPS  group,
        const DEFAULT_EVENTS event);
void setHardwareTriggerSource(
        volatile AdcBase& adc,
        const GROUPS group,
        const OPTIONA_EVENTS event);
uint32_t& getLinkToSourceReg(
        volatile AdcBase& adc,
        const GROUPS group);
uint32_t& getLinkToSampleReg(
        volatile AdcBase& adc,
        const GROUPS group);
uint32_t& getLinkToSampledisenReg(
        volatile AdcBase& adc,
        const GROUPS group);
void reset(volatile AdcBase& adc);
void enable12BitMode(volatile AdcBase& adc);
void setAdclk(
        volatile AdcBase& adc,
        const uint32_t speedInHZ);
float getAdclkTicksFor_ns(
        volatile AdcBase& adc,
        const float sampleTime_ns);
void setInputChannelAcquizionTime_ns(
        volatile AdcBase& adc,
        const GROUPS group,
        const float sampleTime_ns);
void ignoreRamOverrun(
        volatile AdcBase& adc,
        const GROUPS group);
void enableAdc(volatile AdcBase& adc);
void setupBuffers(
        volatile AdcBase& adc,
        const int nBufForEventGroup,
        const int nBufForTG1,
        const int totalBufs);
void enableCapacitorDischarge(
        volatile AdcBase& adc,
        const GROUPS group);
void disableCapacitorDischarge(
        volatile AdcBase& adc,
        const GROUPS group);
void setCapacitorDischargeTime_ns(
        volatile AdcBase& adc,
        const GROUPS group,
        const float t_ns);
void waitForBufferInitializationComplete(volatile AdcBase& adc);
void disableParity(volatile AdcBase& adc);
float convertCodeToVoltage(const int32_t code);




} /* namespace internalAdc */


#endif /* HALCOGEN_INCLUDE_SYS_ADC_H_ */
