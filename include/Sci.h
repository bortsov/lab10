/*
 * SCI.h
 *
 *  Created on: 11 сент. 2016 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_SCI_H_
#define INCLUDE_SCI_H_

#include <cstdint>

namespace mcal {
namespace sci {

void create();
void setRxHandler(void (*handler)(int));
void sendByte(uint8_t byte);
void enableTxDma();
void disableTxDma();

} /* namespace sci */
} /* namespace mcal */

#endif /* INCLUDE_SCI_H_ */
