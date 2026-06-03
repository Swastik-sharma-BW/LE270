/*
 * Copyright (c) 2018 Qianxun SI Inc. All rights reserved.
 *
 * File name   : qxwz_rtos_al.h
 *
 * Vesion      :
 *
 * Date        : 2018.11.25
 *
 * Author      :
 *
 * Description :rtos al head file
 *
 * History     : Created in Nov.25 2018
 *
 */

#ifndef __QXWZ_OS_AL_H__
#define __QXWZ_OS_AL_H__

#include <stdbool.h>
#include <stdint.h>

#define __FIBOL610__
#define FIBO_OPEN

#ifdef __cplusplus
extern "C"
{
#endif

    /******************************MACRO DEF******************************/

    typedef enum
    {
        QXWZ_THREAD_PRIORITY_IDLE = -3,
        QXWZ_THREAD_PRIORITY_LOW = -2,
        QXWZ_THREAD_PRIORITY_BELOW_NORMAL = -1,
        QXWZ_THREAD_PRIORITY_NORMAL = 0,
        QXWZ_THREAD_PRIORITY_ABOVE_NORMAL = 1,
        QXWZ_THREAD_PRIORITY_HIGH = 2,
        QXWZ_THREAD_PRIORITY_REALTIME = 3, ///< priority: realtime (highest)
        QXWZ_THREAD_PRIORITY_ERROR = 0x84  ///< system cannot determine priority or thread has illegal priority
    } qxwz_thread_priority_e;

    typedef enum
    {
        QXWZ_TIMER_ONCE = 0,
        QXWZ_TIMER_PERIODIC = 1
    } QxwzTimerType;

    /* boolean representation */
    typedef enum
    {
        /* FALSE value */
        QXWZ_FALSE,
        /* TRUE value */
        QXWZ_TRUE
    } qxwz_bool_t;

    /********************************END**********************************/

    /******************************ENUM DEF*******************************/

    /********************************END**********************************/

    /*****************************STRUCT DEF******************************/

    typedef uint64_t qxwz_time_t;
    typedef void (*qxwz_timer_callback_t)(void *);

    /********************************END**********************************/

#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

    // #define FP_ZERO 0
#define QX_TASK_PRIORITY_BASE 0
#define QXWZ_NTRIP_RECV_CONN_TIMEOUT 90000

    typedef struct
    {
        unsigned char *buf;
        unsigned int in;
        unsigned int out;
        unsigned int size;
    } fifo_t;

    typedef void *qxwz_thread_t;
    typedef void *qxwz_queue_t;

    typedef void *qxwz_sem_t;
    typedef void *qxwz_mutex_t;

    typedef unsigned int qxwz_timer_t;

    typedef struct tm1
    {
        int tm_sec;   /* �룬��Χ�� 0 �� 59                */
        int tm_min;   /* �֣���Χ�� 0 �� 59                */
        int tm_hour;  /* Сʱ����Χ�� 0 �� 23                */
        int tm_mday;  /* һ���еĵڼ��죬��Χ�� 1 �� 31                    */
        int tm_mon;   /* �·ݣ���Χ�� 0 �� 11                */
        int tm_year;  /* ����                */
        int tm_wday;  /* һ���еĵڼ��죬��Χ�� 0 �� 6                */
        int tm_yday;  /* һ���еĵڼ��죬��Χ�� 0 �� 365                    */
        int tm_isdst; /* ����ʱ or ����ʱ                       */
    } tm;

    /*
 * @brief : Get os's (unsigned long long)ms
 * @param[in]: none
 *
 * @return: the os's milliseconds since booting
 */
extern qxwz_time_t qxwz_get_boot_ms(void);

    /*
 * convert time
 * @param[in]: timestamp:time,uint is ms
               hsm: ptr to msecond
               ysm: ptr to day
 *
 * @return: tm ptr
 */
    tm *qxwz_gmtime(unsigned long long timestamp, double *hsm, int *ymd);

    /*
 * @brief: sleep seconds
 * @param[in]  sec: seconds.
 *
 * @return: none
 */
    extern void qxwz_sleep(unsigned int sec);

    /*
 * @brief: sleep milliseconds
 * @param[in]  msec: milliseconds.
 *
 * @return: none
 */
    extern void qxwz_msleep(unsigned int msec);

    /*
 * allocate memory from heap
 * @param[in]  size: the size of the memory.
 *
 * @return: a pointer to the allocated memory.
 */
    extern void *qxwz_malloc(unsigned int size);

    /*
 * free memory
 * @param[in]  addr: a pointer to the freeed memory..
 *
 * @return: none.
 */
    extern void qxwz_free(void *addr);

    /*
 * create queue
 * @param[out] queue: handle returned by qxwz_queue_create
 * @param[in]  nmemb: nmemb items of data.
 * @param[in]  size: size of item.
 *
 * @return: 
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_queue_create(qxwz_queue_t *queue, unsigned int nmemb, unsigned int size);

    /*
 * delete queue
 * @param[in]  queue: handle of queue .
 *
 * @return: none.
 */
    extern void qxwz_queue_delete(qxwz_queue_t queue);

    /*
 * send queue
 * @param[in]  queue: handle of queue.
 * @param[in]  item: a pointer to item.
 * @param[in]  size: size of item.
 * @param[in]  tm: timeout milliseconds.
 *
 * @return:
 *  0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_queue_send(qxwz_queue_t queue, const void *item, int size, unsigned int tm);

    /*
 * read queue
 * @param[in]  queue: handle of queue.
 * @param[in]  item: a pointer to item.
 * @param[in]  size: size of item.
 * @param[in]  tm: timeout milliseconds.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_queue_recv(qxwz_queue_t queue, const void *item, int size, unsigned int tm);

    /*
 * create a mutex
 * @param[in/out] mutex: handle returned by qxwz_mutex_create
 * @param[in]  name: mutex's name
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_mutex_create(qxwz_mutex_t *mutex, const char *name);

    /*
 * @brief: delete a mutex
 * @param[in] mutex: the mutex need to be deleted.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_mutex_delete(qxwz_mutex_t *mutex);

    /*
 * @brief: take a mutex
 * @param[in] mutex: the mutex need to be take.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_mutex_lock(qxwz_mutex_t *mutex);

    /*
 * @brief: release a mutex
 * @param[in] mutex: the mutex need to be release.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_mutex_unlock(qxwz_mutex_t *mutex);

    /*
 * create a semaphore
 * @param[in/out] mutex: handle returned by qxwz_sem_create
 * @param[in]  name:  semaphore's name
 * @param[in]  value: initial value for the semaphore
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_sem_create(qxwz_sem_t *sem, const char *name, int value);

    /*
 * @brief: delete a semphone
 * @param[in] sem: the semphone need to be deleted.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_sem_delete(qxwz_sem_t *sem);

    /*
 * @brief: take a semphone
 * @param[in] sem: the semphone need to be taken.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_sem_wait(qxwz_sem_t *sem);

    /*
 * @brief: take a semphone
 * @param[in] sem: the semphone need to be taken.
 * @param[in] millisec: relative timeout.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason, in case of timeout 
 */
    extern int qxwz_sem_timedwait(qxwz_sem_t *sem, unsigned int millisec);

    /*
 * @brief: release a semphone
 * @param[in] sem: the semphone need to be release.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_sem_post(qxwz_sem_t *sem);

    enum
    {
        QXWZ_THREAD_CLEANUP_PUSH,
        QXWZ_THREAD_CLEANUP_POP,
    };

    typedef struct
    {
        char *name;
        int priority;
        /*stack size requirements in bytes; 0 is default stack size*/
        unsigned int stacksize;
        /*True:Detached, False:Joinable*/
        bool detached;
    } qxwz_thread_attr_t;

