#include "app_image.h"
#include "fibo_opencpu.h"
#include "app.h"
#include "app_pdp.h"

/*****************************************************************************
*
* Note:
*   App receives fibocom events callback
*
*****************************************************************************/
static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    APP_LOG_INFO("sig_res_callback  sig = %d (200 is opencpu test.)", sig);
}

/*****************************************************************************
*
* Note:
*   fibo_send_at callback
*
*****************************************************************************/
static void at_res_callback(UINT8 *buf, UINT16 len)
{
    APP_LOG_INFO("FIBO <--%s", buf);
}

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};

/*****************************************************************************
*
* Note:
*   Support auto mode or gpio mode to control sleep wake-up
*       auto mode: UART has no data interaction within auto_sleep_time. Will enter sleep
*       gpio mode:
*           LE270-CN-1D3W-00: PIN5 and PIN19 support for controlling sleep
*
*****************************************************************************/
static void fibo_lpm_demo()
{
    APP_LOG_INFO("fibo_lpm_thread enter");

    for (int n = 0; n < 10; n++)
    {
        fibo_taskSleep(500);
    }

    /*
     * Using auto mode to control sleep wake-up.
     *
     */
    fibo_setSleepMode(3);
}


void *appimg_enter(void *param)
{
    APP_LOG_INFO("app image enter");
    fibo_thread_create(fibo_lpm_demo, (INT8 *)"fibo_lpm_thread", 10*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
    return;
}
