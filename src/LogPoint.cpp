/*
 * LogPoint.cpp
 *
 *  Created on: 6 окт. 2018 г.
 *      Author: Виталий
 */

#include "LogPoint.h"


namespace logpoint {

static constexpr int MAX_POINTS = 1000;
static constexpr LogPoint ZERO_POINT = {0.0F, 0.0F, 0.0F, 0.0F};

static LogPoint buf[MAX_POINTS];
static volatile bool flagEnable;
static volatile bool flagFill;
static int nAverages;
static int nPoints = MAX_POINTS;
static int countAverages;
static volatile int idx;
static LogPoint p;


static void reset()
{
    flagEnable = false;
    flagFill = false;
    for (auto& v : buf) {
        v = ZERO_POINT;
    }
    idx = 0;
    countAverages = 0;
}


void create()
{
    reset();
    setAverages(20);
}


void destroy()
{

}


static void doSumm(const LogPoint& p_)
{
    for (int i = 0; i < N_CHANNELS; ++i) {
        p.v[i] += p_.v[i];
    }
}


static void doAverage(LogPoint& to, const LogPoint& sum)
{
    for (int i = 0; i < N_CHANNELS; ++i) {
        to.v[i] = sum.v[i] / static_cast<float>(nAverages);
    }
}


/*
 * Call this function only if there is a space to place point.
 * Otherwise memory corruption may occur.
 * Check space by calling isNeedToDoLog() function.
 */
void put(const LogPoint& p_)
{
    doSumm(p_);
    ++countAverages;
    if (countAverages >= nAverages) {
        doAverage(buf[idx], p);
        countAverages = 0;
        p = ZERO_POINT;
        ++idx;
        if (idx >= nPoints) {
            flagFill = true;
        }
    }
}


bool isLogFilled()
{
    return flagFill;
}


bool isLogEnabled()
{
    return flagEnable;
}


bool isNeedToDoLog()
{
    return isLogEnabled() && !isLogFilled();
}


const LogPoint& get(const int idx)
{
    return buf[idx];
}


void setAverages(const int nAverages_)
{
    nAverages = nAverages_;
}


int getAverages()
{
    return nAverages;
}


int getCurrentSize()
{
    return nPoints;
}


int getMaxSize()
{
    return MAX_POINTS;
}


void setSize(const int s)
{
    nPoints = s;
}


void enable()
{
    flagEnable = false;
    flagFill = false;
    countAverages = 0;
    idx = 0;
    p = ZERO_POINT;
    flagEnable = true;
}


void disable()
{
    flagEnable = false;
}

} /* namespace logpoint */

