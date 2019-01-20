/*
 * internalAdc1.cpp
 *
 *  Created on: 18 окт. 2014 г.
 *      Author: bortsov
 */

#include "internalAdc1.h"

#include "PhysicalValue.h"
#include "irq.h"
#include "sys_adc.h"
#include "userExitCodes.h"


using namespace internalAdc;

namespace mcal {

namespace internalAdc1 {


static constexpr size_t MAXIMUM_CHANNELS = 24;
static constexpr GROUPS USED_GROUP = GROUP1;

static uint32_t adcData[MAXIMUM_CHANNELS];
static float adcData_mv[MAXIMUM_CHANNELS];
static class InternalAdc1* adc = nullptr;
static struct ConversationSetup {
    physv::PhysicalValue* physObjectHandler;
    size_t ch;
} convertHandlers[MAXIMUM_CHANNELS];


class InternalAdc1 : private ::irq::IrqCapable {
    volatile AdcBase& adc;
    volatile uint32_t* ram;
    void acknowledgeIrqForGroup1();
    uint32_t getDataFromRam(uint32_t offset);
    void init();
    void enableGroup1ConversationCompleteIrq();
    void startGroup1ConversationForAllChannels();
    void operator=(const InternalAdc1& );
    InternalAdc1(const InternalAdc1& );
public:
    virtual bool handlerIrq() override;  /* вынесено в публичную часть для тестирования */
    InternalAdc1(volatile AdcBase& adc, volatile uint32_t* ram);
    virtual ~InternalAdc1(){}
};




InternalAdc1::InternalAdc1(volatile AdcBase& adc, volatile uint32_t *ram) :
        IrqCapable(irq::IRQ_ADC1_GROUP_1, irq::IRQ),
        adc(adc),
        ram(ram)
{
    init();
    enableGroup1ConversationCompleteIrq();
    for (auto& ch : convertHandlers) {
        ch.physObjectHandler = nullptr;
    }
    startGroup1ConversationForAllChannels();
}

void InternalAdc1::acknowledgeIrqForGroup1()
{
    adc.GxINTFLG[1] = 9;
}

uint32_t InternalAdc1::getDataFromRam(uint32_t offset)
{
    return *(ram + offset) & 0xFFF;
}

/*
 * Обработчик IRQ по завершению преобразования
 */
bool InternalAdc1::handlerIrq()
{
    acknowledgeIrqForGroup1();
    for (size_t i = 0; i < MAXIMUM_CHANNELS; ++i) {
        adcData[i] = getDataFromRam(i);
    }
    return false;
}

void InternalAdc1::init()
{
    constexpr uint32_t ADCLK_HZ = 30'000'000;
    constexpr float acquitionTime_ns = 10000; /* for Rext=100 k and C=Cmux+Csamp=30 pF: RC = 3 uS */

    reset(adc);
    enableAdc(adc);
    setAdclk(adc, ADCLK_HZ);
    setInputChannelAcquizionTime_ns(adc, USED_GROUP, acquitionTime_ns);
    enable12BitMode(adc);
    setupBuffers(adc, 0, MAXIMUM_CHANNELS, 32);
    enableCapacitorDischarge(adc, USED_GROUP);
    setCapacitorDischargeTime_ns(adc, USED_GROUP, 1000);
    ignoreRamOverrun(adc, USED_GROUP);
    setHardwareTriggerSource(adc, USED_GROUP, RTICOMPARE0);
    enableHardwareTrigger(adc, USED_GROUP);
    waitForBufferInitializationComplete(adc);
    disableParity(adc);
}


void InternalAdc1::enableGroup1ConversationCompleteIrq()
{
    adc.GxINTENA[USED_GROUP] |= 1 << 3;
}

void InternalAdc1::startGroup1ConversationForAllChannels()
{
    constexpr uint32_t bitMaskForEnabledChannels = 0;//1 << 18;
    adc.GxSEL[USED_GROUP] = bitMaskForEnabledChannels;
}




void process()
{
    for (size_t i = 0; i < MAXIMUM_CHANNELS; ++i) {
        adcData_mv[i] = convertCodeToVoltage(adcData[i]);
        const ConversationSetup& cs = convertHandlers[i];
        if (cs.physObjectHandler) {
            cs.physObjectHandler->setAdcCode(adcData[cs.ch]);
            cs.physObjectHandler->convert();
        }
    }
}

void create(void* baseAddress, void* ramAddress)
{
    if (adc) {
        exit(EXIT_ADC_IS_ALREADY_CREATED);
    }
    adc = new InternalAdc1(
            *reinterpret_cast<volatile AdcBase*>(baseAddress),
            reinterpret_cast<volatile uint32_t*>(ramAddress));
}

void destroy()
{
    delete adc;
    adc = nullptr;
}




void registerConvertIrqHandler(physv::PhysicalValue* handler, size_t nch)
{
    if (nch >= MAXIMUM_CHANNELS) {
        exit(EXIT_ADC_INVALID_CHANNEL_TO_REGISTER_USER_IRQ);
    }
    for (auto& v : convertHandlers) {
        if (v.physObjectHandler == nullptr) {
            v.physObjectHandler = handler;
            v.ch = nch;
            return;
        } else if (v.physObjectHandler == handler) {
        	exit(EXIT_ADC_USER_IRQ_HANDLER_IS_ALREADY_REGISTERED);
        } else if (v.ch == nch) {
        	exit(EXIT_ADC_USER_IRQ_CHANNEL_IS_ALREADY_REGISTERED);
        }
    }
	exit(EXIT_ADC_NO_SPACE_TO_REGISTER_USER_IRQ);
}



uint32_t& getCode(size_t nChannel)
{
    return adcData[nChannel];
}

float& getVoltage(size_t nChannel)
{
    return adcData_mv[nChannel];
}
} /* namespace adc */
} /* namespace mcal */

