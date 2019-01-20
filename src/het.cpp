/* имеется несоответствие в техническом описании RM46x TRM и RM46L852:
 * в RM46x TRM говорится о прерываниях Level1, Level2 (настраиваемых через HETPRY и определяемых через HETOFF1, HETOFF2)
 * а в RM46L852 говорится о прерываниях Level0, Level1.
 * Экспериментально выявлено, что Level2 из TRM соответствует Level1 из описания RM46L852:
 *   при бите HETPRY=0 (Level2) меняется HETOFF2 (Level2) и срабатывает вектор №24 (N2HET1 level1 interrupt)
 * Во избежание путаницы в коде используются названия из RM46x TRM (Level1, Level2)
 */

#include "het.h"
#include "init.h"
#include "reg_het.h"
#include "userExitCodes.h"

#include <cstring>
#include <string>


namespace het {
static constexpr uint32_t MAX_NUMBER_N2HET_INSTRUCTIONS = 160;
static constexpr uint32_t MAX_NUMBER_N2HET_BLOCKS = 2;
static class Het* het[MAX_NUMBER_N2HET_BLOCKS] = {nullptr};


struct DefinesForHet {
    irq::IRQNUMBER irqNumber;
    volatile HetBase& base;
    volatile HetRam& ram;
    DefinesForHet& operator=(const DefinesForHet& rhs) {
        memcpy(this, &rhs, sizeof(rhs));
        return *this;
    }
};

static DefinesForHet defhet[MAX_NUMBER_N2HET_BLOCKS] = {
        {irq::IRQ_HET1_LEVEL_2, getHet1(), getHet1Ram()},
        {irq::IRQ_HET2_LEVEL_2, getHet2(), getHet2Ram()}
};


union HetPfr {
    uint32_t d;
    struct {
        uint32_t HRPFC:6;
        uint32_t :2;
        uint32_t LRPFC:3;
    } bits;
    HetPfr(const uint32_t numberHRclocks) {
        d = 0;
        int i;
        for (i = 0; i < 8; i++) {
            if (numberHRclocks & (1 << i)) {
                break;
            }
        }
        bits.LRPFC = i;
    }
    HetPfr(){d = 0;}
};


class Het : private ::irq::IrqCapable {
    const HET nHet;
    virtual bool handlerIrq() override;
    static constexpr int MAX_NUMBER_USER_IRQ_HANDLERS = 6;
    struct IrqHandler {
        UserIrqHandler irqHandler;
        uint32_t address;
    } finishIrq[MAX_NUMBER_USER_IRQ_HANDLERS];
    bool dispatchUserIrqHandler(const uint32_t address);
public:
    Het(const HET nHet,
        const char* program,
        const uint32_t size,
        const uint32_t nHrClocks,
        const irq::IRQTYPE irqt);
    ~Het();
    void registerUserIrqHandler(
            UserIrqHandler handler,
            const uint32_t address,
            const bool flagEnableIrq = true);
    void enableUserIrq(const uint32_t address);
    void disableUserIrq(const uint32_t address);
};

static bool isHrClocksInvalid(const uint32_t hrc)
{
    uint32_t cntOnesInHrClocks = 0;
    for (uint32_t i = 0; i < 8; i++) {
        if (hrc & (1 << i)) {
            cntOnesInHrClocks++;
        }
    }
    return (cntOnesInHrClocks != 1);
}

Het::Het(
        const HET nHet,
        const char* program,
        const uint32_t size,
        const uint32_t nHrClocks,
        const irq::IRQTYPE irqt) :
        IrqCapable(defhet[nHet].irqNumber, irqt), nHet(nHet)
{
    memset(finishIrq, 0, sizeof(finishIrq));
    const HetPfr pfr(nHrClocks);
    defhet[nHet].base.PFR = pfr.d;
    defhet[nHet].base.GCR = 0x00470000; /* смотри TRM на МК для понимания */
    loadProgram(nHet, program, size);
}

Het::~Het()
{
    defhet[nHet].base.GCR &= ~1;
}


void Het::registerUserIrqHandler(
        UserIrqHandler handler,
        const uint32_t address,
        const bool flagEnableIrq)
{
    for (auto& v : finishIrq) {
        if (v.irqHandler == nullptr) {
            v.irqHandler = handler;
            v.address = address & 0x1F;
            if (flagEnableIrq) {
                enableUserIrq(address);
            }
            return;
        }
    }
    exit(EXIT_HET_NO_SPACE_TO_REGISTER_USER_IRQ);
}


void Het::enableUserIrq(const uint32_t address)
{
    const uint32_t ad2 = address & 0x1F;
    defhet[nHet].base.INTENAS = 1 << ad2;
}


void Het::disableUserIrq(const uint32_t address)
{
    const uint32_t ad2 = address & 0x1F;
    defhet[nHet].base.INTENAC = 1 << ad2;
    defhet[nHet].base.FLG = 1 << ad2;
}


bool Het::handlerIrq()
{
    return dispatchUserIrqHandler(defhet[nHet].base.OFF2);
}


bool Het::dispatchUserIrqHandler(const uint32_t ad2)
{
    if (!ad2) {
        return false;
    }
    if (ad2 <= 0x20) { /* смотри TRM; при значении выше 32 возникают ошибки в программе HET */
        const uint32_t address = ad2 - 1;
        const bool en = (1 << address) & defhet[nHet].base.INTENAS;
        if (en) {
            for (const auto& v : finishIrq) {
                if (v.irqHandler && v.address == address) {
                    return v.irqHandler(address);
                }
            }
        }
    } /* else error handler */
    return false;
}








void create(
        const HET nHet,
        const uint32_t nHrClocksHet,
        const char* programHet,
        const uint32_t sizeProgramHet,
        const irq::IRQTYPE irqt)
{
    if (isHrClocksInvalid(nHrClocksHet)) {
        exit(EXIT_HET_INVALID_HR_CLOCKS);
    }
    if (!sizeProgramHet) {
        exit(EXIT_HET_INVALID_PROGRAM_SIZE);
    }
    if (!programHet) {
        exit(EXIT_HET_INVALID_PROGRAM_ADDRESS);
    }
    het[nHet] = new Het(
            nHet,
            programHet,
            sizeProgramHet,
            nHrClocksHet,
            irqt);
}


void destroy(const HET nHet)
{
    delete het[nHet];
}


void turnHetOn(const HET nHet)
{
    defhet[nHet].base.GCR |= 1;
}


void loadProgram(
        const HET nHet,
        const void* program,
        const uint32_t size)
{
    memcpy((void *)&defhet[nHet].ram, program, size);
}


void setupPinForOutput(const HET nHet, const Hetpin nPin)
{
    defhet[nHet].base.DIR |= 1 << nPin;
}


void clearPin(const HET nHet, const Hetpin nPin)
{
    defhet[nHet].base.DCLR |= 1 << nPin;
}


void setPin(const HET nHet, const Hetpin nPin)
{
    defhet[nHet].base.DSET |= 1 << nPin;
}


uint32_t& getDataFieldReference(const HET nHet, const uint32_t nInstruction)
{
    return (uint32_t&)defhet[nHet].ram.Instruction[nInstruction].Data;
}


uint32_t& getControlFieldReference(const HET nHet, const uint32_t nInstruction)
{
    return (uint32_t&)defhet[nHet].ram.Instruction[nInstruction].Control;
}


uint32_t& getProgramFieldReference(const HET nHet, const uint32_t nInstruction)
{
    return (uint32_t&)defhet[nHet].ram.Instruction[nInstruction].Program;
}


uint32_t getTimeSlots(const HET nHet)
{
    HetPfr pfr;
    pfr.d = defhet[nHet].base.PFR;
    return (1 << pfr.bits.LRPFC) * (pfr.bits.HRPFC + 1);
}


uint32_t getLrpBits(const HET nHet)
{
    HetPfr pfr;
    pfr.d = defhet[nHet].base.PFR;
    return pfr.bits.LRPFC;
}


void setControlField(
        const HET nHet,
        const uint32_t nInstruction,
        const uint32_t v)
{
    getControlFieldReference(nHet, nInstruction) = v;
}


float getNumbersOfHetPeriodForUserPeriod(
        const HET nHet,
        const float period,
        const float vclk2FreqInMhz)
{
    const auto ts = getTimeSlots(nHet);
    const float T = ts / vclk2FreqInMhz / 1e6F;

    return period / T;
}


int getLowerEvenHetCyclesNumberForFrequency(
        const HET nHet,
        const float userFreqInHz,
        const float vclk2FreqInMhz)
{
    const float userPeriod = 1.0F / userFreqInHz;
    const auto exactFullCycleNumber = getNumbersOfHetPeriodForUserPeriod(nHet, userPeriod, vclk2FreqInMhz);
    return (static_cast<int>(exactFullCycleNumber) / 2) * 2;
}

float countFreqencyForHetCyclesNumber(
        const HET nHet,
        const uint32_t nHetCycles,
        const float vclk2FreqInMhz)
{
    const auto totalTimeSlots = getTimeSlots(nHet) * nHetCycles;
    return vclk2FreqInMhz * 1e6F / totalTimeSlots;
}


void updatePinNumberInControlWordForMcmpInstruction(
        const HET nHet,
        const uint32_t nInstruction,
        const Hetpin numPin)
{
    uint32_t& refControlWord = getControlFieldReference(nHet, nInstruction);
    refControlWord &= ~(0x1F << 8);
    refControlWord |= numPin << 8;
}

void updatePinNumberInProgramWordForPcntInstruction(
        const HET nHet,
        const uint32_t nInstruction,
        const Hetpin numPin)
{
    uint32_t& refProgramWord = getProgramFieldReference(nHet, nInstruction);
    refProgramWord &= ~0x1F;
    refProgramWord |= numPin;
}


void registerUserIrqHandler(
        const HET nHet,
        UserIrqHandler handler,
        const uint32_t nInstruction,
        const bool flagEnableIrq)
{
    het[nHet]->registerUserIrqHandler(handler, nInstruction, flagEnableIrq);
}


void enableUserIrq(const HET nHet, const uint32_t nInstruction)
{
    het[nHet]->enableUserIrq(nInstruction);
}


void disableUserIrq(const HET nHet, const uint32_t nInstruction)
{
    het[nHet]->disableUserIrq(nInstruction);
}


uint32_t* getPtrDin(const HET nHet)
{
    return (uint32_t *)&defhet[nHet].base.DIN;
}









namespace test {
void setHetAddress(
        const HET nHet,
        volatile void* baseAddress,
        volatile void* hetRamAddress)
{
    if (baseAddress == nullptr) {
        exit(EXIT_HET_INVALID_BASE_ADDRESS);
    }
    if (hetRamAddress == nullptr) {
        exit(EXIT_HET_INVALID_RAM_ADDRESS);
    }
    const DefinesForHet dh = {
            irq::IRQ_HET1_LEVEL_2,
            *reinterpret_cast<volatile HetBase*>(baseAddress),
            *reinterpret_cast<volatile HetRam*>(hetRamAddress)};
    defhet[nHet] = dh;
}
} /* namespace test */

}

