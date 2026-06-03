/**
 * @file bot_system.h
 * @brief System abstraction layer interface, used to connect to different OS
 * @details This file is a unified abstraction layer for different RTOS and must be adapted
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */

#ifndef __BOT_SYSTEM_H__
#define __BOT_SYSTEM_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#include "bot_system_utils.h"
#include "bot_platform.h"
#include "bot_platform_user_config.h"

/** @def PLATFORM_WAIT_INFINITE
    @brief Define the wait forever timeout macro
*/
#define PLATFORM_WAIT_INFINITE (~0)

typedef void *bot_timer_t;
typedef void (*bot_timer_cb)(void *arg);

/** @struct bot_mem_para_t
    @brief head info
*/
typedef struct _bot_mem_para {
    void        *start; /* start addr of bot heap for bot sdk */
    unsigned int len;   /* length of bot heap  for bot sdk    */
} bot_mem_para_t;

/** @struct bot_task_prio_e
    @brief bot task priority
*/
typedef enum {
    BOT_TASK_PRIO_HIGN = 0,      ///< High priority
    BOT_TASK_PRIO_ABOVE_NORMAL,  ///< Medium high priority
    BOT_TASK_PRIO_NORMAL,        ///< Medium Priority
    BOT_TASK_PRIO_BLOW_NORMAL,   ///< Medium low priority
    BOT_TASK_PRIO_LOW,           ///< Low priority
} bot_task_prio_e;

/** @struct bot_task_param_t
    @brief Task creation parameters
*/
typedef struct _bot_task_param_t {
    char            *name;         ///< task name
    bot_task_prio_e  priority;     ///< task priority
    int              stack_size;   ///< task stack size in bytes
    void (*task_entry)(void *);    ///< task entry
    void *arg;                     ///< task arg
} bot_task_param_t;

/** @struct bot_timer_type_e
    @brief Timer creation parameters
*/
typedef enum {
    BOT_TIMER_ONCE = 0, /* one shot timer */
    BOT_TIMER_REPEAT    /* repeat timer */
} bot_timer_type_e;

/** @struct bot_timer_type_e
    @brief Timer config parameters
*/
typedef struct {
    bot_timer_cb cb;          ///< user timer callback when expired
    void        *arg;         ///< user timer arg
    uint32_t     ms;          ///< timer period, unit ms. Notably, ms must be no less than 20ms, and an integer multiple of 20ms
    bot_timer_type_e repeat;  ///< timer type of the timer, BOT_TIMER_ONCE or BOT_TIMER_REPEAT
} bot_timer_cfg_t;

/** @struct bot_task_param_t
    @brief System memory info
*/
typedef struct bot_heap_info {
    unsigned long heap_total; ///< total heap memory
    unsigned long heap_used;  ///< used heap memory
    unsigned long heap_free;  ///< free heap memory
} bot_heap_info_t;

/**
 * @brief Memory Alloc
 *
 * @param[in] size the length to alloc
 *
 * @return the address of the memory to alloc
 * @retval NULL fail
 * @retval others success
 */
void *bot_os_alloc(unsigned int size);

/**
 * @brief Memory Free
 *
 * @param[in] addr the address of the memory to free
 *
 */
void bot_os_free(void *addr);

/**
 * @brief Memory Realloc
 *
 * @param[in]  addr   the address of the memory to free
 * @param[in]  size   the length to alloc
 *
 * @return the address of the memory to realloc
 * @retval NULL fail
 * @retval others success
 */
void *bot_os_realloc(void *addr, unsigned int newsize);

/**
 * @brief Get current time in milliseconds.
 *
 * @return  type long long  elapsed time in milliseconds from system starting.
 */
unsigned long long bot_uptime(void);

/**
 * @brief Get current time.
 * @param[out] tv    get the number of seconds and microseconds
 * @param[out] dummy reserved
 * @retval 0 success
 * @retval -1 failure.
 */
int bot_gettimeofday(struct timeval *tv, void *dummy);

/**
 * @brief Task sleep
 *
 * @param[in] ms time for task to sleep in milliseconds.
 *
 * @return void
 */
void bot_msleep(unsigned int ms);

/**
 * @brief Create a semaphore.
 *
 * @param[in]  init_value  semaphore init value.
 *
 * @return    pointer of semaphore object
 * @retval    NULL fail
 * @retval    others success
 */
void *bot_sem_create(unsigned int init_value);

