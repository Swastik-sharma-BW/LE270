#ifndef  LCDDEV_7567_H
#define  LCDDEV_7567_H

#include "lcdDrv.h"

#define HEIGHT_7567             (64)
#define WIDTH_7567              (128)


#define     ST7567_BPP              (1) // 12: 444;   16:565;   18:666
#define     ST7567_WIDTH            (128)
#define     ST7567_HEIGHT           (64)
#define     ST7567_FREQ             (6*1024*1024)
#define     ST7567_TIME_OF_FRAME    (149356) // us
#define     ST7567_TE_CYCLE         (16742) // us
#define     ST7567_TE_WAIT_TIME     (623)   // us
#define     ST7567_X_OFFSET         (0)
#define     ST7567_Y_OFFSET         (0)


#undef      LCD_WIDTH
#undef      LCD_HEIGHT
#define     LCD_HEIGHT           (ST7567_HEIGHT)
#define     LCD_WIDTH            (ST7567_WIDTH)
#define     LCD_BPP_USE          (ST7567_BPP)
#define     LCD_FREQ             (ST7567_FREQ)
#define     LCD_X_OFFSET         (ST7567_X_OFFSET)
#define     LCD_Y_OFFSET         (ST7567_Y_OFFSET)

#define     LCD_PIXEL            (LCD_HEIGHT*LCD_WIDTH)
#define     LCD_TIME_OF_FRAME    (ST7567_TIME_OF_FRAME) // us
#define     LCD_TE_CYCLE         (ST7567_TE_CYCLE)      // us
#define     LCD_TE_WAIT_TIME     (ST7567_TE_WAIT_TIME)  // us
#endif
