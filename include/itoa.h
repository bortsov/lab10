/*
 * itoa.h
 *
 *  Created on: 29 февр. 2016 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_ITOA_H_
#define INCLUDE_ITOA_H_

#include <cstdint>
#include <cstddef>

/*
 * преобразование числа в строку; окончательный '\0' не добавляется
 * return: число символов
 */
size_t int32_tToA   (char* buf,     const int32_t i);
size_t int32_tToA   (uint8_t* buf,  const int32_t i);

size_t float3pToA   (char* buf,     const float v);
size_t float3pToA   (uint8_t* buf,  const float v);

size_t uint32_tToA  (char* buf,     const uint32_t i);
size_t uint32_tToA  (uint8_t* buf,  const uint32_t i);

size_t float6pToA   (char* buf,    float v);



#endif /* INCLUDE_ITOA_H_ */
