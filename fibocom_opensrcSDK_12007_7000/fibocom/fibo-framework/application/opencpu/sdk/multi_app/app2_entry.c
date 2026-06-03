#include <stdio.h>
#include "fibo_opencpu_comm.h"
//#include "fibo_opencpu_api.h"
#include "multi_app_common.h"


static APP_SERRICES_T * g_service = NULL;


/*
static void sig_recv_callback(THIRD_APP_SIGNAL_ID_T sig, va_list arg)
{
    switch (sig)
    {
        case THIRD_APP_SIG_TEST1:
        {
            //fibo_printf("[APP2] Received GAPP_SIG_TEST1 signal");
            // 处理测试信号
            //fibo_printf("[APP2] Test signal processing completed");
        }
        break;
        default:
            //fibo_printf("[APP2] Received unknown signal: %d", sig);
        break;
    }
}


static THIRD_APP_RECV_SIG_CALLBACK_T third_app_recv_sig_callback = {
    .fibo_signal = sig_recv_callback

};*/


void app2_demo(void)
{
	while(1)
	{
		g_service->sys_taskSleep(500);
		g_service->sys_test_201();
	}	
	return ;
}
void * app_entry(void *param)
//void * app2_entry(void *param)
{

    g_service = (APP_SERRICES_T *)param;

	app2_demo();

    return (void *) g_service; //&third_app_recv_sig_callback;
}


