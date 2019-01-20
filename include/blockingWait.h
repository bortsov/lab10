/*
 * blockingWait.h
 *
 *  Created on: 28 мая 2015 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_BLOCKINGWAIT_H_
#define INCLUDE_BLOCKINGWAIT_H_

#include <cstdint>
#include <ctime>

void waitus(clock_t t);
void waitms(uint32_t t);
void waitusWithUserFunction(clock_t t, void (*f)());
void waitmsWithUserFunction(uint32_t t, void (*f)());

void waitTillus(clock_t endTime_us);
void waitTillusWithUserFunction(clock_t endTime_us, void (*f)());

#endif /* INCLUDE_BLOCKINGWAIT_H_ */
