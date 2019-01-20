/*
 * getclock.cpp
 *
 *  Created on: 16 сент. 2015 г.
 *      Author: bortsov
 */



#include <ctime>
#include "rti.h"
#include "getclock.h"


static clock_t getclockImpl()
{
    return mcal::rti::get();
}

clock_t (*getclock)() = getclockImpl;
