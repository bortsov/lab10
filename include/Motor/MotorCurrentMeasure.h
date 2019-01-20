/*
 * MotorCurrentMeasure.h
 *
 *  Created on: 26 дек. 2017 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_MOTORCURRENTMEASURE_H_
#define INCLUDE_MOTORCURRENTMEASURE_H_

namespace motorCurrent {

enum WINDING_CURRENT {
    EXT_IA,
    EXT_IC
};


void create(const float gainMotorCurrentExt);
void destroy();
void setCallbackOnConversationDoneIrq(
        void (*handler)(
                const float ia,
                const float ic));
float getCurrent(const WINDING_CURRENT nWinding);
void setOffset(const WINDING_CURRENT nWindings, const float offset);

} /* namespace motorCurrent */



#endif /* INCLUDE_MOTORCURRENTMEASURE_H_ */
