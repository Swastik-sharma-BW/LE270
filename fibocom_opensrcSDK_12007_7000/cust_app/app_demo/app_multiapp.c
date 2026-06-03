#include <stdio.h>
#include "fibo_opencpu_api.h"
#include "fibo_opencpu_comm.h"
#include "app.h"
#include "mem_map.h"

#ifdef    APP2_IMAGE_FLASH_LOAD_ADDR
#include "multi_app_common.h"
INT32 sys_thread_create(void *pvTaskCode, int8_t *pcName, uint32_t usStackDepth, void *pvParameters, uint32_t uxPriority)
{
    return fibo_thread_create(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority);
    //return 0;
}
void sys_thread_delete(void)
{
    fibo_thread_delete();
    return;
}
void *sys_malloc(uint32_t size)
{
     return fibo_malloc(size);
}
int32_t sys_free(void *buffer)
{
     fibo_free(buffer);
     return 0 ;
}
void sys_taskSleep(uint32_t msec)
{
     fibo_taskSleep(msec);
     return;
}
int32_t sys_test_201(void)
{
   // do nothing
   fibo_textTrace("APP2  hello world,sys_test_201");
   return 0;
}
	
/*
* @ mutil app test function sample
# @ fibocom
*/

static APP_SERRICES_T api_for_3rd_app = {
    sys_thread_create,
    sys_thread_delete,
    sys_malloc,
    sys_free,
    sys_taskSleep,
    sys_test_201,
};

static void mulitapp_demo()
{
    static int count = 0;
	
    fibo_textTrace("application thread enter");
    
       
    while (1)
    {
		APP_LOG_INFO("APP1 RUN %d", count++);
		fibo_taskSleep(1000);
		if(15 == count)
		{
            void *p = NULL;
			p = fibo_app_start(APP_IMAGE_FLASH_LOAD_SIZE,APP2_IMAGE_FLASH_LOAD_SIZE,APP_RAM_DATA_SIZE,APP2_RAM_DATA_SIZE, (void *)&api_for_3rd_app);
			APP_LOG_INFO("start APP2 return %p", p);			
		} 
    }
    
	return;
    
}

int app_multiapp_init(void)
{	
	APP_LOG_INFO("This multiapp APP2 init demo.");
    // 单独启任务跳转APP2，不要从APP1的入口直接启动APP2
	fibo_thread_create(mulitapp_demo, (INT8 *)"app_task", 16*1024, NULL, osPriorityNormal);
    return 0;
}
#else
int app_multiapp_init(void)
{	
	APP_LOG_INFO("Multiapp APP2 init NULL.");
    return 0;
}
#endif