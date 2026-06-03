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
#include "oc_lcd.h"
#include "app.h"
#include "app_pdp.h"
#include "oc_osi.h"

UINT16 g_testBuff_128_160[40 * 80];
UINT16 g_TestBuffer[320 * 240];

void test_colorbar_buffer(UINT16 *p, int w, int h)
{
    int i, j;
    for (i=0; i<h; i++)
    {
        for (j=0; j< w; j++)
        {
            if (j<w/4)
            {
                p[i*w+j] = 0x001f;
            }
            else if (j<w/2)
            {
                p[i*w+j] = 0xf000;
            }
            else if (j< w*3/4)
            {
                p[i*w+j] = 0x07f0;
            }
            else
            {
                p[i*w+j] = 0xf80f;
            }
        }
    }

}
void fill_word_buff(UINT16 *p, int w, int h)
{
    UINT16 i, j;
    for (i=0; i<h; i++)
    {
        for (j=0; j<w; j++)
        {
            p[i*w+j] = 0xf800;
        }
    }

    for (i=10; i<120; i++)
    {
        for (j=80; j<90; j++)
        {
            p[i * w + j] = 0x001f;
        }
    }
    for (i=10; i<120; i++)
    {
        for (j=180; j<190; j++)
        {
            p[i*w+j] = 0x001f;
        }
    }
    for (i=60; i<70; i++)
    {
        for (j=90; j<180; j++)
        {
            p[i*w+j] = 0x001f;
        }
    }
}

static void lcd_thread_entry(void *param)
{
    APP_LOG_INFO("application thread enter, param 0x%x", param);
    int ret = 0;
    lcdDisplay_t startPoint;
    lcdFrameBuffer_t data;

    ret = fibo_lcd_init();
    if (ret != 0)
    {
        APP_LOG_INFO("fibo_lcd_init failed:%d.", ret);
    }
    else
    {
        APP_LOG_INFO("fibo_lcd_init success.");
    }


    for (int n=0; n<30; n++)
    {
        APP_LOG_INFO("hello world %d", n);

        fibo_lcd_SetBrushDirection(LCD_DIRECT_NORMAL);
        fibo_taskSleep(2 * 1000);

        startPoint.x = 0;
        startPoint.y = 0;
        startPoint.width = 240;
        startPoint.height =320;
        fibo_lcd_FillRect(&startPoint, WHITE);
        fibo_taskSleep(2 * 1000);
        startPoint.x = 50;
        startPoint.y = 100;
        startPoint.width = 100;
        startPoint.height = 50;
        fibo_lcd_FillRect(&startPoint, BLUE);

        fibo_taskSleep(2 * 1000);
        fibo_lcd_SetPixel(10, 10, RED);
        fibo_taskSleep(2 * 1000);
        fibo_lcd_SetPixel(10, 20, GREEN);
        fibo_taskSleep(2 * 1000);

        fibo_lcd_DrawLine(1, 170, 200, 310, BLUE);
        fibo_taskSleep(2 * 1000);
        fibo_lcd_DrawLine(200, 170, 1, 310, RED);
        fibo_taskSleep(2 * 1000);
        fibo_lcd_DrawLine(200, 240, 1, 240, GREEN);
        fibo_taskSleep(2 * 1000);
        fibo_lcd_DrawLine(100, 310, 100, 170, BLUE);
        fibo_taskSleep(2 * 1000);

        startPoint.x = 0;
        startPoint.y = 0;
        startPoint.width = 240;
        startPoint.height = 320;
        fibo_lcd_FillRect(&startPoint, RED);
        fibo_taskSleep(2 * 1000);

        test_colorbar_buffer(g_testBuff_128_160, 40, 80);
        data.buffer = (uint16_t *)g_testBuff_128_160;

        startPoint.x = 60;
        startPoint.y = 60;
        startPoint.height = 80;
        startPoint.width = 40;
        fibo_lcd_FrameTransfer(&data, &startPoint);
        fibo_taskSleep(2*2000);

        fill_word_buff(g_TestBuffer, 240, 320);
        data.buffer = (uint16_t *)g_TestBuffer;

        startPoint.x = 0;
        startPoint.y = 0;
        startPoint.width = 240;
        startPoint.height = 320;
        fibo_lcd_FrameTransfer(&data, &startPoint);
        fibo_taskSleep(5000);
    }
    fibo_lcd_deinit();
}

void * appimg_enter(void *param)
{
    APP_LOG_INFO("application image enter, param 0x%x", param);
    fibo_thread_create(lcd_thread_entry, (int8_t*)"lcd_custhread", 1024 * 8, NULL, OSI_PRIORITY_NORMAL);
    return 0;
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
}
