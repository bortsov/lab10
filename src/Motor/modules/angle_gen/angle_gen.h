#ifndef _ANGLE_GEN_H_
#define _ANGLE_GEN_H_

namespace angle_gen {

float get_pu();
void run();
void setRunPeriod_s(const float ctrlPeriod_s);
void setElectricalFrequency_Hz(const float electricalFrequency_Hz);
void setInitialAngle_pu(const float a);

} /* namespace angle_gen */

#endif

