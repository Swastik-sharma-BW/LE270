#include "bsp.h"
#include "lcdDrv.h"
#include "lcdComm.h"
#include "lcdDev_7789.h"
#include "sctdef.h"
#ifdef FIBOCOM_BASE
#include DEBUG_LOG_HEADER_FILE
#include "oc_gpio.h"
#endif

#define LCD_MADCTL  0x36

#ifdef FIBOCOM_BASE
#define PIC_SRC_HEIGHT 320
#define PIC_SRC_WIDTH  240

#undef ST7789_BPP
#define ST7789_BPP      (18)
#undef ST7789_FREQ
#define ST7789_FREQ      (41*1024*1024)
#undef LCD_INTERFACE
#define LCD_INTERFACE    (SPI_4W_I)

#define COLOR_RGB_SWAP //ec need define
#ifdef COLOR_RGB_SWAP
#define CLOOR_RGB   0x08
#else
#define CLOOR_RGB   0x00
#endif

#endif

extern lspiDrvInterface_t *lcdDrv;
//static uint8_t s_MADCTL = 0x0; 
AP_PLAT_COMMON_BSS static uint16_t previewWidth;
AP_PLAT_COMMON_BSS static uint16_t previewHeight;
extern lcdIoCtrl_t lcdIoCtrlParam;

AP_PLAT_COMMON_DATA static  initLine_t initTable7789[] = 
{
/*
 *  FIBOCOM: GD024Q569B
 *	  st7789v2
 */
#if 0
    {0x11, 1, {0}},
    {0xff, 1, {120}}, //sleep 120ms
    {0x36, 1, {0x0|CLOOR_RGB}},//0x00=0 0x60=90 0xc0=180 0xA0=270
    //{0x21, 1, {0}}, // display inverse
#if (ST7789_BPP == 16)
    {0x3a, 1, {0x55}}, // rgb565
#elif (ST7789_BPP == 12)
    {0x3a, 1, {3}}, // rgb444
#elif (ST7789_BPP == 18)
    {0x3a, 1, {6}}, // rgb666
#endif
  //  {0x35, 1, {0}}, // te on
#if (SPI_2_DATA_LANE == 1)
    {0xe7, 1, {0x10}},
#endif
    {0xb2, 5, {0x0c, 0x0c, 0x00, 0x33, 0x33}}, // rate
    {0xb7, 1, {0x35}},
    {0xbb, 1, {0x1c}},
    {0xc0, 1, {0x2c}},
    {0xc2, 1, {0x01}},
    {0xc3, 1, {0x0b}},
    {0xc4, 1, {0x20}},
    {0xc6, 1, {0x0f}},
    {0xd0, 2, {0xa4, 0xa1}}, // pwctr
    {0xe0, 14, {0xd0,0x00,0x03,0x09,0x13,0x1c,0x3a,0x55,0x48,0x18,0x12,0x0e,0x19,0x1e}}, //Positive voltage gamma
    {0xe1, 14, {0xd0,0x00,0x03,0x09,0x05,0x25,0x3a,0x55,0x50,0x3d,0x1c,0x1d,0x1d,0x1e}}, //Negative voltage gamma
    {0x29, 1, {0x00}},
#endif

/*
 *  CONFIG_CUST_ONE_LCD: U3403BA-ZA0
 *	  st7789p3
 */
#if 1
    {0x11, 1, {0}},
    {0xff, 1, {120}}, //sleep 120ms
    {0x36, 1, {0x0|CLOOR_RGB}},//0x00=0 0x60=90 0xc0=180 0xA0=270
    //{0x21, 1, {0}}, // display inverse
#if (ST7789_BPP == 16)
    {0x3a, 1, {0x55}}, // rgb565
#elif (ST7789_BPP == 12)
    {0x3a, 1, {3}}, // rgb444
#elif (ST7789_BPP == 18)
    {0x3a, 1, {6}}, // rgb666
#endif
  //  {0x35, 1, {0}}, // te on
#if (SPI_2_DATA_LANE == 1)
    {0xe7, 1, {0x10}},
#endif
    {0xb0, 2, {0x00, 0xE0}},
    {0xb2, 5, {0x0c, 0x0c, 0x00, 0x33, 0x33}}, // rate
    {0xb7, 1, {0x02}},
    {0xbb, 1, {0x19}},
    {0xc0, 1, {0x2c}},
    {0xc2, 1, {0x01}},
    {0xc3, 1, {0x13}},
    {0xc6, 1, {0x0f}},
    {0xd0, 1, {0xa7}},
    {0xd0, 2, {0xa4, 0xa1}}, // pwctr
    {0xd6, 1, {0xa1}},
    {0xe0, 14, {0xf0,0x0f,0x16,0x07,0x08,0x25,0x36,0x54,0x4B,0x37,0x13,0x13,0x2e,0x35}}, //Positive voltage gamma
    {0xe1, 14, {0xf0,0x10,0x14,0x08,0x06,0x23,0x35,0x44,0x4a,0x38,0x10,0x10,0x2c,0x33}}, //Negative voltage gamma
    {0x21, 1, {0x00}},
    {0x29, 1, {0x00}},
#endif

#if 0 //EC lcd
    {0x11, 1, {0}},
    {0xff, 1, {120}},
    {0x36, 1, {0}},
    {0x21, 1, {0}}, // display inverse
#if (ST7789_BPP == 16)    
    {0x3a, 1, {5}}, // rgb565
#elif (ST7789_BPP == 12)
    {0x3a, 1, {3}}, // rgb444
#elif (ST7789_BPP == 18)    
    {0x3a, 1, {6}}, // rgb666
#endif    
    {0x35, 1, {0}}, // te on
#if (SPI_2_DATA_LANE == 1)
    {0xe7, 1, {0x10}},
#endif    
    {0xb2, 5, {0x0c, 0x0c, 0x00, 0x33, 0x33}}, // rate
    {0xb7, 1, {0x35}},
    {0xbb, 1, {0x20}},
    {0xc0, 1, {0x2c}},
    {0xc2, 1, {0x01}},
    {0xc3, 1, {0x0b}},
    {0xc4, 1, {0x20}},
    {0xc6, 1, {0x0f}},
    {0xd0, 2, {0xa4, 0xa1}}, // pwctr
    {0xe0, 14, {0xd0,0x03,0x09,0x0e,0x11,0x3d,0x47,0x55,0x53,0x1a,0x16,0x14,0x1f,0x22}}, //Positive voltage gamma
    {0xe1, 14, {0xd0,0x02,0x08,0x0d,0x12,0x2c,0x43,0x55,0x53,0x1e,0x1b,0x19,0x20,0x22}}, //Negative voltage gamma
    {0x29, 1, {0x00}},
#endif
};

