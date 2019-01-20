#ifndef _PI_H_
#define _PI_H_


struct PiRegul;


namespace pi {

PiRegul* create();

void runSeries(PiRegul&,
         const float refValue,
         const float fbackValue,
         float& pOutValue);
void runSeries(PiRegul*,
         const float refValue,
         const float fbackValue,
         float& pOutValue);
void runParallel(PiRegul&,
         const float refValue,
         const float fbackValue,
         float& pOutValue);
void runParallel(PiRegul*,
         const float refValue,
         const float fbackValue,
         float& pOutValue);

float runSeries(PiRegul&, const float refValue, const float fbackValue);
float runSeries(PiRegul*, const float refValue, const float fbackValue);
float runParallel(PiRegul&, const float refValue, const float fbackValue);
float runParallel(PiRegul*, const float refValue, const float fbackValue);


float getFbackValue      (const PiRegul&);
float getIntegralGain    (const PiRegul&);
float getProportionalGain(const PiRegul&);
float getRefValue        (const PiRegul&);
float getIntegrator      (const PiRegul&);
float getSaturation      (const PiRegul&);

float getFbackValue      (const PiRegul*);
float getIntegralGain    (const PiRegul*);
float getProportionalGain(const PiRegul*);
float getRefValue        (const PiRegul*);
float getIntegrator      (const PiRegul*);
float getSaturation      (const PiRegul*);


void setGains(PiRegul&,
              const float Kp,
              const float Ki);
void setGains(PiRegul*,
              const float Kp,
              const float Ki);
void setIntegrator      (PiRegul&, const float Ui);
void setIntegralGain    (PiRegul&, const float Ki);
void setMinMax          (PiRegul&, const float outMin, const float outMax);
void setProportionalGain(PiRegul&, const float Kp);
void setIntegrator      (PiRegul*, const float Ui);
void setIntegralGain    (PiRegul*, const float Ki);
void setMinMax          (PiRegul*, const float outMin, const float outMax);
void setProportionalGain(PiRegul*, const float Kp);


} /* namespace pi */

#endif //end of _PI_H_ definition

