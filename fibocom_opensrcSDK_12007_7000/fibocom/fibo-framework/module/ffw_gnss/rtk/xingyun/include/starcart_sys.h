#ifndef _STARCART_SYS_H_
#define _STARCART_SYS_H_
#ifdef __cplusplus
extern "C"
{
#endif
/*平台相关数据类型定义*/
typedef char starcart_int8_t;
typedef unsigned char starcart_uint8_t;
typedef short starcart_int16_t;
typedef unsigned short starcart_uint16_t;
typedef int starcart_int32_t;
typedef unsigned int starcart_uint32_t;
typedef unsigned long long starcart_uint64_t;
/*平台相关互斥锁和线程定义*/
typedef void *starcart_mutex_t;
typedef void *starcart_thread_t;

extern int starcart_thread_create(void *thread_fuction,const char *thread_name,starcart_thread_t * pthread_t);
extern int starcart_thread_delete(starcart_thread_t * pthread_t);
extern int starcart_mutex_create(starcart_mutex_t *pmutex, const char *mutex_name);
extern int starcart_mutex_lock(starcart_mutex_t *pmutex);
extern int starcart_mutex_unlock(starcart_mutex_t *pmutex);
extern int starcart_mutex_destroy(starcart_mutex_t *pmutex);
extern void starcart_sleep_ms(starcart_uint32_t ms);
extern void*starcart_malloc(starcart_uint32_t size);
extern void starcart_free(void* ptr);
#ifdef __cplusplus
}
#endif
#endif
