/*
 * MotoCurrentMeasure.cpp
 *
 *  Created on: 26 дек. 2017 г.
 *      Author: bortsov
 */

#include "Motor/MotorCurrentMeasure.h"
#include "irq.h"
#include "reg_adc.h"
#include "sys_adc.h"
#include "rti.h"



#if 1 /* work DRV8301-KIT */
static constexpr float OFFSET_EXT_IA = 2044.72F;
static constexpr float OFFSET_EXT_IC = 2032.0F;
static constexpr float EXT_GAIN_A_ADJUST = 1.0F;
//static constexpr float EXT_GAIN_C_ADJUST = 0.9597F;
static constexpr float EXT_GAIN_C_ADJUST = 1.029F;
#endif

#if 0 /* home DRV8301-KIT */
static constexpr float OFFSET_EXT_IA = 2021.75;
static constexpr float OFFSET_EXT_IC = 2045.65;
static constexpr float EXT_GAIN_A_ADJUST = 1.036585F;
static constexpr float EXT_GAIN_C_ADJUST = 1.0F;
#endif


namespace motorCurrent {

static constexpr int N_CHANNELS_MOTOR_CURRENTS = 2;
static constexpr internalAdc::GROUPS USED_ADC_GROUP = internalAdc::EVENT;
static class InternalAdc2* adc2;

class InternalAdc2 : private ::irq::IrqCapable {
    struct Winding {
        uint32_t code;
        float offset;
        float i;
        float g;
    };
    volatile Winding windings[N_CHANNELS_MOTOR_CURRENTS];
    void (*userHandler)(const float ia, const float ic);

