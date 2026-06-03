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

static HAL_I2C_BPS_T bsp1_1 = {2, 1};
UINT8 data[6] = {0xfd,0xfe};
UINT8 data1[2] = {0x21, 0x30};
i2c_Handle i2c_handle = 0;
int result = 0;
drvI2cSlave_t drv_i2c= {0x18, 0, 0, false, false};
UINT8 data2[2] ={0x00};

#define EC718_PLAT

#define I2C_SDA_PIN 66
#define I2C_SCL_PIN 67
#define I2C_SDA_MUX 2
#define I2C_SCL_MUX 2

#ifdef EC718_PLAT
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
#endif

static void i2c_thread_entry(void *param)
{
    int ret;
    APP_LOG_INFO("application thread enter, param 0x%x", param);
    set_app_ver(app_ver);

    fibo_gpio_mode_set(I2C_SDA_PIN, I2C_SDA_MUX);// le270-i2c1 sda gpio8 func2
    fibo_gpio_mode_set(I2C_SCL_PIN, I2C_SCL_MUX);// le270-i2c1 scl gpio9 func2

    result = fibo_i2c_open(bsp1_1, &i2c_handle);
    APP_LOG_INFO("i2c handle is %p", i2c_handle);
    APP_LOG_INFO("test_i2c open result %d", result);
    fibo_taskSleep(1000);

    result = fibo_i2c_Write(i2c_handle, drv_i2c, data1, 2);
    APP_LOG_INFO("test_i2c i2c_send_data result = %d", result);
    APP_LOG_INFO("test_i2c i2c_send_data data = %d, %d", data1[0], data1[1]);

    while(1)
    {
        fibo_taskSleep(1000);
        ret = fibo_i2c_xfer(i2c_handle, drv_i2c, &data[0], 1, &data2[0], 1);
        result = fibo_i2c_xfer(i2c_handle, drv_i2c, &data[1], 1, &data2[1], 1);
        APP_LOG_INFO("test_i2c i2c_get_data ret=%d,%d, data = 0x%x,0x%x", ret,result,data2[0], data2[1]);
    }

    
}

void * appimg_enter(void *param)
{
    APP_LOG_INFO("application image enter, param 0x%x", param);

    fibo_thread_create(i2c_thread_entry, (INT8*)"i2c_custhread", 1024*4, NULL, osPriorityBelowNormal);

#ifdef EC718_PLAT
    return (void *)&user_callback;
#else
    return 0;
#endif
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
}

