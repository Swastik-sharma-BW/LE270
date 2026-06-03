#ifndef __OC_LPM_H__
#define __OC_LPM_H__

#include "fibo_opencpu_comm.h"

/**
 * @brief
 *
 * @function: fibo_setSleepMode(uint8_t time)
 * @param time: auto sleep_time
 * @retval FFW_R_SUCCESS
 */
int32_t fibo_setSleepMode(uint8_t time);

int32 fibo_pm_wake_lock(const char *lock_name);

int32 fibo_pm_wake_unlock(const char *lock_name);

#ifdef CONFIG_CUST_XIAONIU_FEATURE
bool fibo_TY_wakeup_init(UINT8 pin,UINT8 level);

bool fibo_TY_wakeup_enable(void);
#endif
#endif