/*
 * SciDmaUart.h
 *
 *  Created on: 8 янв. 2018 г.
 *      Author: Виталий
 */

#ifndef INCLUDE_SCIDMAUART_H_
#define INCLUDE_SCIDMAUART_H_

#include <cstdint>
#include <cstddef>


namespace linscidma {

void create();
void setTxPointer(const uint8_t* data);
const uint8_t* getTxPointer();
void setTxSize(size_t size);
void startTx();
bool isLastTxDone();
void waitForEndTransfer();

} /* namespace linscidma */


#endif /* INCLUDE_SCIDMAUART_H_ */
