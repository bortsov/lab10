/*
 * internalAdc1.h
 *
 *  Created on: 18 окт. 2014 г.
 *      Author: bortsov
 */

#ifndef ADCI_H_
#define ADCI_H_

#include <cstddef>
#include <cstdint>

namespace physv {class PhysicalValue;}

namespace mcal {
namespace internalAdc1 {

void create(void *baseAddress, void *ram);
void destroy();
void registerConvertIrqHandler(physv::PhysicalValue* handler, size_t nch);
uint32_t& getCode(size_t nChannel);
float& getVoltage(size_t nChannel);
void process();

} /* namespace internalAdc1 */
} /* namespace mcal */




#endif /* ADCI_H_ */
