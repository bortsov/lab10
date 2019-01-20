/*
 * testpins.h
 *
 *  Created on: 27 дек. 2017 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_TESTPINS_H_
#define INCLUDE_TESTPINS_H_

#include "pin/PinInterface.h"

namespace testpins {
enum TESTPINS {D15};

PinInterface* get(TESTPINS tp = D15);
void toggle(TESTPINS tp = D15);
void create();

} /* namespace testpins */

#endif /* INCLUDE_TESTPINS_H_ */
