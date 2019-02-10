#include "angle_gen.h"



namespace angle_gen {

static float ctrlPeriod_s;
static float angle_pu;
static float stepAngle_pu;



void setRunPeriod_s(const float ctrlPeriod_s_)
{
    ctrlPeriod_s = ctrlPeriod_s_;
    angle_pu = 0.0F;
    stepAngle_pu = 0.0F;
}


float get_pu()
{
    return angle_pu;
}


void setElectricalFrequency_Hz(const float electricalFrequency_Hz)
{
    stepAngle_pu = electricalFrequency_Hz * ctrlPeriod_s;
}



/*
 * рассчитывает требуемый угол в зависимости от частоты
 */
void run()
{
    float a = angle_pu + stepAngle_pu;
    if (a >= 1.0F) {
        a -= 1.0F;
    } else if (a < 0.0F) {
        a += 1.0F;
    }
    angle_pu = a;
}


void setInitialAngle_pu(const float a)
{
    angle_pu = a;
}

} /* namespace angle_gen */

