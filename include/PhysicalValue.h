/*
 * physicalValue.h
 *
 *  Created on: 09 окт. 2014 г.
 *      Author: bortsov
 */

#ifndef PHYSICALVALUE_H_
#define PHYSICALVALUE_H_

#include <cstdint>

namespace physv {

class PhysicalValue {
    float v;
    int32_t adc_code;
public:
    float& getValueRef() {return v;}
    const int32_t& getCodeRef() {return adc_code;}
    virtual void convert() = 0;
    virtual ~PhysicalValue(){}
    void setAdcCode(int32_t code) {adc_code = code;}
    int32_t getAdcCode() const {return adc_code;}
    PhysicalValue() : v(0.0F), adc_code(0) {}
};


} /* namespace physv */



#endif /* PHYSICALVALUE_H_ */
