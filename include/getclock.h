/*
 * getclock.h
 *
 *  Created on: 20 мая 2015 г.
 *      Author: vit
 */

#ifndef INCLUDE_GETCLOCK_H_
#define INCLUDE_GETCLOCK_H_

#ifdef __cplusplus
#include <ctime>
extern clock_t (*getclock)();
#else
#include <time.h>
extern clock_t (*getclock)(void);
#endif

#endif /* INCLUDE_GETCLOCK_H_ */