/**
 * @brief Delete a semaphore.
 *
 * @param[in]  sem  semaphore object.
 *
 * @return  delete result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_sem_delete(void *sem);

/**
 * @brief Release a semaphore.
 *
 * @param[in]  sem  semaphore object.
 *             It will wakeup one task which is waiting for the sem according to task priority.
 *
 * @return post result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_sem_post(void *sem);

/**
 * @brief Acquire a semaphore.
 *
 * @param[in]  sem         semaphore object.
 * @param[in]  timeout_ms  waiting until timeout in milliseconds.
 *                         value:  PLATFORM_WAIT_INFINITE: wait semaphore forever.
 *                                 0: return immediately if not get mutex.
 *
 * @return wait result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_sem_wait(void *sem, unsigned int timeout_ms);

/**
 * @brief Create a queue
 *
 * @param[in]  queue_length  the length of the buffer.
 * @param[in]  item_size     the max size for one msg.
 *
 * @return  pointer to the queue.
 * @retval    NULL fail
 * @retval    others success
 */
void *bot_queue_create(unsigned int queue_length, unsigned int item_size);

/**
 * @brief Delete a queue
 *
 * @param[in] queue pointer to the queue.
 *
 * @return delete result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_queue_delete(void *queue);

/**
 * @brief Receive msg from a queue.
 *
 * @param[in]   queue  pointer to the queue.
 * @param[out]  msg    buffer to store message.
 * @param[in]   size   size of the buffer to store message.
 * @param[in]   ms     ms to wait before receive.
 *
 * @return queue recv result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_queue_recv(void *mq, void *msg, unsigned int size, unsigned int ms);

/**
 * @brief Send a msg to the front of a queue.
 *
 * @param[in]  queue  pointer to the queue.
 * @param[in]  msg    buffer of the message to send.
 * @param[in]  size   size of the message to send.
 * @param[in]  ms     ms to wait before send.
 *
 * @return queue send result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_queue_send(void *queue, void *msg, unsigned int size, unsigned int ms);

/**
 * @brief Send a msg to the front of a queue in isr.
 *
 * @param[in]  queue  pointer to the queue.
 * @param[in]  msg    message to send.
 * @param[in]  size   size of the message to send.
 *
 * @return queue send result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_queue_send_isr(void *queue, void *msg, unsigned int size);


/**
 * @brief Create a task.
 *
 * @param[out]  task_handle          output task handle.
 * @param[in]   task_param           param of the task.
 *
 * @return task create result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_task_create(void **task_handle, bot_task_param_t *task_param);

/**
 * @brief Delete a task by name.
 *
 * @param[in]  task_handle  task handle.
 *
 * @return task delete result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_task_delete(void *task_handle);

/**
 * @brief create a mutex.
 *
 * @return    pointer of mutex object.
 * @retval    NULL fail
 * @retval    others success
 */
void *bot_mutex_create(void);

/**
 * @brief Free a mutex.
 *
 * @param[in]  mutex  mutex object.
 *
 * @return mutex delete result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_mutex_delete(void *mutex);

/**
 * @brief Lock a mutex.
 *
 * @param[in] mutex mutex object.
 *
 * @return void
 */
void bot_mutex_lock(void *mutex);

/**
 * @brief Unlock a mutex.
 *
 * @param[in] mutex mutex object.
 *
 * @return void
 */
void bot_mutex_unlock(void *mutex);

/**
 * @brief Create a timer.
 *
 * @param[out] timer      timer handle
 * @param[in]  timer_cfg  user timer configuration
 *
 * @return timer create result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_timer_create(bot_timer_t *timer, bot_timer_cfg_t *timer_cfg);

/**
 * @brief Start a timer.
 *
 * @param[in] timer timer object.
 *
 * @return timer start result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_timer_start(bot_timer_t timer);

/**
 * @brief Stop a timer.
 *
 * @param[in] timer timer object.
 *
 * @return timer stop result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_timer_stop(bot_timer_t timer);

/**
 * @brief Delete a timer.
 *
 * @param[in] timer timer object.
 *
 * @return timer delete result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_timer_delete(bot_timer_t timer);

/**
 * @brief Get mem info for MaaS sdk.
 *
 * @param[out] mem_info  describe the address and length for MaaS heap.
 *
 * @return get mem int result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_mem_info_get(bot_mem_para_t *mem_info);

/**
 * @brief Platform initialize, which is called in bot SDK init function.
 *
 * @return platform init result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_platform_init(void);

/**
 * @brief maas app restart api
 *
 * @return restart result
 * @retval 0 success
 * @retval <0 fail
*/
int bot_reboot(void);

/**
 * @brief System memory info api
 *
 * @param[out] heap_info system heap info
 *
 * @return get heap memory info result
 * @retval 0 success
 * @retval <0 fail
*/
int bot_heap_memory_info(bot_heap_info_t *heap_info);

#if defined(__cplusplus)
}
#endif

#endif /* __BOT_SYSTEM_H__ */

