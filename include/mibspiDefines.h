/*
 * mibspi_.h
 *
 *  Created on: 22 сент. 2014 г.
 *      Author: bortsov
 */

#ifndef MIBSPI__H_
#define MIBSPI__H_

#include <cstdint>
#include <cstddef>

#include "reg_mibspi.h"

namespace mibspi {

enum MIBSPI {MibSPI1, MibSPI3, MibSPI5};


enum TRIGGER_EVENT
{
    TRGEVENT_NEVER,
    TRGEVENT_RISING,
    TRGEVENT_FALLING,
    TRGEVENT_BOTH,
    TRGEVENT_HIGH = 5,
    TRGEVENT_LOW,
    TRGEVENT_ALWAYS
};

enum trgSource {
    TRGSRC_DISABLED,
    TRGSRC_GIOA0,
    TRGSRC_GIOA1,
    TRGSRC_GIOA2,
    TRGSRC_GIOA3,
    TRGSRC_GIOA4,
    TRGSRC_GIOA5,
    TRGSRC_GIOA6,
    TRGSRC_GIOA7,
    TRGSRC_HET1_8,
    TRGSRC_HET1_10,
    TRGSRC_HET1_12,
    TRGSRC_HET1_14,
    TRGSRC_HET1_16,
    TRGSRC_HET1_18,
    TRGSRC_TICK
};

enum DATAFORMAT {
    DATAFORMAT0,
    DATAFORMAT1,
    DATAFORMAT2,
    DATAFORMAT3
};

enum CHIPSELECT {
    CS_NONE = 0xFFU,
    CS_0    = 0xFEU,
    CS_1    = 0xFDU,
    CS_2    = 0xFBU,
    CS_3    = 0xF7U,
    CS_4    = 0xEFU,
    CS_5    = 0xDFU,
    CS_6    = 0xBFU,
    CS_7    = 0x7FU
};

union TGCTRL_REG {
    uint32_t data;
    struct {
        uint32_t pcurrent:7;
        uint32_t :1;
        uint32_t pstart:7;
        uint32_t :1;
        uint32_t trigsrc:4;
        uint32_t trigevt:4;
        uint32_t :4;
        uint32_t tgtd:1;
        uint32_t prst:1;
        uint32_t oneshot:1;
        uint32_t tgena:1;
    } bits;
    operator uint32_t() const {return data;}
    TGCTRL_REG(uint32_t v) {data = v;}
    TGCTRL_REG(){}
};

union DATAFORMAT_REG {
    uint32_t data;
    struct {
        uint32_t charlen:5;
        uint32_t :3;
        uint32_t prescale:8;
        uint32_t phase:1;
        uint32_t polarity:1;
        uint32_t disableCStimers:1;
        uint32_t enableHalfduplex:1;
        uint32_t LSBfirst:1;
        uint32_t enableWaitFromENApin:1;
        uint32_t enableParity:1;
        uint32_t useOddParity:1;
        uint32_t waitDelayBetweenTransmissions:8;
    } bits;
};

class UserIrqCapable {
public:
    virtual void handlerFinishTgIrq(size_t tg) = 0;
    virtual ~UserIrqCapable(){};
};


void destroy();
void create                                     (MIBSPI nspi, void* base, void* ram);
size_t createTgWithCsHoldBetweenTransactions    (MIBSPI nspi, DATAFORMAT df, CHIPSELECT cs, size_t nWordsInRAM);
size_t createTgWithoutCsHoldBetweenTransactions (MIBSPI nspi, DATAFORMAT df, CHIPSELECT cs, size_t nWordsInRAM);
void registerFinishTgIrqHanlder                 (MIBSPI nspi, UserIrqCapable* handler, size_t tg);
uint16_t getTxData                              (MIBSPI nspi, size_t nbuf);
void startTg                                    (MIBSPI nspi, size_t group);
size_t getBeginBufferNumberForTg                (MIBSPI nspi, size_t tg);
void setTxData                                  (MIBSPI nspi, size_t nbuf, uint16_t data);
void setTriggerSourceForTg                      (MIBSPI nspi, size_t ng, trgSource source);
void setTriggerEventForTg                       (MIBSPI nspi, size_t ng, TRIGGER_EVENT type);
void resetOneShot                               (MIBSPI nspi, size_t ng);
uint16_t getRxData                              (MIBSPI nspi, size_t nbuf);


void waitForBufferInitializationComplete         (volatile MibspiBase& base);
void disableRamErrorsParityDetection             (volatile MibspiBase& base);
void setAllInterruptsLevelToLowLevel             (volatile MibspiBase& base);
void clearAllInterruptFlags                      (volatile MibspiBase& base);
void disableAllInterrupts                        (volatile MibspiBase& base);
void enable                                      (volatile MibspiBase& base);
void runPeriodicCounter                          (volatile MibspiBase& base);
void setDefaultChipSelect                        (volatile MibspiBase& base, uint8_t cs);
void setDelayForChipSelectActiveToTransmitStart_ns(volatile MibspiBase& base, uint32_t delayIn_ns);
void setDelayForTransmitEndToChipSelectInactive_ns(volatile MibspiBase& base, uint32_t delayIn_ns);
void setDataformat                               (volatile MibspiBase& base, const DATAFORMAT_REG& dfr, DATAFORMAT df);
void setPeriodicCounter                          (volatile MibspiBase& base, uint16_t v, DATAFORMAT df);


namespace test {
size_t getMaxFinishTgIrqHandlers(MIBSPI nspi);
} /* namespace test */

} /* namespace mibspi */




#endif /* MIBSPI__H_ */
