#ifndef _FFW_PAL_LCD_H_
#define _FFW_PAL_LCD_H_
#include "ffw_lcd.h"

int ffw_pal_lcd_device_init();

void ffw_pal_lcd_device_deInit();

int ffw_pal_lcd_checkId();

void ffw_pal_lcd_sleepMode(uint8_t mode);

void ffw_pal_lcd_setPixel(uint16_t ulx, uint16_t uly, uint16_t ulcolor);

void ffw_pal_lcd_setWindow(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey);

int  ffw_pal_lcd_fillRect(uint8_t *buf,uint32_t fillLen);

void ffw_pal_lcd_drawLine(uint16_t ulSx, uint16_t ulSy, uint16_t ulEx, uint16_t ulEy,uint16_t ulColor);

void ffw_pal_lcd_setBrushDirection(lcdDirect_t direct_type);

void ffw_pal_lcd_getInfo(uint32_t * devid, uint32_t * width, uint32_t *height);

void ffw_pal_dotMatrix_lcd_setConfig(doMatrixParm_t *cfg);

void ffw_pal_dotMatrix_lcd_sendData(uint8_t column, uint8_t row, void *buff, uint32_t size);

uint32_t ffw_pal_get_lcd_pixel();
#endif