    volatile AdcBase& adc;
    volatile uint32_t* ram;
    void acknowledgeIrqForGroup1();
    uint32_t getDataFromRam(const uint32_t offset);
    uint32_t getDataFromFifo(const internalAdc::GROUPS g) const;
    void init();
    void enableGroup1ConversationCompleteIrq();
    void startGroup1ConversationForUsedChannels();
    void handlerScanMotorWindingsCurrentDone();
    void operator=(const InternalAdc2&);
    InternalAdc2(const InternalAdc2&);
    void loadCodeFromAdcRam();
    void callUserHandler() const;

public:
    virtual bool handlerIrq() override;  /* public for TDD */
    InternalAdc2();
    void setCallbackIrqMeasureDone(void (*handler)(const float, const float));
    float getCurrent(const WINDING_CURRENT nWinding) const {
        return windings[nWinding].i;
    }
    void setOffset(const WINDING_CURRENT nWinding, const float offset) {
        windings[nWinding].offset = offset;
    }
    void setGain(const WINDING_CURRENT nWinding, const float gain) {
        windings[nWinding].g = gain;
    }
    virtual ~InternalAdc2(){}
};


void InternalAdc2::startGroup1ConversationForUsedChannels()
{
    constexpr uint32_t bitMaskForEnabledChannels = 0
            | (1 << 8)  /* EXT_IC-FB, g = 20 */
            | (1 << 14) /* EXT_IA-FB, g = 20 */
            ;
    adc.GxSEL[USED_ADC_GROUP] = bitMaskForEnabledChannels;
}


void InternalAdc2::setCallbackIrqMeasureDone(
        void (*handler)(const float, const float))
{
    userHandler = handler;
}



void InternalAdc2::acknowledgeIrqForGroup1()
{
    adc.GxINTFLG[USED_ADC_GROUP] = 9;
}


uint32_t InternalAdc2::getDataFromRam(const uint32_t offset)
{
    return *(ram + offset) & 0xFFF;
}


uint32_t InternalAdc2::getDataFromFifo(const internalAdc::GROUPS g) const
{
    return adc.GxBUF[g].BUF0;
}


void InternalAdc2::handlerScanMotorWindingsCurrentDone()
{
    for (auto& v : windings) {
        const double delta = static_cast<double>(v.code) - v.offset;
        v.i = delta * v.g;
    }
}


/*
 * First FIFO value is lowest ADC channel.
 * As ADC2 event group is scanned only two channels, there are only two values
 * in FIFO: first value for channel 8 (EXT_IC-FB), second value for
 * channel 14 (EXT_IA-FB).
 * Channel EXT_IB-FB is very noisly
 */
void InternalAdc2::loadCodeFromAdcRam()
{
    windings[EXT_IC].code = getDataFromFifo(USED_ADC_GROUP) & 0xFFF;
    windings[EXT_IA].code = getDataFromFifo(USED_ADC_GROUP) & 0xFFF;
}


void InternalAdc2::callUserHandler() const
{
    if (userHandler) {
        const float ia = windings[EXT_IA].i;
        const float ic = windings[EXT_IC].i;
        userHandler(ia, ic);
    }
}


bool InternalAdc2::handlerIrq()
{
    loadCodeFromAdcRam();
    acknowledgeIrqForGroup1();

    const irq::VfpGuard g;
    handlerScanMotorWindingsCurrentDone();
    callUserHandler();
    return false;
}


void InternalAdc2::enableGroup1ConversationCompleteIrq()
{
    adc.GxINTENA[USED_ADC_GROUP] |= 1 << 3;
}


void InternalAdc2::init()
{
    using namespace internalAdc;

    constexpr uint32_t ADCLK_HZ = 30'000'000;

    reset(adc);
    enableAdc(adc);
    setAdclk(adc, ADCLK_HZ);
    setInputChannelAcquizionTime_ns(adc, USED_ADC_GROUP, 0);
    enable12BitMode(adc);

    /* must be even */
    constexpr int totalBuffersForEventGroup = 2;

    /* must be even; for three channels may be >=4 */
    constexpr int totalBuffersForTriggerGroup1 = 0;

    /* may be 16, 32 or 64; rest buffers belong to trigger group 2 */
    constexpr int nTotalBuffers = 16;

    setupBuffers(
            adc,
            totalBuffersForEventGroup,
            totalBuffersForTriggerGroup1,
            nTotalBuffers);

    disableCapacitorDischarge(adc, USED_ADC_GROUP);

    ignoreRamOverrun(adc, USED_ADC_GROUP);
    setHardwareTriggerSource(adc, USED_ADC_GROUP, N2HET1_19);
    enableHardwareTrigger(adc, USED_ADC_GROUP);
    waitForBufferInitializationComplete(adc);
    disableParity(adc);
}


InternalAdc2::InternalAdc2() :
        IrqCapable{irq::IRQ_MIBADC2_EVENT, irq::IRQ},
        windings{
            {0, 0.0F, 0.0F, 0.0F},
            {0, 0.0F, 0.0F, 0.0F}},
        userHandler(nullptr),
        adc{getAdc2()},
        ram{getAdc2Ram()}
{
    init();
    enableGroup1ConversationCompleteIrq();
    startGroup1ConversationForUsedChannels();
}



/*
 * Для работы данной функции необходимо иметь разрешённые прерывания,
 * которые нужны для калибровки смещения
 */
void create(const float gainMotorCurrentExt)
{
    constexpr float VOLTAGE_REFERENCE = 3.3F;
    constexpr float MAXIMUM_CODE = 4095.0F;
    constexpr float R = 0.002F;
    constexpr float K_REF = VOLTAGE_REFERENCE / (MAXIMUM_CODE * R);
    const float gainExtA = K_REF / (gainMotorCurrentExt * EXT_GAIN_A_ADJUST);
    const float gainExtC = K_REF / (gainMotorCurrentExt * EXT_GAIN_C_ADJUST);

    adc2 = new InternalAdc2();
    adc2->setOffset(EXT_IA, OFFSET_EXT_IA);
    adc2->setOffset(EXT_IC, OFFSET_EXT_IC);
    adc2->setGain(EXT_IA, gainExtA);
    adc2->setGain(EXT_IC, gainExtC);
}





void setCallbackOnConversationDoneIrq(
        void (*handler)(
                const float ia,
                const float ic))
{
    adc2->setCallbackIrqMeasureDone(handler);
}


float getCurrent(const WINDING_CURRENT nWinding)
{
    return adc2->getCurrent(nWinding);
}


void setOffset(const WINDING_CURRENT nWindings, const float offset)
{
    adc2->setOffset(nWindings, offset);
}


} /* namespace motorCurrent */