#define qxwz_thread_setattr(name, priority, stacksz, detach) const qxwz_thread_attr_t qxwz_thread_attr_##name = {#name, (priority), (stacksz), (detach)}
#define qxwz_thread_getattr(name) &qxwz_thread_attr_##name

#if defined(QXWZ_USE_PTHREAD_CLEANUP)
/*
 * Install a cleanup handler: ROUTINE will be called with arguments ARG
   when the thread is canceled or thread exit.  ROUTINE will also
   be called with arguments ARG when the matching qxwz_thread_cleanup_pop
   is executed with non-zero EXECUTE argument.

   qxwz_thread_cleanup_push and qxwz_thread_cleanup_pop are macros and must always
   be used in matching pairs at the same nesting level of braces. 
*/
#define qxwz_thread_cleanup_push(routine, arg) pthread_cleanup_push(routine, arg)

/*
 * Remove a cleanup handler installed by the matching qxwz_thread_cleanup_push.
   If EXECUTE is non-zero, the handler function is called.
 */
#define qxwz_thread_cleanup_pop() pthread_cleanup_pop(0)

#else
    typedef struct qxwz_thread_cleanup_t qxwz_thread_cleanup_t;

    struct qxwz_thread_cleanup_t
    {
        qxwz_thread_cleanup_t *next;
        void (*proc)(void *);
        void *data;
    };

