/*
 * SciLog.h
 *
 *  Created on: 21 янв. 2018 г.
 *      Author: Виталий
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <cstdint>
#include <cstddef>

namespace logSci {


void create(void);

/* выдача 5 целых чисел в полном диапазоне */
void log5(
        const int32_t v1,
        const int32_t v2,
        const int32_t v3,
        const int32_t v4,
        const int32_t v5);

/* выдача 3 целых чисел в полном диапазоне */
void log8(
        const int32_t v1,
        const int32_t v2,
        const int32_t v3);

/* выдача 2 целых чисел в полном диапазоне */
void log9(
        const int32_t v1,
        const int32_t v2);

/* выдача беззнакового в формате X.XXX и 3 целых чисел в полном диапазоне */
void log10(
        const float v1,
        const int32_t v2,
        const int32_t v3,
        const int32_t v4);

/* выдача беззнакового в формате X.XXX и 1 целого числа в полном диапазоне */
void log11(
        const float v1,
        const int32_t v2);


/* выдача 4 чисел с 3 знаками после запятой */
void log12(
        const float v1,
        const float v2,
        const float v3,
        const float v4);

} /* namespace logSci */

#endif /* INCLUDE_H_ */