void st7789HandleUspIrq4Cam(lcdDrvFunc_t *lcd)
{

}

void st7789HandleUspIrq4Fill(lcdDrvFunc_t *lcd)
{
    LSPI2->STAS |= 1<<13; // clear ram wr int end. 1,1/2,1/4...
}

#ifdef FIBOCOM_BASE
int st7789_gpio_init(void)
{
    //LCD_RST
    fibo_gpio_mode_set(LCD_RST_PIN, LCD_RST_PIN_GPIO_MODE);
    fibo_gpio_cfg(LCD_RST_PIN, 1);

    //LCD_INTERFACE
    fibo_gpio_mode_set(LCD_CLK_PIN, LCD_CLK_PIN_MODE);
    fibo_gpio_mode_set(LCD_CS_PIN, LCD_CS_PIN_MODE);
    fibo_gpio_mode_set(LCD_TE_PIN, LCD_TE_PIN_MODE);
    fibo_gpio_mode_set(LCD_SIO_PIN, LCD_SIO_PIN_MODE);
    fibo_gpio_mode_set(LCD_SDC_PIN, LCD_SDC_PIN_MODE);

    return 0;
}

static int st7789Init(lcdDrvFunc_t *lcd, void* uspCb, void *dmaCb, uint32_t freq, uint8_t bpp)
{
    st7789_gpio_init();
    lcdRst(10, 10);
    dmaInit(dmaCb);
    lspiDefaultCfg(lcd, uspCb, freq, bpp);
    lspiDataFmt.txFifoEndianMode = 1;
    lcdInterfaceType(LCD_INTERFACE);
    lcdRegInit(0x7789);

    #if 0 // test read id
    uint32_t read7789[3] = {0};
    lspiReadReg(0x04, read7789, 3, 0);
    #endif
    return 0;
}

