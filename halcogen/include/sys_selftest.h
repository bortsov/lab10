
#ifndef __sys_selftest_H__
#define __sys_selftest_H__

#include <stdint.h>

constexpr uint32_t PBISTSELFCHECK_FAIL1 = 5;
constexpr uint32_t PBISTSELFCHECK_FAIL2 = 41;
constexpr uint32_t PBISTSELFCHECK_FAIL3 = 42;

void disableParity(void);

void custom_dabort(void);
void selftestFailNotification(uint32_t flag);
void errata_PBIST_4(void);


#endif
