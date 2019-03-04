/*
 * init.h
 *
 *  Created on: 14 сент. 2015 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_INIT_INIT_H_
#define INCLUDE_INIT_INIT_H_

namespace init {

void pll();
void peripheral();
void flash();
void trimLpo();
void mapClocks();
void vim();
void vimSetAllHandlerToPhantomInterrupt();
void rti();
void mibspi();
void internalAdc1();

} /* namespace init */

extern "C" void registerIrqVectorInVim(const int nIrq, void (* const vector)());
extern "C" void registerFiqVectorInVim(const int nIrq, void (* const vector)());


#endif /* INCLUDE_INIT_INIT_H_ */