static void st7789Direction(lcdDrvFunc_t *lcd,uint8_t Dir)
{
    switch (Dir)
    {
        case LCD_DIRECT_NORMAL:
            //Vertical screen  startpoint at topleft
            lspiCmdSendByteData(0x36, 0x00|CLOOR_RGB);
        break;
        case LCD_DIRECT_ROT_90:
            //Horizontal screen  startpoint at topleft
            lspiCmdSendByteData(0x36, 0x60|CLOOR_RGB);
        break;
        case LCD_DIRECT_ROT_180:
            //Horizontal screen  startpoint at topleft
            lspiCmdSendByteData(0x36, 0xC0|CLOOR_RGB);
        break;
        case LCD_DIRECT_ROT_270:
            //Horizontal screen  startpoint at topleft
            lspiCmdSendByteData(0x36, 0xA0|CLOOR_RGB);
        break;
        case LCD_DIRECT_NORMAL2:
            //Vertical screen  startpoint at bottomleft
            lspiCmdSendByteData(0x36, 0x80|CLOOR_RGB);
        break;
        case LCD_DIRECT_NORMAL2_ROT_90:
            //Horizontal screen  startpoint at bottomleft
            lspiCmdSendByteData(0x36, 0x20|CLOOR_RGB);
        break;
        case LCD_DIRECT_NORMAL2_ROT_180:
            //Horizontal screen  startpoint at bottomleft
            lspiCmdSendByteData(0x36, 0x40|CLOOR_RGB);
        break;
        case LCD_DIRECT_NORMAL2_ROT_270:
            //Horizontal screen  startpoint at topleft
            lspiCmdSendByteData(0x36, 0xE0|CLOOR_RGB);
        break;
        default:
            lspiCmdSendByteData(0x36, 0x00|CLOOR_RGB);
        break;
    }

    lcdWriteCmd(0x2c);
}
#else
static int st7789Init(lcdDrvFunc_t *lcd, void* uspCb, void *dmaCb, uint32_t freq, uint8_t bpp)
{     
    lcdIoInit(false);
    lcdRst(10, 10);
    dmaInit(dmaCb);
    lspiDefaultCfg(lcd, uspCb, freq, bpp);
    lcdInterfaceType(ST7789_INTERFACE);
    lcdRegInit(0x7789);
    
    #if 0 // test read id
    uint32_t read7789[3] = {0};
    lspiReadReg(0x04, read7789, 3, 0);
    #endif
    return 0;
}

