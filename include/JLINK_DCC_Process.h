/*
 * JLINK_DCC_Process.h
 *
 *  Created on: 06 марта 2015 г.
 *      Author: bortsov
 */

#ifndef SRC_H_JLINK_DCC_PROCESS_H_
#define SRC_H_JLINK_DCC_PROCESS_H_

#include <cstdint>

extern "C" void CP14_WriteDCC(uint32_t);
extern "C" void CP14_BlockWriteDCC(uint32_t);
extern "C" uint32_t CP14_ReadDCC();
extern "C" uint32_t CP14_ReadDCCStat();
uint32_t DCCIsTxEmpty();
uint32_t DCCIsNewCharRdy();


#endif /* SRC_H_JLINK_DCC_PROCESS_H_ */
