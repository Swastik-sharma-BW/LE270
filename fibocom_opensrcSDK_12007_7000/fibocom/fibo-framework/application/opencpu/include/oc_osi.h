#include <stdbool.h>
#include <stdint.h>
#include "fibo_opencpu_comm.h"

#ifdef CONFIG_CUST_NBSANXING_FEATURE
UINT32 fibo_timer_create(void (*fn)(void *arg), void *arg);
bool fibo_timer_start(UINT32 timer_id, UINT32 ms);
INT32 fibo_timer_delete(UINT32 timer);
bool fibo_timer_stop(UINT32 timerid);
#else
void* fibo_timer_create(void (*fn)(void *arg), void *arg,bool isIRQ);
bool fibo_timer_start(void* timer, UINT32 time,bool isPeriod);
INT32 fibo_timer_delete(void * timer);
INT32 fibo_timer_stop(void * timer);
#endif

#if defined(CONFIG_FFW_CUSTOM_TIMER_SUPPORT)
int fibo_timer_is_running(void* timer);
INT32 fibo_timer_remaining(void* timer);
#endif
INT32 fibo_heapinfo_get(uint32_t * size,uint32_t *avail_size,uint32_t *max_block_size);
UINT32 fibo_queue_create(UINT32 msg_count, UINT32 msg_size);
INT32 fibo_queue_put(UINT32 msg_id, const void *msg, UINT32 timeout);
INT32 fibo_queue_put_to_front(UINT32 msg_id, const void *msg, UINT32 timeout);
INT32 fibo_queue_get(UINT32 msg_id, void *msg, UINT32 timeout);
int32 fibo_queue_peek(uint32 msg_id, void* msg);
INT32 fibo_queue_put_isr(UINT32 msg_id, const void *msg);
INT32 fibo_queue_put_to_front_isr(UINT32 msg_id, const void *msg);
INT32 fibo_queue_get_isr(UINT32 msg_id, void *msg);
void fibo_queue_delete(UINT32 msg_id);
UINT32 fibo_queue_space_available(UINT32 msg_id);
void fibo_queue_reset(UINT32 msg_id);
UINT32 fibo_mutex_create(void);
void fibo_mutex_lock(UINT32 mutex_id);
INT32 fibo_mutex_try_lock(UINT32 mutex_id, UINT32 timeout);
void fibo_mutex_unlock(UINT32 mutex_id);
void fibo_mutex_delete(UINT32 mutex_id);
void fibo_taskSleep(UINT32 msec);
INT32 fibo_thread_create(void *pvTaskCode, INT8 *pcName, UINT32 usStackDepth, void *pvParameters, UINT32 uxPriority);
INT32 fibo_thread_create_ex(void *pvTaskCode, char *pcName, UINT32 usStackDepth, void *pvParameters, UINT32 uxPriority, UINT32* pThreadId);
void fibo_thread_delete(void);
void fibo_thread_suspend(uint32 uThread);
void fibo_thread_resume(uint32 uThread);
UINT64 fibo_get_sys_tick(void);
void *fibo_malloc(UINT32 size);
INT32 fibo_free(void *buffer);
INT32 fibo_soft_reset(void);
INT32 fibo_softReset(void);
INT32 fibo_soft_power_off(void);
INT32 fibo_softPowerOff(void);
UINT32 fibo_sem_new(UINT8 inivalue);
uint32 fibo_sem_new_v1(uint32 inivalue, uint32 max_count);
int32 fibo_sem_get_count(uint32 semid, uint32 *count_ptr);
void fibo_sem_free(UINT32 semid);
void fibo_sem_wait(UINT32 semid);
bool fibo_sem_try_wait(UINT32 semid, UINT32 timeout);
void fibo_sem_signal(UINT32 semid);
extern INT32 fibo_rng_generate(void *buf, UINT32 len);
UINT32 fibo_timer_new(UINT32 ms, void (*fn)(void *arg), void *arg);
INT32 fibo_timer_free(UINT32 timerid);
UINT32 fibo_timer_period_new(UINT32 ms, void (*fn)(void *arg), void *arg);
UINT32 fibo_get_thread_stack_remaining_size(UINT32 ThreadId);
UINT32 fibo_thread_id(void);
void fibo_specify_thread_delete(UINT32 uThread);
void delay_us(uint32_t us);
void vTaskSuspendAll(void);
long xTaskResumeAll(void);
UINT32 fibo_enterCS(void);
void fibo_exitCS(UINT32 status);
INT32 fibo_watchdog_enable(UINT32 sec);
INT32 fibo_watchdog_disable(void);
INT32 fibo_watchdog_feed(void);
void fibo_set_security_flag(void);
bool fibo_get_security_flag(void);