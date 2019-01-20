
#include "modules/pi/pi.h"
#include "Motor/saturation.h"


struct PiRegul {
    float Kp;           //!< the proportional gain for the PID controller
    float Ki;           //!< the integral gain for the PID controller

    float Ui;           //!< the integrator start value for the PID controller

    float refValue;     //!< the reference input value
    float fbackValue;   //!< the feedback input value

    float outMin;       //!< the minimum output value allowwed for the PID controller
    float outMax;       //!< the maximum output value allowwed for the PID controller
};


namespace pi {


PiRegul* create()
{
    auto piHandle = new PiRegul;
    piHandle->Ui = 0.0F;
    piHandle->refValue = 0.0F;
    piHandle->fbackValue = 0.0F;

    return piHandle;
}


float getFbackValue      (const PiRegul& pi) {return pi.fbackValue;}
float getIntegralGain    (const PiRegul& pi) {return pi.Ki;}
float getProportionalGain(const PiRegul& pi) {return pi.Kp;}
float getRefValue        (const PiRegul& pi) {return pi.refValue;}
float getIntegrator      (const PiRegul& pi) {return pi.Ui;}
float getSaturation      (const PiRegul& pi) {return pi.outMax;}

float getFbackValue      (const PiRegul* pi) {return pi->fbackValue;}
float getIntegralGain    (const PiRegul* pi) {return pi->Ki;}
float getProportionalGain(const PiRegul* pi) {return pi->Kp;}
float getRefValue        (const PiRegul* pi) {return pi->refValue;}
float getIntegrator      (const PiRegul* pi) {return pi->Ui;}
float getSaturation      (const PiRegul* pi) {return pi->outMax;}


void setGains(PiRegul& pi, const float Kp, const float Ki)
{
    pi.Kp = Kp;
    pi.Ki = Ki;
}


void setGains(PiRegul* pi, const float Kp, const float Ki)
{
	setGains(*pi, Kp, Ki);
}


void setMinMax(PiRegul& pi, const float outMin, const float outMax)
{
    pi.outMin = outMin;
    pi.outMax = outMax;
}


void setMinMax(PiRegul* pi, const float outMin, const float outMax)
{
	setMinMax(*pi, outMin, outMax);
}


void setIntegrator      (PiRegul& pi, const float Ui) {pi.Ui = Ui;}
void setIntegralGain    (PiRegul& pi, const float Ki) {pi.Ki = Ki;}
void setProportionalGain(PiRegul& pi, const float Kp) {pi.Kp = Kp;}
void setIntegrator      (PiRegul* pi, const float Ui) {pi->Ui = Ui;}
void setIntegralGain    (PiRegul* pi, const float Ki) {pi->Ki = Ki;}
void setProportionalGain(PiRegul* pi, const float Kp) {pi->Kp = Kp;}



#undef CIMSIS
#ifdef CIMSIS
//! \brief     Runs the PID controller
//! \param[in] pidHandle   The PID controller handle
//! \param[in] refValue    The reference value to the controller
//! \param[in] fbackValue  The feedback value to the controller
//! \param[in] pOutValue   The pointer to the controller output value
void runSeries(PiRegul& pi,
         const float refValue,
         const float fbackValue,
         float& pOutValue)
{

    uint32_t  index;            // Index value returned by arm min/max f32 functions
    uint32_t  blocksize = 2;    // Block size for min/max value calculation
    float minArg[2];        // Arguments for min function
    float maxArg[2];        // Arguments for max function
    float maxValue;        // Output of min function
    float minValue;        // output of max function

    // Update proportional output
    const float Error = refValue - fbackValue;
    const float Up = pi.Kp * Error;           // Compute the proportional output

    // Compute the integral output with saturation
    // Handle saturation minimum bound condition
    minArg[0] = pi.Ui + pi.Ki * Up;
    minArg[1] = pi.outMax;
    arm_min_f32(minArg, blocksize, &minValue, &index);

    // Handle saturation maximum bound condition
    maxArg[0] = minValue;
    maxArg[1] = pi.outMin;
    arm_max_f32 (maxArg, blocksize, &maxValue, &index);

    pi.Ui = maxValue;
    pi.refValue = refValue;
    pi.fbackValue = fbackValue;

    // Saturate the output
    // Handle saturation minimum bound condition
    minArg[0] = Up + pi.Ui;
    minArg[1] = pi.outMax;
    arm_min_f32(minArg, blocksize, &minValue, &index);

    // Handle saturation maximum bound condition
    maxArg[0] = minValue;
    maxArg[1] = pi.outMin;
    arm_max_f32(maxArg, blocksize, &maxValue, &index);
    pOutValue = maxValue;
}
#else
//! \brief     Runs the PID controller
//! \param[in] pidHandle   The PID controller handle
//! \param[in] refValue    The reference value to the controller
//! \param[in] fbackValue  The feedback value to the controller
//! \param[in] pOutValue   The pointer to the controller output value
void runSeries(PiRegul& pi,
         const float refValue,
         const float fbackValue,
         float& pOutValue)
{
    const float Error = refValue - fbackValue;
    const float Up = pi.Kp * Error;
    const float U = pi.Ui + pi.Ki * Up;
    const float Ui = getSaturatedValue(U, pi.outMax, pi.outMin);

    pi.Ui = Ui;
    pi.refValue = refValue;
    pi.fbackValue = fbackValue;

    pOutValue = getSaturatedValue(Up + Ui, pi.outMax, pi.outMin);         // Saturate the output
}


void runParallel(PiRegul& pi,
         const float refValue,
         const float fbackValue,
         float& pOutValue)
{
    const float Error = refValue - fbackValue;
    const float Up = pi.Kp * Error;
    const float Ui = pi.Ui + pi.Ki * Error;
    const float Uis = getSaturatedValue(Ui, pi.outMax, pi.outMin);

    pi.Ui = Uis;
    pi.refValue = refValue;
    pi.fbackValue = fbackValue;

    pOutValue = getSaturatedValue(Up + Ui, pi.outMax, pi.outMin);         // Saturate the output
}


void runSeries(PiRegul* pi,
         const float refValue,
         const float fbackValue,
         float& pOutValue)
{
	runSeries(*pi, refValue, fbackValue, pOutValue);
}


void runParallel(PiRegul* pi,
         const float refValue,
         const float fbackValue,
         float& pOutValue)
{
    runParallel(*pi, refValue, fbackValue, pOutValue);
}
#endif


float runSeries(PiRegul& pi, const float refValue, const float fbackValue)
{
    const float Error = refValue - fbackValue;
    const float Up = pi.Kp * Error;
    const float Ui = pi.Ui + pi.Ki * Up;
    const float Uis = getSaturatedValue(Ui, pi.outMax, pi.outMin);

    pi.Ui = Uis;

    const float outUnsaturated = Up + Uis;
    const float out = getSaturatedValue(outUnsaturated, pi.outMax, pi.outMin);

    return out;
}


float runParallel(PiRegul& pi, const float refValue, const float fbackValue)
{
    const float Error = refValue - fbackValue;
    const float Up = pi.Kp * Error;
    const float Ui = pi.Ui + pi.Ki * Error;
    const float Uis = getSaturatedValue(Ui, pi.outMax, pi.outMin);

    pi.Ui = Uis;

    const float outUnsaturated = Up + Uis;
    const float out = getSaturatedValue(outUnsaturated, pi.outMax, pi.outMin);

    return out;
}


float runSeries(PiRegul* pi, const float refValue, const float fbackValue)
{
	return runSeries(*pi, refValue, fbackValue);

}



float runParallel(PiRegul* pi, const float refValue, const float fbackValue)
{
    return runParallel(*pi, refValue, fbackValue);

}


} /* namespace pi */
