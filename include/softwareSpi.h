/*
 * softwareSpi.h
 *
 *  Created on: 30 янв. 2018 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_SOFTWARESPI_H_
#define INCLUDE_SOFTWARESPI_H_

#include <cstdint>

class PinInterface;

namespace softspi {

void create(
        PinInterface* cs,
        PinInterface* clk,
        PinInterface* mosi,
        PinInterface* miso);

uint16_t doExchange16bit(uint16_t to);

} /* namespace softspi */




#endif /* INCLUDE_SOFTWARESPI_H_ */
