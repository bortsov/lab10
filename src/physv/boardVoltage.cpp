/*
 * boardVoltage.cpp
 *
 *  Created on: 23 окт. 2015 г.
 *      Author: bortsov
 */


#include "physv/boardVoltage.h"
#include "physv.h"
#include "internalAdc1.h"
#include "sys_adc.h"
#include "userExitCodes.h"


namespace physv {

namespace boardVoltage {

static constexpr size_t channel = 18;
static constexpr float K = 20.098F;




class BoardVoltage : public PhysicalValue {
public:
    virtual void convert() override {
        const auto code = getAdcCode();
        const auto voltage = ::internalAdc::convertCodeToVoltage(code);
        const float v = voltage * K;
        getValueRef() = v;
    }
    BoardVoltage() {mcal::internalAdc1::registerConvertIrqHandler(this, channel);}
};


static BoardVoltage* ptr = nullptr;


void create()
{
    if (ptr) {
    	exit(EXIT_PHYSV_BOARDVOLTAGE_IS_ALREADY_CREATED);
    }
    ptr = new BoardVoltage;
}


void destroy()
{
    ptr = nullptr;
}


float get()
{
    if (ptr) {
        return ptr->getValueRef();
    } else {
    	exit(EXIT_PHYSV_BOARDVOLTAGE_IS_NOT_CREATED);
    }
}


float& getReferenceToBoardVoltage()
{
    return ptr->getValueRef();
}

} /* namespace boardVoltage */
} /* namespace physv */