#if 0
static int st7789Direction(lcdDrvFunc_t *lcd,DisDirection_e Dir)
{
    switch(Dir)
    {
        case DIS_MIRROR_X:  
            s_MADCTL ^= BIT(6);
            break;
        case DIS_MIRROR_Y:  
            s_MADCTL ^= BIT(7);
            break;   
        case DIS_SWAP_XY:   
            s_MADCTL ^= 0xC0;
            break; 
        case DIS_DIR_LRTB: 
            s_MADCTL &= ~(BIT(2));   
            s_MADCTL &= ~(BIT(4)); 
            s_MADCTL &= ~(BIT(5));
            break;
        case DIS_DIR_LRBT: 
            s_MADCTL &= ~(BIT(2)); 
            s_MADCTL &= ~(BIT(5)); 
            s_MADCTL |= BIT(4);
            break;   
        case DIS_DIR_RLTB:   
            s_MADCTL &= ~(BIT(4)); 
            s_MADCTL &= ~(BIT(5)); 
            s_MADCTL |= BIT(2);
            break; 
        case DIS_DIR_RLBT: 
            s_MADCTL &= ~(BIT(5));
            s_MADCTL |=  BIT(4);  
            s_MADCTL |=  BIT(2); 
            break; 
        case DIS_DIR_TBLR: 
            s_MADCTL &= ~(BIT(2)); 
            s_MADCTL &= ~(BIT(4));
            s_MADCTL |= BIT(5);  
            break; 
        case DIS_DIR_BTLR:  
            s_MADCTL &= ~(BIT(2));
            s_MADCTL |= BIT(4); 
            s_MADCTL |= BIT(5);   
            break; 
        case DIS_DIR_TBRL:  
            s_MADCTL &= ~(BIT(4));
            s_MADCTL |= BIT(2); 
            s_MADCTL |= BIT(5);
            break; 
        case DIS_DIR_BTRL: 
            s_MADCTL |= BIT(2); 
            s_MADCTL |= BIT(4); 
            s_MADCTL |= BIT(5);  
            break; 
        case DIS_DIR_MAX:
        break;
    }
    
    lspiCmdSend(LCD_MADCTL, &s_MADCTL, 1);
    return s_MADCTL;
}
#endif

static void st7789BackLight(lcdDrvFunc_t *lcd, uint8_t level)
{
#if (BK_USE_GPIO == 1)
    lcdGpioBkLevel(level);
#elif (BK_USE_PWM == 1)
    lcdPwmBkLevel(level);
#endif
}
#endif

/* 
    sx: start x
    sy: start y
    ex: end x
    ey: end y
*/
static uint32_t st7789AddrSet(lcdDrvFunc_t *lcd, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
    uint8_t set_x_cmd[] = {sx>>8, sx, ex>>8, ex};
    lspiCmdSend(0x2A, set_x_cmd, sizeof(set_x_cmd));
	previewWidth = ex - sx + 1;
    
    uint8_t set_y_cmd[] = {sy>>8, sy, ey>>8, ey};
    lspiCmdSend(0x2B, set_y_cmd, sizeof(set_y_cmd));
	previewHeight = ey - sy + 1;
    
    lcdWriteCmd(0x2C);

    return (ey-sy+1) * (ex-sx+1) * (ST7789_BPP/8);
}

static void st7789StartStop(lcdDrvFunc_t *lcd, bool startOrStop)
{
    dmaStartStop(startOrStop);
}

static int st7789Fill(lcdDrvFunc_t *lcd, uint32_t fillLen, uint8_t *buf)
{
    return lcdDmaTrans(lcd, buf, fillLen);
}

static void st7789DrawPoint(lcdDrvFunc_t *lcd, uint16_t x, uint16_t y, uint32_t dataWrite)
{
#ifdef FIBOCOM_BASE
    st7789AddrSet(lcd, x, y, x, y);
#endif
    lspiFifoWrite(dataWrite);
    return;
}

static void st7789RegisterUspIrqCb(lcdDrvFunc_t *lcd, uspCbRole_e who)
{
    if (lcd == NULL)
    {
        EC_ASSERT(0,0,0,0);
    }

    (who == cbForCam)? (lcd->uspIrq4CamCb = st7789HandleUspIrq4Cam) : (lcd->uspIrq4FillCb = st7789HandleUspIrq4Fill);
}

static void st7789UnRegisterUspIrqCb(lcdDrvFunc_t *lcd, uspCbRole_e who)
{
    if (lcd == NULL)
    {
        EC_ASSERT(0,0,0,0);
    }

    (who == cbForCam)? (lcd->uspIrq4CamCb = NULL) : (lcd->uspIrq4FillCb = NULL);
}

