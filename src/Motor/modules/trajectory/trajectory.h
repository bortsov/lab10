#ifndef _TRAJ_H_
#define _TRAJ_H_

/*
 * Интерфейсный класс TrajectroyInterface не сделан из-за лишних операций на доступ к реализации:
 * доступ к виртуальным функциям требует лишнего обращения
 */


namespace trajectory {


class Trajectory;

Trajectory* create();
void  run              (Trajectory* handle);
float getIntermediateValue      (Trajectory* handle);
float getMaxDelta      (Trajectory* handle);
float getMaxValue      (Trajectory* handle);
float getMinValue      (Trajectory* handle);
float getTargetValue   (Trajectory* handle);
void  setIntValue      (Trajectory* handle, const float intValue);
void  setMaxDelta      (Trajectory* handle, const float maxDelta);
void  setMaxValue      (Trajectory* handle, const float maxValue);
void  setMinValue      (Trajectory* handle, const float minValue);
void  setTargetValue   (Trajectory* handle, const float targetValue);

} /* namespace trajectory */

#endif // end of _TRAJ_H_ definition





