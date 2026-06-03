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


static void fibo_pwm_demo()
{
    APP_LOG_INFO("fibo_pwm_thread enter");

    for (int n = 0; n < 10; n++)
    {
        fibo_taskSleep(500);
    }

    int32_t index = 4;
    uint16_t duty = 200;
    uint16_t arr = 999;
    uint16_t psc = 0;

    uint16_t pin = 6;
    UINT8 mode = 5;

    fibo_gpio_mode_set(pin, mode);

    //Before configuring PWM output, ensure that the corresponding PIN uses the PWM function
    fibo_pwm_open(index);
    fibo_pwm_config(index, duty, arr, psc);

    
}


void *appimg_enter(void *param)
{
    APP_LOG_INFO("app image enter");
    fibo_thread_create(fibo_pwm_demo, (INT8 *)"fibo_pwm_thread", 10*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
    return;
}