static void st7789CamPreviewStartStop(lcdDrvFunc_t *lcd, camPreviewStartStop_e previewStartStop)
{   
	uint8_t  divVal = 0;
	uint16_t divRemain = 0;
	
#if defined TYPE_EC718M
    if (previewStartStop)
    {
		st7789RegisterUspIrqCb(lcd, cbForCam);
		st7789UnRegisterUspIrqCb(lcd, cbForFill);
		
		lspiDataFmt.wordSize = 7;
		lspiDataFmt.txPack = 0;
		lcdDrv->ctrl(LSPI_CTRL_DATA_FORMAT, 0);

		lspiDmaCtrl.txDmaReqEn			= 0;
		lcdDrv->ctrl(LSPI_CTRL_DMA_CTRL, 0);

        lspiInfo.frameHeight            = PIC_SRC_HEIGHT;  	// frame input height
        lspiInfo.frameWidth             = PIC_SRC_WIDTH;   // frame input width
        lspiFrameInfoOut.frameHeightOut = previewHeight;   	// frame output height
        lspiFrameInfoOut.frameWidthOut  = previewWidth;    	// frame output width

		if ((previewHeight > PIC_SRC_HEIGHT) || (previewWidth > PIC_SRC_WIDTH))
		{
			EC_ASSERT(0,0,0,0);
		}

		if (lcdIoCtrlParam.previewModeSel == CAM_PREVIEW_SET_AUTO)
		{
			divVal 		= lspiInfo.frameHeight / lspiFrameInfoOut.frameHeightOut;
			divRemain	= lspiInfo.frameHeight % lspiFrameInfoOut.frameHeightOut;	
			lspiTailorInfo0.tailorTop		= divRemain/2;
			lspiTailorInfo0.tailorBottom	= divRemain/2;
			EC_ASSERT(divVal, lspiInfo.frameHeight, lspiFrameInfoOut.frameHeightOut, 0);
			lspiScaleInfo.colScaleFrac 		= (divVal==1)? 0 : (128/divVal);

			divVal 		= lspiInfo.frameWidth / lspiFrameInfoOut.frameWidthOut;
			divRemain	= lspiInfo.frameWidth % lspiFrameInfoOut.frameWidthOut;	
			lspiTailorInfo.tailorLeft		= divRemain/2;
			lspiTailorInfo.tailorRight		= divRemain/2;
			EC_ASSERT(divVal, lspiInfo.frameWidth, lspiFrameInfoOut.frameWidthOut, 0);
			lspiScaleInfo.rowScaleFrac = (divVal==1)? 0 : (128/divVal);
		}
		else
		{
			lspiScaleInfo.rowScaleFrac		= lcdIoCtrlParam.previewManulSet.rowScaleFrac;
			lspiScaleInfo.colScaleFrac		= lcdIoCtrlParam.previewManulSet.colScaleFrac;
			lspiTailorInfo.tailorLeft		= lcdIoCtrlParam.previewManulSet.tailorLeft;
			lspiTailorInfo.tailorRight		= lcdIoCtrlParam.previewManulSet.tailorRight;
			lspiTailorInfo0.tailorBottom    = lcdIoCtrlParam.previewManulSet.tailorBottom;
        	lspiTailorInfo0.tailorTop       = lcdIoCtrlParam.previewManulSet.tailorTop;
		}

        lcdDrv->ctrl(LSPI_CTRL_SCALE_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_TAILOR_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_FRAME_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_FRAME_INFO_OUT, 0);

		lspiCtrl.datSrc 				= 0; // 0: data from camera; 1: data from memory
		lspiCtrl.colorModeIn			= 0; // YUV422, every item is 8bit
		lspiCtrl.colorModeOut			= 1; // RGB565
		lcdDrv->ctrl(LSPI_CTRL_CTRL, 0);

		lspiCmdCtrl.wrRdn				= 1; // 1: wr	0: rd
		lspiCmdCtrl.ramWr				= 1; // start ramwr
		lspiCmdCtrl.dataLen 			= 0x3fffff; // infinite data, used in camera to lspi
		lcdDrv->ctrl(LSPI_CTRL_CMD_CTRL, 0);
    }
#else // CHIP 718
    if (previewStartStop)
    {
		st7789RegisterUspIrqCb(lcd, cbForCam);
		st7789UnRegisterUspIrqCb(lcd, cbForFill);

		lspiDataFmt.wordSize = 7;
		lspiDataFmt.txPack = 0;
		lcdDrv->ctrl(LSPI_CTRL_DATA_FORMAT, 0);

		lspiDmaCtrl.txDmaReqEn			= 0;
		lcdDrv->ctrl(LSPI_CTRL_DMA_CTRL, 0);

        lspiInfo.frameHeight            = PIC_SRC_HEIGHT;  	// frame input height
        lspiInfo.frameWidth             = PIC_SRC_WIDTH;   // frame input width
        lspiFrameInfoOut.frameHeightOut = previewHeight;   	// frame output height
        lspiFrameInfoOut.frameWidthOut  = previewWidth;    	// frame output width

		if ((previewHeight > PIC_SRC_HEIGHT) || (previewWidth > PIC_SRC_WIDTH))
		{
			EC_ASSERT(0,0,0,0);
		}

		if (lcdIoCtrlParam.previewModeSel == CAM_PREVIEW_SET_AUTO)
		{
			divVal 		= lspiInfo.frameHeight / lspiFrameInfoOut.frameHeightOut;
			divRemain	= lspiInfo.frameHeight % lspiFrameInfoOut.frameHeightOut;	
			lspiTailorInfo0.tailorTop		= divRemain/2;
			lspiTailorInfo0.tailorBottom	= divRemain/2;
			EC_ASSERT(divVal, lspiInfo.frameHeight, lspiFrameInfoOut.frameHeightOut, 0);
			lspiScaleInfo.colScaleFrac 		= (divVal==1)? 0 : (128/divVal);

			divVal 		= lspiInfo.frameWidth / lspiFrameInfoOut.frameWidthOut;
			divRemain	= lspiInfo.frameWidth % lspiFrameInfoOut.frameWidthOut;	
			lspiTailorInfo.tailorLeft		= divRemain/2;
			lspiTailorInfo.tailorRight		= divRemain/2;
			EC_ASSERT(divVal, lspiInfo.frameWidth, lspiFrameInfoOut.frameWidthOut, 0);
			lspiScaleInfo.rowScaleFrac = (divVal==1)? 0 : (128/divVal);
		}
		else
		{
			lspiScaleInfo.rowScaleFrac		= lcdIoCtrlParam.previewManulSet.rowScaleFrac;
			lspiScaleInfo.colScaleFrac		= lcdIoCtrlParam.previewManulSet.colScaleFrac;
			lspiTailorInfo.tailorLeft		= lcdIoCtrlParam.previewManulSet.tailorLeft;
			lspiTailorInfo.tailorRight		= lcdIoCtrlParam.previewManulSet.tailorRight;
			lspiTailorInfo0.tailorBottom    = lcdIoCtrlParam.previewManulSet.tailorBottom;
        	lspiTailorInfo0.tailorTop       = lcdIoCtrlParam.previewManulSet.tailorTop;
		}

        lcdDrv->ctrl(LSPI_CTRL_SCALE_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_TAILOR_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_FRAME_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_FRAME_INFO_OUT, 0);
        lcdDrv->ctrl(LSPI_CTRL_TAILOR_INFO0, 0);

		lspiCtrl.datSrc 				= 0; // 0: data from camera; 1: data from memory
		lspiCtrl.colorModeIn			= 0; // YUV422, every item is 8bit
		lspiCtrl.colorModeOut			= 1; // RGB565
		lcdDrv->ctrl(LSPI_CTRL_CTRL, 0);

		lspiCmdCtrl.wrRdn				= 1; // 1: wr	0: rd
		lspiCmdCtrl.ramWr				= 1; // start ramwr
		lspiCmdCtrl.dataLen 			= 0x3ffff; // infinite data, used in camera to lspi
		lcdDrv->ctrl(LSPI_CTRL_CMD_CTRL, 0);
    }
#endif    	
    else
    {
        lspiDmaCtrl.txDmaReqEn          = 0;
        lspiDataFmt.wordSize	        = 31;
        lspiDataFmt.txPack		        = 0;
        lspiInfo.frameHeight	        = lcd->height;  // frame input height
        lspiInfo.frameWidth		        = lcd->width;   // frame input width
        lspiFrameInfoOut.frameHeightOut = lcd->height;  // frame output height
        lspiFrameInfoOut.frameWidthOut 	= lcd->width;   // frame output width
        lspiCtrl.colorModeIn            = 3; // rgb565
        lspiCtrl.colorModeOut           = 1; // rgb565
        lspiCtrl.datSrc                 = 1; // 0: data from camera; 1: data from memory
        lspiScaleInfo.rowScaleFrac      = 0;
        lspiScaleInfo.colScaleFrac      = 0;
        lspiTailorInfo.tailorLeft       = 0; // frame output height
        lspiTailorInfo.tailorRight      = 0; // frame output width
        lcdDrv->ctrl(LSPI_CTRL_SCALE_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_TAILOR_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_CTRL, 0);
        lcdDrv->ctrl(LSPI_CTRL_DATA_FORMAT, 0);
        lcdDrv->ctrl(LSPI_CTRL_FRAME_INFO, 0);
        lcdDrv->ctrl(LSPI_CTRL_DMA_CTRL, 0);
        lcdDrv->ctrl(LSPI_CTRL_TAILOR_INFO0, 0);
        lcdDrv->ctrl(LSPI_CTRL_FRAME_INFO_OUT, 0);    

        // before send first cmd/data to lcd, clear unnessary status
        if (((LSPI2->STAS >> 27)& 0x1) > 0)
        {
            LSPI2->STAS |= 1<<27;
        }

        if (((LSPI2->STAS >> 28)& 0x1) > 0)
        {
            LSPI2->STAS |= 1<<28;
        }

#if ((defined CHIP_EC718) && !(defined TYPE_EC718M)) || (defined CHIP_EC716)
		if (((LSPI2->STAS >> 13)& 0x3f) > 0)
#else // 719
		if (((LSPI2->LSPI_STAT >> 8)& 0x3f) > 0)
#endif
        {
            LSPI2->DMACTL |= 1<<25;
        }
        
        LSPI2->DMACTL |= 1<<24; // clear rx fifo

        // write first dummy cmd to lcd. It's nessary to clear lspi fifo
        lcdWriteCmd(0xff);
        lcdWriteData(0x00);
        lcdDrv->send(NULL, 0);

#if ((defined CHIP_EC718) && !(defined TYPE_EC718M)) || (defined CHIP_EC716)
		if (((LSPI2->STAS >> 13)& 0x3f) > 0)
#else // 719
		if (((LSPI2->LSPI_STAT >> 8)& 0x3f) > 0)
#endif
        {
            LSPI2->DMACTL |= 1<<25;
        }
    }
}

