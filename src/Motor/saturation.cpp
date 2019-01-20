/*
 * saturation.cpp
 *
 *  Created on: 24 февр. 2018 г.
 *      Author: Виталий
 */

#include "Motor/saturation.h"
#include <cmath>


/*
 * Самописная версия ограничителя
 */
float getSaturatedValue(
        const float inValue,
        const float maxValue,
        const float minValue)
{
    if (inValue < minValue) {
        return minValue;
    } else if (inValue > maxValue) {
        return maxValue;
    }
    return inValue;
}


