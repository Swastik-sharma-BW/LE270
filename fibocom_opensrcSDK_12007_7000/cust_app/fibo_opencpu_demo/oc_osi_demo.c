#include "app_image.h"
#include "fibo_opencpu.h"

#if 1

static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    fibo_textTrace("sig_res_callback  sig = %d (200 is opencpu test.)", sig);
}


static void at_res_callback(UINT8 *buf, UINT16 len)
{
    fibo_textTrace("FIBO <--%s", buf);
}

#endif

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};

static int g_hardware_index = 0;

void *g_hardware_timer = NULL;

void timer_function(void *arg)
{
    g_hardware_index++;
    if (g_hardware_index < 10)
        fibo_timer_start(g_hardware_timer, 100*1000, false);
}

/*****************************************************************************
* hardware_timer
*   Note: Cannot perform too many operations in callback functions,
        such as printing logs , deleting timers
*
*****************************************************************************/
void test_hardware_timer()
{
    g_hardware_timer = fibo_timer_create(timer_function, NULL, true);
    fibo_textTrace("test_timer_creat ok...");
    fibo_timer_start(g_hardware_timer, 300*1000, false);
}

static void hello_world_demo()
{
    fibo_textTrace("application thread enter");
    fibo_textTrace("test_timer_creat start...");
    for (int n = 0; n < 10; n++)
    {
        fibo_taskSleep(500);
    }
    test_hardware_timer();
    for (int n = 0; n < 10; n++)
    {
        fibo_textTrace("app image hello world %d, g_hardware_index %d", n, g_hardware_index);
        fibo_taskSleep(500);
    }
    
}


void *appimg_enter(void *param)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_enter_1, P_INFO, "application appimg_enter");
    fibo_textTrace("app image enter");
    fibo_thread_create(hello_world_demo, (INT8 *)"mythread", 10*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_exit_1, P_INFO, "application image exit");
    return;
}
