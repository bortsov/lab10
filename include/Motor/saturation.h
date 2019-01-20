/*
 * saturation.h
 *
 *  Created on: 24 февр. 2018 г.
 *      Author: Виталий
 */

#ifndef SRC_MOTOR_MODULES_SATURATION_SATURATION_H_
#define SRC_MOTOR_MODULES_SATURATION_SATURATION_H_

float getSaturatedValue(
		const float inValue,
		const float maxValue,
		const float minValue);

#endif /* SRC_MOTOR_MODULES_SATURATION_SATURATION_H_ */
