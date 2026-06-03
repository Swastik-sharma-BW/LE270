#ifndef __OC_LCD_H__
#define __OC_LCD_H__
#include "fibo_opencpu_comm.h"
#include "ffw_lcd.h"

// color code
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40
#define BRRED            0XFC07
#define GRAY             0X8430

INT32 fibo_lcd_init(void);

INT32 fibo_lcd_deinit(void);

INT32 fibo_lcd_Sleep(BOOL mode);

INT32 fibo_lcd_FrameTransfer(const lcdFrameBuffer_t * pstFrame, const lcdDisplay_t * pstWindow);

INT32 fibo_lcd_SetOverLay(const lcdFrameBuffer_t * pstFrame);

INT32 fibo_lcd_SetBrushDirection(lcdDirect_t direct_type);

INT32 fibo_lcd_SetPixel(UINT16 ulx, UINT16 uly, UINT16 ulcolor);

INT32 fibo_lcd_FillRect(const lcdDisplay_t * pstWindow, UINT16 ulBgcolor);

INT32 fibo_lcd_DrawLine(UINT16 ulSx, UINT16 ulSy, UINT16 ulEx, UINT16 ulEy,UINT16 ulColor);

INT32 fibo_lcd_Getinfo(uint32_t * puldevid, uint32_t * pulwidth, uint32_t *pulheight);

INT32 fibo_lcd_Setvideosize(uint32_t width, uint32_t height, uint32_t imageBufWidth);

void fibo_dotMatrix_lcd_setConfig(doMatrixParm_t *cfg);

void fibo_dotMatrix_lcd_sendData(uint8_t column, uint8_t row, void *buff, uint32_t size);

#endif

