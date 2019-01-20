/*
 * softwareSpi.cpp
 *
 *  Created on: 30 янв. 2018 г.
 *      Author: bortsov
 */

#include "softwareSpi.h"
#include "pin/PinInterface.h"

#include <cstddef>



namespace softspi {

static PinInterface* cs   = nullptr;
static PinInterface* clk  = nullptr;
static PinInterface* mosi = nullptr;
static PinInterface* miso = nullptr;

enum STATE {OFF, ON};



static void initForOutput(PinInterface* pin, STATE defaultState = OFF);

static void initForOutput(PinInterface* pin, STATE defaultState)
{
    if (pin->isHwInitialized() == false) {
        pin->initHw();
    }
    if (defaultState) {
        pin->on();
    } else {
        pin->off();
    }
    pin->setDirectionOutput();
}


static void initForInput(PinInterface* pin)
{
    if (pin->isHwInitialized() == false) {
        pin->initHw();
    }
    pin->setDirectionInput();
}



void create(
        PinInterface* cs_,
        PinInterface* clk_,
        PinInterface* mosi_,
        PinInterface* miso_)
{
    cs = cs_;
    clk = clk_;
    mosi = mosi_;
    miso = miso_;
    initForOutput(cs, ON);
    initForOutput(clk);
    initForOutput(mosi);
    initForInput(miso);
}



uint16_t doExchange16bit(uint16_t to)
{
    constexpr size_t N_BITS = 16;
    constexpr uint16_t MASK_FOR_MSB = 0x8000;
    uint16_t rc = 0;

    cs->off();

    for (size_t i = 0; i < N_BITS; ++i) {

        clk->on();

        if (to & MASK_FOR_MSB) {
            mosi->on();
        } else {
            mosi->off();
        }
        to = to << 1;

        rc = rc << 1;
        if (miso->getState()) {
            rc |= 1;
        }

        clk->off();
    }
    cs->on();
    mosi->off();
    return rc;
}

} /* namespace softspi */





