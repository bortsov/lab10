/*
 * blockingWait.cpp
 *
 *  Created on: 28 мая 2015 г.
 *      Author: bortsov
 */

#include "blockingWait.h"
#include "getclock.h"


static void dummyFunction(){}

void waitTillusWithUserFunction(clock_t endTime_us, void (*f)())
{
    const clock_t beginTime_us = getclock();
    if (endTime_us > beginTime_us) {
        clock_t t = getclock();
        while (t < endTime_us) {
            f();
            t = getclock();
        }
    } else {
        clock_t t = getclock();
        while (t > endTime_us) {
            f();
            t = getclock();
        }
    }
}


void waitTillus(clock_t endTime_us)
{
    waitTillusWithUserFunction(endTime_us, dummyFunction);
}



void waitusWithUserFunction(clock_t t, void (*f)())
{
    const clock_t beginTime = getclock();
    const clock_t endTime = beginTime + t;
    waitTillusWithUserFunction(endTime, f);
}

void waitus(clock_t t)
{
    waitusWithUserFunction(t, dummyFunction);
}


void waitmsWithUserFunction(uint32_t t, void (*f)())
{
    waitusWithUserFunction(t * 1000, f);
}


void waitms(uint32_t t)
{
    waitusWithUserFunction(t * 1000, dummyFunction);
}