#define qxwz_thread_cleanup_push(routine, arg)                                                                                                                                                         \
    do                                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        qxwz_thread_cleanup_t cleanup_data = {                                                                                                                                                         \
            NULL,                                                                                                                                                                                      \
            routine,                                                                                                                                                                                   \
            arg,                                                                                                                                                                                       \
        };                                                                                                                                                                                             \
    qxwz_thread_control_cancel(QXWZ_THREAD_CLEANUP_PUSH, &cleanup_data)

#define qxwz_thread_cleanup_pop()                                                                                                                                                                      \
    qxwz_thread_control_cancel(QXWZ_THREAD_CLEANUP_POP);                                                                                                                                               \
    }                                                                                                                                                                                                  \
    while (0)

#endif /* End of QXWZ_USE_PTHREAD_CLEANUP */
    /*
 * @brief: create a thread
 * @param[in] th: thread handle.
 * @param[in] attr: a struct related to thread.
 * @param[in] start_routine: start_routine of created thread.
 * @param[in] arg: argument of thread function.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_thread_create(qxwz_thread_t *th, const qxwz_thread_attr_t *attr, void *(*start_routine)(void *), void *arg);

    /*
 * @brief: start a thread
 * @param[in] id: thread handle.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_thread_start(qxwz_thread_t id);

    /*
  * @brief: stop a thread
  * @param[in] thread: thread handle.
  *
  * @return:
  *  =0 if success
  *  -1 if fail for any reason
  */

    extern int qxwz_thread_cancel(qxwz_thread_t thread);

    /*
 * @brief: delete a thread
 * @param[in] thread: thread handle.
 * @param[in] retval: thread parameter.
 *
 * @return:
 *	=0 if success
 *	-1 if fail for any reason
 */

    extern int qxwz_thread_join(qxwz_thread_t thread, void **retval);

    /*
 * @brief: Issues an explicit deferred cancellation point.
 *
 * This has no effects if thread cancellation is disabled.
 * This is also used to force a cancellation point in a function that would exit
 * @param:  none
 * @return: none
 */
    extern void qxwz_thread_testcancel(void);

    /*
 * @brief: Enable cancellation.
 * This function will enable cancellation for the calling thread
 * @param:  none
 * @return: none
 */
    extern void qxwz_thread_enablecancel(void);

    /*
 * @brief: Disable cancellation.
 * This function will disabel cancellation for the calling thread which can't be cancelled
 * It must be called before entering a piece of code that is not cancellation-safe.
 * @param:  none
 * @return: none
 */
    extern void qxwz_thread_disablecancel(void);

    /**
 *  @brief: Get Thread handle.
 *
 * @param:  none
 *
 * @return:
 * the thread name of the calling thread.
 *
 */
    //extern qxwz_thread_t qxwz_thread_self(void);
    extern const char *qxwz_get_thread_name(void);

    /**
 * @brief: Internal handler for thread cancellation
 * 
 * Do not call this function directly. Use wrapper macros instead:
 * qxwz_thread_cleanup_push(), qxwz_thread_cleanup_pop().
 */
    extern void qxwz_thread_control_cancel(int cmd, ...);

    /*
 * @brief: create a timer
 * @param[in] timer_id: timer handle
 * @param[in] name: name of timer.
 * @param[in] callback: the callback of timer.
 * @param[in] type:  timer type, once or periodic.
 * @param[in] data: the argument of timer function.
 *
 * @return: 
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_timer_create(qxwz_timer_t *timer_id, const char *name, qxwz_timer_callback_t callback, QxwzTimerType type, void *data);

    /*
 * @brief: delete a timer
 * @param[in] timer: a handle for timer.
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_timer_delete(qxwz_timer_t timer_id);

    /*
 * @brief: arms or disarms an initialized timer.
 * @param[in] timer_id: timer handle
 * @param[in] value: zero to disarm the timer, otherwise the initial time to wait before firing the timer
 * @param[in] interval: zero to fire the timer just once, otherwise the timer repetition interval.
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_timer_schedule(qxwz_timer_t timer_id, qxwz_time_t value, qxwz_time_t interval);

    /*
 * @brief: printf function
 * @param[in] data: the pointer to buffer
 * @param[in] len: the length of buffer
 *
 * @return:
 *  =0 if success
 *  -1 if fail for any reason
 */
    extern int qxwz_trace(unsigned char *data, int len);

#ifdef __cplusplus
}
#endif

#endif
