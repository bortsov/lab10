/*
 * testpins.cpp
 *
 *  Created on: 27 дек. 2017 г.
 *      Author: bortsov
 */

#include "testpins.h"
#include "pin/pin.h"
#include "userExitCodes.h"

namespace testpins {

static PinInterface* d15;

PinInterface* get(TESTPINS tp)
{
    switch (tp) {
        case D15: return d15;
        default: break;
    }
    exit(EXIT_PINS_INVALID_TESTPIN);
}

void create()
{
    d15 = pin::createHetPin(het::HET1, 1);
    d15->setDirectionOutput();
    d15->off();
}

void toggle(TESTPINS tp)
{
    auto v = get(tp);
    if (v->getState()) {
        v->off();
    } else {
        v->on();
    }
}

} /* namespace testpins */

