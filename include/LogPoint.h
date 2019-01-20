/*
 * LogPoint.h
 *
 *  Created on: 9 янв. 2018 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_LOGPOINT_H_
#define INCLUDE_LOGPOINT_H_

namespace logpoint {

static constexpr int N_CHANNELS = 4;

struct LogPoint {
    float v[N_CHANNELS];
};

void create();
void destroy();
void put(const LogPoint& p);
bool isLogFilled();
bool isLogEnabled();
bool isNeedToDoLog();
const LogPoint& get(const int idx);
void setAverages(const int nAverages);
int getAverages();
int getCurrentSize();
int getMaxSize();
void setSize(const int s);
void enable();
void disable();


} /* namespace logpoint */



#endif /* INCLUDE_LOGPOINT_H_ */
