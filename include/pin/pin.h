/*
 * pin.h
 *
 *  Created on: 01 сент. 2015 г.
 *      Author: bortsov
 */

#ifndef SRC_PINGIO_H_
#define SRC_PINGIO_H_

#include <cstddef>

#include "pin/PinInterface.h"
#include "mibspiDefines.h"
#include "het.h"
#include "reg_gio.h"
#include "reg_can.h"
#include "reg_adc.h"
#include "reg_sci.h"

namespace pin {

PinInterface* createGioPin(volatile GioPort& baseport, size_t npin);
PinInterface* createCanRxPin(CanBase& can);
PinInterface* createCanTxPin(CanBase& can);
PinInterface* createHetPin(het::HET nhet, size_t npin);
PinInterface* createMibspiCsPin(mibspi::MIBSPI nspi, size_t nCS);
PinInterface* createMibspiSomiPin(mibspi::MIBSPI nspi, size_t nSOMI = 0);
PinInterface* createMibspiSimoPin(mibspi::MIBSPI nspi, size_t nSIMO = 0);
PinInterface* createMibspiClkPin(mibspi::MIBSPI nspi);
PinInterface* createMibspiEnaPin(mibspi::MIBSPI nspi);
PinInterface* createAdevtPin(AdcBase& adc);
PinInterface* createEclkPin();
PinInterface* createLinrxPin(SciBase& sciBase);
PinInterface* createLintxPin(SciBase& sciBase);

} /* namespace pin */

#endif /* SRC_PINGIO_H_ */
