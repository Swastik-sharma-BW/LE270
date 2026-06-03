#ifndef __FFW_LCD_H__
#define __FFW_LCD_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LCD_DIRECT_NORMAL = 0x00,
    LCD_DIRECT_ROT_90, // Rotation 90
    LCD_DIRECT_ROT_180, // Rotation 180
    LCD_DIRECT_NORMAL2,
    LCD_DIRECT_NORMAL2_ROT_90,
    LCD_DIRECT_NORMAL2_ROT_180,
    LCD_DIRECT_ROT_270,  // Rotation 270
    LCD_DIRECT_NORMAL2_ROT_270,
} lcdDirect_t;

//Block Configuration
typedef struct
{
    uint16_t start_x;
    uint16_t start_y;
    uint16_t end_x;
    uint16_t end_y;
    uint16_t colorkey;
    uint16_t width;     //real data width in SDRAM
    uint8_t resolution; //0:RGB888; 1:RGB666; 2:RGB565(RGB555 if OSD)
    uint8_t type;       //0:OSD; 1:image
    uint8_t alpha;
    uint8_t colorkey_en;
    uint32_t rotation;
} lcdBlockCfg_t;

typedef struct
{
    /// x coordinate of the top left corner of the lcd.
    uint16_t x;
    /// y coordinate of the top left corner of the lcd.
    uint16_t y;
    /// width of the lcd dislay area.
    uint16_t width;
    /// height of the lcd display area.
    uint16_t height;

} lcdDisplay_t;

typedef struct
{
    uint8_t contrastRatio_coarse; 
    uint8_t contrastRatio_fine;
    uint8_t bias;
    uint8_t rate;  //reflesh lcd rate
    uint8_t direction; 
    uint32_t reserved;
    uint32_t reserved2;
    uint32_t reserved3;
}doMatrixParm_t;

//resolution of lcd data

#define LCD_RESOLUTION_RGB565 2
#define LCD_RESOLUTION_YUV422_UYVY 5
#define LCD_RESOLUTION_YUV420 6
#define LCD_RESOLUTION_YUV422_YUYV 9
//inBufFormat
#define YUYV 0
#define YVYU 1
#define UYVY 2

//=============================================================================
// lcdFrameBuffer_t
//-----------------------------------------------------------------------------
/// Frame buffer type: this type describes a frame buffer through its dimensions
/// and a pointer to the data of the buffer. It is mandatory for the buffer to
/// be able to hold the data describing all the pixels at the color format
/// choosen.
///(ie sizeof(buffer) = width*height*NbOfBytesPerPixelAtTheFormat(format))
// colorFormat : support LCD_RESOLUTION_RGB565 ,LCD_RESOLUTION_YUV422 LCD_RESOLUTION_YUV420
//widthOrignal  : the width of the original image buffer,it will just be used to caculate the stribe
//width : the width will convert to. Normal is will equal with widthOrignal,unless zoom,rotation.
//height : the height will convert to.
//=============================================================================
typedef struct
{
    uint16_t *buffer;
    ///  x coordinate of the layer. If only use one layer,normal set 0.
    uint16_t region_x;
    ///  y coordinate of the layer. If only use one layer,normal set 0.
    uint16_t region_y;
    ///the width of after convert. if don't need rotation and zoom ,set to  roginal width.
    uint16_t width;
    ///the height of after convert. if don't need rotation and zoom ,set to  roginal height.
    uint16_t height;
    ///image buffer width.
    uint16_t widthOriginal;
    uint16_t colorFormat;
    ///0 no rotation. 1 rotate 90 degree.
    uint16_t rotation;
    /// enable the color mask function.
    bool keyMaskEnable;
    /// mask color value
    uint16_t maskColor;
} lcdFrameBuffer_t;

int ffw_lcd_init(void);
int ffw_lcd_deinit(void);
int ffw_lcd_sleepMode(bool mode);
int ffw_lcd_frameTransfer(const lcdFrameBuffer_t * pstFrame, const lcdDisplay_t * pstWindow);
int ffw_lcd_setOverLay(const lcdFrameBuffer_t * pstFrame);
int ffw_lcd_setBrushDirection(lcdDirect_t direct_type);
int ffw_lcd_setPixel(uint16_t ulx, uint16_t uly, uint16_t ulcolor);
int ffw_lcd_fillRect(const lcdDisplay_t * pstWindow, uint16_t ulBgcolor);
int ffw_lcd_drawLine(uint16_t ulSx, uint16_t ulSy, uint16_t ulEx, uint16_t ulEy,uint16_t ulColor);
int ffw_lcd_getInfo(uint32_t * puldevid, uint32_t * pulwidth, uint32_t *pulheight);
int ffw_lcd_setvideosize(uint32_t width, uint32_t height, uint32_t imageBufWidth);
void ffw_dotMatrix_lcd_setConfig(doMatrixParm_t *cfg);
void ffw_dotMatrix_lcd_sendData(uint8_t column, uint8_t row, void *buff, uint32_t size);
#endif

