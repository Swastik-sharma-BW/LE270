/* Copyright (C) 2022 FIBOCOM Technologies Limited and/or its affiliates("FIBOCOM").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * FIBOCOM assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. FIBOCOM reserves the right to make changes in the
 * software without notification.  FIBOCOM also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
#include "app_image.h"
#include "fibo_opencpu.h"
#include "app.h"
#include "app_pdp.h"

static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    APP_LOG_INFO("sig_res_callback  sig = %d (200 is opencpu test.)", sig);
}
static void at_res_callback(UINT8 *buf, UINT16 len)
{
    APP_LOG_INFO("FIBO <--%s", buf);
}
static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};

static int normal_low_level = 0;
static int normal_high_level = 0;
static int wakeup_low_level = 0;
static int wakeup_high_level = 0;

void oc_gpionormalisrcallback(void *level)
{
    int *temp_level=(int *)level;

    if (*temp_level == 1)
        normal_high_level++;
    else if(*temp_level == 0)
        normal_low_level++;
}
void oc_gpiowakeupisrcallback(void *level)
{
    int *temp_level=(int *)level;

    if (*temp_level == 1)
        wakeup_high_level++;
    else if(*temp_level == 0)
        wakeup_low_level++;
}

static void gpio_switching_level_thread_entry(void *param)
{
    int ret = -1;

    //LE270
    ret = fibo_gpio_mode_set(23,0);
    APP_LOG_INFO("fibo_gpio_mode_set ret = %d", ret); //set pin 23 to gpio mode

    ret = fibo_gpio_cfg(23, 1); //set pin 23 dir out
    APP_LOG_INFO("fibo_gpio_cfg ret = %d", ret);

    while(1)
    {
        fibo_gpio_set(23, 1);
        fibo_taskSleep(2000);
        fibo_gpio_set(23, 0);
        fibo_taskSleep(2000);
    }
}

static void gpio_normal_isr_thread_entry(void *param)
{
/*------------test gpio interrupt--------------*/
    oc_isr_t demo_isr;
    APP_LOG_INFO("application isr normal thread enter, param 0x%x", param);
    set_app_ver(app_ver);
    int ret = -1;

    memset(&demo_isr,0,sizeof(demo_isr));

    demo_isr.debounce = true;
    demo_isr.intr_enable = true;
    demo_isr.intr_level = false;
    demo_isr.falling = true;
    demo_isr.rising = false;
    demo_isr.pullen = true;
    demo_isr.pullup = true;
    demo_isr.is_wakeup = false;
    demo_isr.cb = oc_gpionormalisrcallback;

    ret = fibo_gpio_mode_set(22,0);
    APP_LOG_INFO("fibo_gpio_mode_set ret = %d", ret); //set pin 22 to gpio mode

    ret = fibo_gpio_isr_init(22, (oc_isr_t *) &demo_isr); //the 22 is pin_id which trigger isr
    APP_LOG_INFO("fibo_gpio_isr_init ret = %d", ret);

    for (int n = 0; n < 10000; n++)
    {
        APP_LOG_INFO("normal isr low_level count: %d high_level count:%d", normal_low_level, normal_high_level);
        fibo_taskSleep(1000);
    }

}

static void gpio_wakeup_isr_thread_entry(void *param)
{
/*------------test gpio interrupt--------------*/
    oc_isr_t demo_isr;
    APP_LOG_INFO("application wakeup isr thread enter, param 0x%x", param);
    set_app_ver(app_ver);
    int ret = -1;

    memset(&demo_isr,0,sizeof(demo_isr));

    demo_isr.debounce = true;
    demo_isr.intr_enable = true;
    demo_isr.intr_level = false;
    demo_isr.falling = true;
    demo_isr.rising = false;
    demo_isr.pullen = true;
    demo_isr.pullup = true;
    demo_isr.is_wakeup = true;
    demo_isr.cb = oc_gpiowakeupisrcallback;

    ret = fibo_gpio_mode_set(19,0);
    APP_LOG_INFO("fibo_gpio_mode_set ret = %d", ret); //set pin 19 to gpio mode

    ret = fibo_gpio_isr_init(19, (oc_isr_t *) &demo_isr); //the 19 is pin_id which trigger isr
    APP_LOG_INFO("fibo_gpio_isr_init ret = %d", ret);

    for (int n = 0; n < 10000; n++)
    {
        APP_LOG_INFO("wakeup isr low_level count: %d high_level count:%d", wakeup_low_level, wakeup_high_level);
        fibo_taskSleep(1000);
    }

    
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("app image enter");
    fibo_thread_create(gpio_switching_level_thread_entry, (INT8*)"gpio_Level_switching_thread", 1024*4, NULL, osPriorityBelowNormal);
    fibo_thread_create(gpio_normal_isr_thread_entry, (INT8*)"gpio_isr_normal_thread", 1024*4, NULL, osPriorityBelowNormal);
    fibo_thread_create(gpio_wakeup_isr_thread_entry, (INT8*)"gpio_isr_wakeup_thread", 1024*4, NULL, osPriorityBelowNormal);

    return (void *)&user_callback;
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
}