static int st7789Close(lcdDrvFunc_t *lcd)
{
    return 0;
}

#ifdef FIBOCOM_BASE
lcdDrvFunc_t st7789Drv =
#else
AP_PLAT_COMMON_DATA lcdDrvFunc_t st7789Drv =
#endif
{
    .id                 = 0x7789,
    .width              = ST7789_WIDTH,
    .height             = ST7789_HEIGHT,
    .freq               = ST7789_FREQ,
    .bpp                = ST7789_BPP,
    .initRegTbl         = initTable7789,
    .initRegTblLen      = sizeof(initTable7789)/sizeof(initLine_t),
    .dir                = 0,

    .init               = st7789Init,
    .drawPoint          = st7789DrawPoint,
    .setWindow          = st7789AddrSet,
    .fill               = st7789Fill,
#ifndef FIBOCOM_BASE
    .backLight          = st7789BackLight,
#endif
    .startStop          = st7789StartStop,
    .startStopPreview   = st7789CamPreviewStartStop,
    .uspIrq4CamCb       = NULL,
    .uspIrq4FillCb      = NULL,
    .registerUspIrqCb   = st7789RegisterUspIrqCb,
    .unregisterUspIrqCb = st7789UnRegisterUspIrqCb,
    .direction          = NULL,
#ifdef FIBOCOM_BASE
    .dotSetFlushDir     = st7789Direction,
#endif
    .close              = st7789Close,
};

