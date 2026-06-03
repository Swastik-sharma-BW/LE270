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


#define OSI_LOG_TAG OSI_MAKE_LOG_TAG('M', 'Y', 'A', 'P')

#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"


#define LCD_ID_ST7567       0x7567//0X858552
#define LCD_WIDTH  128
#define LCD_HEIGHT 64
#define LCD_BUFF_MAX  (LCD_WIDTH) * ((LCD_HEIGHT) / 8)
#define LCD_PAGE   8


lcdOperations_t lcdOpt;

static const lcdSpec_t g_lcd_st7567 =
    {
        LCD_ID_ST7567,
        LCD_WIDTH,
        LCD_HEIGHT,
        HAL_GOUDA_SPI_LINE_4,
        LCD_CTRL_SPI,
       (lcdOperations_t *)&lcdOpt,
        false,
        0,
        1000000,
};

static lcdCfg_t g_lcd_config =
    {
        (lcdSpec_t *)&g_lcd_st7567,
        LCD_POWER_1V8,
        true,
        0,
        0,
        0,
};

uint8_t test_buf1[10]={0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
uint8_t test_buf2[15]={0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb};
uint8_t test_buf3[20]={0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc};
uint8_t test_buf4[25]={0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd};
uint8_t test_buf5[30]={0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee};

uint8_t lcd_page_buf[LCD_WIDTH];

uint16_t device_id = 0 ;
uint8_t status =0;
static void lcd_doMatrix_demo()
{
    APP_LOG_INFO("lcd_doMatrix_demo..");
    fibo_taskSleep(3000);
    // srand(100);
    uint16_t pin = 22;
    UINT8 mode = 0;//0-gpio ,5-pwm;

    fibo_gpio_mode_set(pin, mode);
    fibo_gpio_cfg(pin,1);
    fibo_gpio_set(pin,1);

    int ret = 0;
    ret=fibo_lcd_register(&g_lcd_config, NULL);
    if(ret !=0)
    {
      APP_LOG_INFO("fibo_lcd_register failed:%d.", ret);
    }

    ret=fibo_lcd_init();
    if (ret != 0)
    {
        APP_LOG_INFO("fibo_lcd_init failed:%d.", ret);
    }
    else
    {
        APP_LOG_INFO("fibo_lcd_init success.");
    }

#if 1
    doMatrixParm_t cfg;

    cfg.contrastRatio_coarse = 0x04;
    cfg.contrastRatio_fine = 0x20;
    cfg.bias  = 0x00;
    cfg.direction= 0x01;

    fibo_dotMatrix_lcd_setConfig(&cfg);
#endif
    for (int n=0; n<100; n++)
    {
        fibo_textTrace("hello world %d", n);

       memset(lcd_page_buf,0,LCD_WIDTH);
       for(int i = 0; i < LCD_PAGE ; i++)
        {
            fibo_dotMatrix_lcd_sendData(0,i,lcd_page_buf,LCD_WIDTH);
        }
        fibo_taskSleep(3000);

#if 1
        fibo_dotMatrix_lcd_sendData(0,0,test_buf1,10);
        fibo_taskSleep(3000);
        fibo_dotMatrix_lcd_sendData(20,1,test_buf2,15);
        fibo_taskSleep(3000);
        fibo_dotMatrix_lcd_sendData(30,3,test_buf3,20);
        fibo_taskSleep(3000);
        fibo_dotMatrix_lcd_sendData(40,5,test_buf4,25);
        fibo_taskSleep(3000);
        fibo_dotMatrix_lcd_sendData(60,7,test_buf5,30);
        fibo_taskSleep(5000);
#endif
        fibo_lcd_SetPixel(100,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,1,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,2,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,3,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,4,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,5,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,6,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,7,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,8,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,9,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,10,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,11,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(100,12,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(101,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(102,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(103,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(104,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(105,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(106,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(107,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(108,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(109,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(110,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(111,0,0x01);
        fibo_taskSleep(2000);
        fibo_lcd_SetPixel(112,0,0x01);
        fibo_taskSleep(2000);

        memset(lcd_page_buf,0xff,LCD_WIDTH);
        for(int i = 0; i < LCD_PAGE ; i++)
        {
            fibo_dotMatrix_lcd_sendData(0,i,lcd_page_buf,LCD_WIDTH);
        }
        fibo_taskSleep(3000);
    }
    fibo_lcd_deinit();
}

void * appimg_enter(void *param)
{
    fibo_taskSleep(3000);
    APP_LOG_INFO("application appimg_enter");
    return app_init(lcd_doMatrix_demo);

}

