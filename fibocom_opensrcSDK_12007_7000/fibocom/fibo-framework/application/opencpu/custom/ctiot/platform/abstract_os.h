/*******************************************************************************
 *
 *******************************************************************************/

#ifndef __ABSTRACT_OS_H_
#define __ABSTRACT_OS_H_

#define PLATFORM_FREERTOS 1

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************************
 * Platform abstraction functions to be implemented by the user
 *
 ***************************************************************************************/

    /**************************************************************************
                              CTIOT_ASSERT
***************************************************************************/

#if defined(PLATFORM_LINUX) || defined(PLATFORM_CYGWIN64_OR_MYSYS64)

    #define CTIOT_ASSERT_ENABLE

    #ifdef CTIOT_ASSERT_ENABLE

    #define CTIOT_ASSERT(x)                                                                  \
        do                                                                                   \
        {                                                                                    \
            if (!(x))                                                                        \
            {                                                                                \
                os_log("CTIOT ASSERT:[%s->%s.c:%d]\r\n", __FILE__, __func__, __LINE__); \
                while (1)                                                                    \
                {                                                                            \
                };                                                                           \
            }                                                                                \
        } while (0);
        
    #else
        
    #define CTIOT_ASSERT(x)

    #endif


#else

#endif

/********************************************************
*      log api
*
*********************************************************/

void os_log(const char *format, ...);

/********************************************************
*      malloc and trace api
*
*********************************************************/
#ifndef HEAP_TRACE

    // Allocate a block of size bytes of memory, returning a pointer to the beginning of the block.
    void *ctiot_malloc(size_t s);
    // Deallocate a block of memory previously allocated by lwm2m_malloc() or lwm2m_strdup()
    void ctiot_free(void *p);
    // Allocate a memory block, duplicate the string str in it and return a pointer to this new block.
    char *ctiot_strdup(const char *str);

#else
    #include "heap_trace.h"
    
    #define ctiot_malloc(size) malloc_trace(size, 0xdeadbeaf,__FILE__, __FUNCTION__, __LINE__);
    #define ctiot_free(pmem)   free_trace(pmem, 0xdeadbeaf,__FILE__, __FUNCTION__, __LINE__);
    #define ctiot_strdup(pmem) strdup_trace(pmem, 0xdeadbeaf,__FILE__, __FUNCTION__, __LINE__);

#endif

/********************************************************
*      time api
*
*********************************************************/

    #include <time.h>
    #if 0
     struct timeval
     {
         long tv_sec;  /* seconds */
         long tv_usec; /* and microseconds */
     };
#endif

    // This function must return the number of seconds elapsed since origin.
    // The origin (Epoch, system boot, etc...) does not matter as this
    // function is used only to determine the elapsed time since the last
    // call to it.
    // In case of error, this must return a negative value.
    // Per POSIX specifications, time_t is a signed integer.
    time_t ctiot_gettime(void);

    int ctiot_gettimeofday(void *tv, void *);

/********************************************************
*     rtos api
*
*********************************************************/

/// Attributes structure for thread.
typedef struct 
{
  const char    *name;      ///< name of the thread
  unsigned int  stack_size; ///< size of stack
  unsigned int  priority;   ///< initial thread priority (linux 0~99, CMSIS-RTOS:8~55, freeRTOS:0~64, )
} thread_attr_t;


    #include "cmsis_os2.h"
    
    #define WAIT_FOREVER     osWaitForever
    
    typedef osThreadId_t     thread_handle_t;
    typedef void             thread_ret;
    typedef void*            thread_params;
    typedef thread_ret (*start_routine)(thread_params);
    
    typedef osMutexId_t        thread_mutex_t;
    typedef osSemaphoreId_t    thread_sem_t;
    typedef osMessageQueueId_t thread_queue_t;
    
    typedef osTimerId_t        thread_stimer_t;
    typedef void (*stimer_callback)(void *param);
    
    //void sleep(int sec);
    //void usleep(int usec);
    


    typedef struct
    {
        long type;
        void *data;
    } thread_msg_t;

    enum msg_ret
    {
        MSG_OK = 0,
        MSG_FAIL
    };
    
    enum stimer_ret
    {
        SOFT_TIMER_ONCE     = 0,       ///< one-shot timer
        SOFT_TIMER_PERIOIDC = 1        ///< repeating timer
    };

    void msleep(int msec);
    
    int thread_create(thread_handle_t *thread, thread_attr_t *attr, start_routine entry, void *arg);
    int thread_exit(void *retval);
    int thread_cancel(thread_handle_t thread_id);
    int thread_join(thread_handle_t thread_id, void **retval);

    int thread_mutex_init(thread_mutex_t *mutex, const char *name);
    int thread_mutex_lock(thread_mutex_t *mutex, long int msecs);
    int thread_mutex_unlock(thread_mutex_t *mutex);
    int thread_mutex_destory(thread_mutex_t *mutex);

    int thread_sem_init(thread_sem_t *sem, int value);
    int thread_sem_wait(thread_sem_t *sem, long int msecs);
    int thread_sem_post(thread_sem_t *sem);
    int thread_sem_destroy(thread_sem_t *sem);

    int thread_msg_queue_init(thread_queue_t *msg_handle, size_t msg_count, size_t item_size);
    int thread_msg_queue_put(thread_queue_t *msg_handle, const thread_msg_t *msg, int timeout);
    int thread_msg_queue_get(thread_queue_t *msg_handle, thread_msg_t *msg, int timeout);

    int thread_soft_timer_init(thread_stimer_t *handle, stimer_callback callback, int type);
    int thread_soft_timer_start(thread_stimer_t handle, int millisec);
    int thread_soft_timer_stop(thread_stimer_t handle);
    int thread_soft_timer_destroy(thread_stimer_t handle);

#ifdef __cplusplus
}
#endif

#endif //PLATFORM_H_
