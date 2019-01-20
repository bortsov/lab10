/*
 * jlink_queues.h
 *
 *  Created on: 06 марта 2015 г.
 *      Author: bortsov
 */

#ifndef SRC_H_JLINK_QUEUES_H_
#define SRC_H_JLINK_QUEUES_H_


#include <cstdint>
#include <cstddef>
#include "jlink.h"

namespace jlink {

namespace queues {

void create();
void destroy();
void sendTxQueue(uint32_t data);
void sendRxQueue(uint8_t data, DEBUG_CHANNELS channel);
uint32_t readTxQueue();
uint8_t readRxQueue(DEBUG_CHANNELS channel);
bool checkEmptyTxQueue();
bool checkFullTxQueue();
bool checkEmptyRxQueue(DEBUG_CHANNELS channel);
size_t getTxBufferSize();
size_t getRxBufferSize(DEBUG_CHANNELS channel);


} /* namespace queues */

} /* namespace jlink */


#endif /* SRC_H_JLINK_QUEUES_H_ */
