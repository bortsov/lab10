#ifndef HETCPP_H
#define HETCPP_H

#include <cstdint>
#include "irq.h"

namespace het {

enum HET {
    HET1, HET2
};

using Hetpin = uint32_t;
using UserIrqHandler = const bool (*)(uint32_t address);

void setupPinForOutput  (const HET nHet, const Hetpin nPin);
void setPin             (const HET nHet, const Hetpin nPin);
void clearPin           (const HET nHet, const Hetpin nPin);

void create(
        const HET nHet,
        const uint32_t nHrClocksHet,
        const char* programHet,
        const uint32_t sizeProgramHet,
        const irq::IRQTYPE irqt);
void loadProgram(
        const HET nHet,
        const void* program,
        const uint32_t size);

void destroy    (const HET nHet);
void turnHetOn  (const HET nHet);

uint32_t getTimeSlots   (const HET nHet);
uint32_t getLrpBits     (const HET nHet);
uint32_t* getPtrDin     (const HET nHet);

uint32_t& getDataFieldReference     (const HET nHet, const uint32_t nInstruction);
uint32_t& getControlFieldReference  (const HET nHet, const uint32_t nInstruction);
uint32_t& getProgramFieldReference  (const HET nHet, const uint32_t nInstruction);

void setControlField(
        const HET nHet,
        const uint32_t nInstruction,
        const uint32_t v);
float getNumbersOfHetPeriodForUserPeriod(
        const HET nHet,
        const float period,
        const float vclk2FreqInMhz);
int getLowerEvenHetCyclesNumberForFrequency(
        const HET nHet,
        const float userFreqInHz,
        const float vclk2FreqInMhz);
float countFreqencyForHetCyclesNumber(
        const HET nHet,
        const uint32_t nHetCycles,
        const float vclk2FreqInMhz);
void updatePinNumberInControlWordForMcmpInstruction(
        const HET nHet,
        const uint32_t nInstruction,
        const Hetpin nPin);
void updatePinNumberInProgramWordForPcntInstruction(
        const HET nHet,
        const uint32_t nInstruction,
        const Hetpin nPin);
void registerUserIrqHandler(
        const HET nHet,
        UserIrqHandler handler,
        const uint32_t nInstruction,
        const bool flagEnableIrq);

void enableUserIrq  (const HET nHet, const uint32_t nInstruction);
void disableUserIrq (const HET nHet, const uint32_t nInstruction);

namespace test {
void setHetAddress(
        const HET nHet,
        volatile void *baseAddress,
        volatile void *hetRamAddress);
} /* namespace test */

} /* namespace het */

#endif
