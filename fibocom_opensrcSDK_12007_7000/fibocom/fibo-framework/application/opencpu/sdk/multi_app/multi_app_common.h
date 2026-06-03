#ifndef __MULTI_APP_COMMON_H__
#define __MULTI_APP_COMMON_H__
#include "mem_map.h"


// APP服务接口结构
typedef struct app_services {
    INT32 (*sys_thread_create)(void *pvTaskCode, int8_t *pcName, uint32_t usStackDepth, void *pvParameters, uint32_t uxPriority);
    void (*sys_thread_delete)(void);
    void *(*sys_malloc)(uint32_t size);
    int32_t (*sys_free)(void *buffer);
    void (*sys_taskSleep)(uint32_t msec);
    int32_t (*sys_test_201)(void);
} APP_SERRICES_T;

typedef enum {
    THIRD_APP_SIG_TEST1 = 0,
    THIRD_APP_SIG_MAX
} THIRD_APP_SIGNAL_ID_T;

typedef void (*THIRD_APP_SIGNAL_CALLBACK)(THIRD_APP_SIGNAL_ID_T sig, va_list arg);
typedef struct {
    THIRD_APP_SIGNAL_CALLBACK fibo_signal;
} THIRD_APP_RECV_SIG_CALLBACK_T;

typedef void (*AT_RESP_CALLBACK)(uint8_t *buf, uint16_t len);


// APP服务接口结构
void * app_entry(void *param);

#endif