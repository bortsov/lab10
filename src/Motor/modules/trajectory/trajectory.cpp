#include "trajectory.h"
#include "Motor/saturation.h"


namespace trajectory {


class Trajectory {
public:
    float targetValue;      //!< the target value for the trajectory
    float intermediateValue;//!< the intermediate value along the trajectory
    float minValue;         //!< the minimum value for the trajectory generator
    float maxValue;         //!< the maximum value for the trajectory generator
    float maxDelta;         //!< the maximum delta value for the trajectory generator
};


float getIntermediateValue(Trajectory* handle)  {return handle->intermediateValue;}
float getMaxDelta(Trajectory* handle)           {return handle->maxDelta;}
float getMaxValue(Trajectory* handle)           {return handle->maxValue;}
float getMinValue(Trajectory* handle)           {return handle->minValue;}
float getTargetValue(Trajectory* handle)        {return handle->targetValue;}

void setIntValue(Trajectory* handle, const float intValue)
{
    handle->intermediateValue = intValue;
}

void setMaxDelta(Trajectory* handle, const float maxDelta)
{
    handle->maxDelta = maxDelta;
}

void setMaxValue(Trajectory* handle, const float maxValue)
{
    handle->maxValue = maxValue;
}

void setMinValue(Trajectory* handle, const float minValue)
{
    handle->minValue = minValue;
}

void setTargetValue(Trajectory* handle, const float targetValue)
{
    handle->targetValue = targetValue;
}

void run(Trajectory* handle)
{
    float intValue = handle->intermediateValue;
    float error = handle->targetValue - intValue;
    float maxDelta = handle->maxDelta;

    // increment the value
    intValue += getSaturatedValue(error, maxDelta, -maxDelta);

    // bound the value
    intValue = getSaturatedValue(intValue, handle->maxValue, handle->minValue);

    // store the value
    handle->intermediateValue = intValue;
}




Trajectory* create(void)
{
    return new Trajectory;
}


} /* namespace trajectory */
