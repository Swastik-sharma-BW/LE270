#include "lcdDev_7567.h"
#include "lcdComm.h"
#include "slpman.h"
#include "oc_gpio.h"
#include DEBUG_LOG_HEADER_FILE
#define LCD_DRV_ID      0X55//DOT_LCD_ID_UNSUPPORT

#define LCD_BUFF_SIZE   (LCD_WIDTH) * ((LCD_HEIGHT) / 8)

#define LCD_PAGE        8
#define LCD_LINE        64

#define INST_regRes_MASK            0x20
#define INST_EVReg_MASK             0x00
#define INST_bias_MASK              0xa2
#define INST_STARTLINE_MASK         0x40

#define INST_regRes_VALID_BITS      0x07
#define INST_EVReg_VALID_BITS       0x3F
#define INST_bias_VALID_BITS        0x01//0x07
#define INST_STARTLINE_VALID_BITS   0x3F

//  (0~7): Select Regulator Resister
#define INST_regRes(x)    (INST_regRes_MASK | ((x) & INST_regRes_VALID_BITS))

//  (0~63): Set Electronic Volume Register
#define INST_EVReg(x)     (INST_EVReg_MASK | ((x) & INST_EVReg_VALID_BITS))

// (0~7): 0-1/5 1-1/6 2-1/7 3-1/8 4-1/9 5-1/10 6-1/11 7-1/12
#define INST_bias(x)      (INST_bias_MASK | ((x) & INST_bias_VALID_BITS)) 

#define PAGE_LINE_NUM   8

#define WHICH_PAGE(y)               ((y) / PAGE_LINE_NUM)
#define WHICH_BIT_IN_PAGE(y)        ((y) % PAGE_LINE_NUM)
#define WHICH_COLUMN(x)             (x)

#define LCD_DataWrite_ST(Data)                 \
    {                                          \
        lcdWriteData(Data);                     \
        lspiDrv7567->send(NULL, 0);            \
    }
#define LCD_CtrlWrite_ST(Cmd)                   \
    {                                           \
        lcdWriteCmd(Cmd);                       \
        lspiDrv7567->send(NULL, 0);             \
    }

static uint8_t _g_inst_regRes = 0x04;     //  (0~7): Select Regulator Resister
static uint8_t _g_inst_EVReg = 0x20;      //  (0~63): Set Electronic Volume Register
static uint8_t _g_inst_bias = 0x00;
static bool _g_lcd_isActive = false;
static void _lcdDelayMs(int ms_delay)
{
    lcdDrvDelay(ms_delay);
}

void st7567CleanScreen(void);
void st7567BeforeDisplayOn(void);
extern lspiDrvInterface_t lspiDrvInterface2;
lspiDrvInterface_t *lspiDrv7567 = &lspiDrvInterface2; // We use lspi2 by default

DmaDescriptor_t __ALIGNED(16) lcdDmaTxDesc7567[HEIGHT_7567];
int8_t lcdDmaTxCh7567; // dma tx channel
DmaTransferConfig_t lcdDmaTxCfg7567 =
{
    NULL,
    (void *)&(LSPI2->TFIFO),
    DMA_FLOW_CONTROL_TARGET,
    DMA_ADDRESS_INCREMENT_SOURCE,
    DMA_DATA_WIDTH_ONE_BYTE,
    DMA_BURST_8_BYTES,
    WIDTH_7567 * 2
};

AP_PLAT_COMMON_DATA static  initLine_t initTable7567[] =
{

};

void st7567CamPreviewStartStop(lcdDrvFunc_t *lcd, camPreviewStartStop_e previewStartStop)
{

}

void st7567Reginit(lcdDrvFunc_t *lcd)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, st7567Reginit, P_INFO, "st7567Reginit");
    st7567BeforeDisplayOn();
    st7567CleanScreen();
    LCD_CtrlWrite_ST(0xAF); // Display ON

    lspiCmdCtrl.wrRdn = 1;
    lspiCmdCtrl.ramWr = 1;
    lspiCmdCtrl.dataLen = 0x3ffff;
    lspiDrv7567->ctrl(LSPI_CTRL_CMD_CTRL, 0);

    lcdDrvDelay(50); //Delay 100ms

    _g_lcd_isActive = true;
}

static void st7567GpioInit(lcdDrvFunc_t *lcd)
{
    //LCD_RST
    fibo_gpio_mode_set(LCD_RST_PIN, LCD_RST_PIN_GPIO_MODE);
    fibo_gpio_cfg(LCD_RST_PIN, 1);

    //LCD_BACK_LIGHT
    fibo_gpio_mode_set(LCD_BACK_LIGHT_PIN, LCD_BACK_LIGHT_PIN_MODE);
    fibo_gpio_cfg(LCD_BACK_LIGHT_PIN, 1);
    fibo_gpio_set(LCD_BACK_LIGHT_PIN, 1);

    //LCD_PIN
    fibo_gpio_mode_set(LCD_CLK_PIN, LCD_CLK_PIN_MODE);
    fibo_gpio_mode_set(LCD_CS_PIN, LCD_CS_PIN_MODE);
    fibo_gpio_mode_set(LCD_TE_PIN, LCD_TE_PIN_MODE);
    fibo_gpio_mode_set(LCD_SIO_PIN, LCD_SIO_PIN_MODE);
    fibo_gpio_mode_set(LCD_SDC_PIN, LCD_SDC_PIN_MODE);
}

static int st7567Init(lcdDrvFunc_t *lcd, void* uspCb, void *dmaCb, uint32_t freq, uint8_t bpp)
{
    st7567GpioInit(lcd);
    lcdRst(250, 250);
    lcdDrvDelay(20);
    dmaInit(dmaCb);
    lspiDefaultCfg(lcd, uspCb, freq, bpp);
    lcdInterfaceType(LCD_INTERFACE);

    lspiDataFmt.wordSize = 7;
    lspiDataFmt.txPack = 2;
    lspiDrv7567->ctrl(LSPI_CTRL_DATA_FORMAT, 0);

    lspiQuartileCtrl.grayCtrl   = 2;

    lspiGrayPageCmd0.pageCmd = 0xb0; // start page 0, page cmd should be fixed with 0xb0+page_num
    lspiGrayPageCmd0.pageCmd0 = 0x10; // command followed with page_cmd. column_num high bits
    lspiGrayPageCmd0.pageCmd01ByteNum = 2; // page_cmd0+page_cmd1   byte num

    lspiGrayPageCmd1.pageCmd1 = 0; // followed with page_cmd0. column_num low bits

    lspiDrv7567->ctrl(LSPI_CTRL_GRAY_PAGE_CMD0, 0);
    lspiDrv7567->ctrl(LSPI_CTRL_GRAY_PAGE_CMD1, 0);
    lspiDrv7567->ctrl(LSPI_CTRL_QUARTILE_CTRL, 0);

    lspiInfo.frameHeight = LCD_HEIGHT;//69; // frame input height
    lspiInfo.frameWidth = LCD_WIDTH;//92; // frame input width
    lspiDrv7567->ctrl(LSPI_CTRL_FRAME_INFO, 0);

    lspiFrameInfoOut.frameHeightOut = LCD_HEIGHT; // frame output height
    lspiFrameInfoOut.frameWidthOut = LCD_WIDTH;//92; // frame output width
    lspiDrv7567->ctrl(LSPI_CTRL_FRAME_INFO_OUT, 0);

    st7567Reginit(lcd);

    return 0;
}

static int st7567Fill(lcdDrvFunc_t *lcd, uint32_t fillLen, uint8_t *buf)
{
    return 0;
}

#if 0
static void stPrepareDisplay(lcdDrvFunc_t * lcd, uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey)
{
 //   return 0;
}

static int st7567OnOff(lcdDrvFunc_t * lcd, uint8_t sta)
{
    return 0;
}

static void st7567BackLight(lcdDrvFunc_t * lcd, uint8_t sta)
{
}

#endif
void st7567BuildDmaChain(lcdDrvFunc_t * lcd, uint8_t* dataBuf, const uint16_t chainCnt,
                                bool needStop)
{


}

void st7567StartStop(lcdDrvFunc_t *lcd, bool startOrStop)
{
    if (startOrStop)
    {
        DMA_loadChannelDescriptorAndRun(DMA_INSTANCE_MP, lcdDmaTxCh7567, lcdDmaTxDesc7567);
    }
    else
    {
        extern void DMA_stopChannelNoWait(DmaInstance_e instance, uint32_t channel);
        DMA_stopChannelNoWait(DMA_INSTANCE_MP, lcdDmaTxCh7567);
    }

}

uint8_t check321 = 0;
void st7567Clear(lcdDrvFunc_t* lcd, uint8_t* buf, uint16_t lcdHeight, uint16_t lcdWidth, uint32_t dmaTrunkLength)
{
    uint32_t bk1=0,bk2=0,bk3=0,bk4=0,bk5=0,bk6=0, bk7=0,bk8, bk9, bk10;

    bk1 = LSPI2->LSPI_CTRL;
    bk2 = LSPI2->LSPFINFO;
    bk3 = LSPI2->LSPFINFO0;
    bk4 = LSPI2->LSPIGPCMD0;
    bk5 = LSPI2->LSPIGPCMD1;
    bk6 = LSPI2->DFMT;
    bk7 = CSPI1->CSPIPROCLSPI;
    bk8 = LSPI2->LSPIQUARTCTRL;
    bk9 = LSPI2->LSPTINFO;
    bk10 = LSPI2->LSPTINFO0;

    CSPI1->CSPIPROCLSPI = 0;
    LSPI2->DFMT         = 7<<6 | 2<<18;
    LSPI2->LSPI_CTRL    = 1 | 1<<2 | 1<<3 | 1<<4 | 4<<6 | 1<<29; // 7789: 3<<4 | 1<<6;   7567: 1<<4 | 4<<6
    LSPI2->LSPFINFO     = 64<<0 | 128<<16;
    LSPI2->LSPFINFO0	= 64<<0 | 128<<10;
    
    LSPI2->LSPIGPCMD0   = 0<<0 | 0xb<<4 | 0x0010<<8 | 2<<28;
    LSPI2->LSPIGPCMD1   = 0x0000;
    LSPI2->LSPTINFO0 = 0;
    LSPI2->LSPTINFO = 0;
    LSPI2->LSPIQUARTCTRL = 0;

    
    //st7567Line(0xff, 0x00);
    int  i;

    for (int j=0; j<64; j++)
    {
        for (i=0; i<64; i++)
        {
            //lcdWriteData(0xff);
            //lcdWriteData(0xff);
            lspiDrv7567->prepareSend(0xff);
            lspiDrv7567->prepareSend(0xff);

            check321++;
            
            if (check321 == 2)
            {
                #if 0
                // txfifo
                tmp = ((LSPI2->STAS) >> 13) & 0x3f;
                //while (tmp > 15);
                while (!LSPI2->LSPI_STAT);
                LSPI2->TFIFO = dataList[0]; 
                check321 = 0;
                dataListIndex = 0;
                dataList[0]= 0;
                #endif
                lspiDrv7567->send(NULL, 0);
            }
        }
        
        delay_us(100*1000);

      //  st7567Line(0x00, 0xff);
        //delay_us(600*1000);
    }

    LSPI2->LSPI_CTRL = bk1;
    LSPI2->LSPFINFO  = bk2;
    LSPI2->LSPFINFO0 = bk3;
    LSPI2->LSPIGPCMD0 = bk4;
    LSPI2->LSPIGPCMD1 = bk5;
    LSPI2->DFMT         = bk6;
    CSPI1->CSPIPROCLSPI = bk7;
    LSPI2->LSPTINFO0 = bk10;
    LSPI2->LSPTINFO = bk9;
    LSPI2->LSPIQUARTCTRL = bk8;
}

uint32_t st7567ReadId(lcdDrvFunc_t *lcd)
{
    uint16_t ret_id = 0x7567;

    //lspiReadReg(0xFC, (uint8_t*)&ret_id, 2, 0);
    return ret_id;
}

//doMatrixLcd api 
uint8_t st7567_lcdbuf[(LCD_WIDTH) * ((LCD_HEIGHT) / 8)];

static uint8_t _dotMatrixLcdSetSeg(uint8_t segVal, uint8_t bit, uint8_t val)
{
    if (val == 0) {
        return segVal & (~(0x01 << bit));
    } else {
        return segVal | (0x01 << bit);
    }
}

static void _dotMatrixLcdSetAddr(uint8_t column, uint8_t page)
{
    LCD_CtrlWrite_ST(0xb0 + page);
    LCD_CtrlWrite_ST(((column >> 4) & 0x0f) + 0x10);
    LCD_CtrlWrite_ST(column & 0x0f);
}

static void _dotMatrixLcdCleanScreen()
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, st7567CleanScreen, P_ERROR, "LCD_WIDTH %d",LCD_WIDTH);
    for (int j = 0; j < LCD_PAGE; j++) {
        _dotMatrixLcdSetAddr(0, j);
        for (int i = 0; i < LCD_WIDTH; i++) {
            lcdWriteData(0x00);
        }
        lspiDrv7567->send(NULL, 0);
    }
}

void st7567CleanScreen(void)
{
    _dotMatrixLcdCleanScreen();
}

static void _lcdInitCmdBeforeDisplayOn(void)
{
    LCD_CtrlWrite_ST(0xA4); //Cancel All Pixel ON
    LCD_CtrlWrite_ST(0xAE); //display off
    LCD_CtrlWrite_ST(0xE2); //SoftwareReset
    LCD_CtrlWrite_ST(0xA0); //Select SEG Normal Direction
    LCD_CtrlWrite_ST(0xC0); //Select COM Normal Direction
    LCD_CtrlWrite_ST(0x24); //Select Regulation Ratio=5.0
    LCD_CtrlWrite_ST(0x2C); //Booster ON
    LCD_CtrlWrite_ST(0x2E); //Regulator ON
    LCD_CtrlWrite_ST(0x2F); //Follower ON

    // LCD_CtrlWrite_ST(0x24); //Select Regulation Ratio=5.0
    // LCD_CtrlWrite_ST(0x81); //Set EV Command
    // LCD_CtrlWrite_ST(0x20); //Set EV=32
    // LCD_CtrlWrite_ST(0xA2); //Select 1/9Bias

    LCD_CtrlWrite_ST(INST_regRes(_g_inst_regRes)); // Select regulator register(1+(Ra+Rb))
    LCD_CtrlWrite_ST(0x81); // Set Reference Voltage 
    LCD_CtrlWrite_ST(INST_EVReg(_g_inst_EVReg)); // EV=35 => Vop =10.556V
    LCD_CtrlWrite_ST(INST_bias(_g_inst_bias)); // Set LCD Bias=1/9 V0
}

void st7567BeforeDisplayOn(void)
{
    _lcdInitCmdBeforeDisplayOn();
}

#if 0
static uint8_t _dotMatrixLcdReadModifyWrite(uint16_t column, uint16_t page, uint8_t bit, uint8_t val)
{
    HAL_ERR_T r_err;
    uint8_t rData[2] = {0};

    LCD_CtrlWrite_ST(0xE0);
    _dotMatrixLcdSetAddr(column, page);

    lspiReadReg(0x04,rData,2,0);

    uint8_t oldSegVal = rData[1];
    uint8_t newSegVal = _dotMatrixLcdSetSeg(oldSegVal, bit, val);
    LCD_DataWrite_ST(newSegVal);
    LCD_CtrlWrite_ST(0xEE);
    return 0;
}
#endif

static uint8_t _dotMatrixLcdReadSegFromBuf(uint16_t column, uint16_t page)
{
    return (st7567_lcdbuf[page * (LCD_WIDTH) + column]);
}

static void _dotMatrixLcdSetSegToBuf(uint16_t column, uint16_t page, uint8_t segVal)
{
    st7567_lcdbuf[page * (LCD_WIDTH) + column] = segVal;
}

static void  st7567SetPixel(lcdDrvFunc_t *lcd, uint16_t x, uint16_t y, uint32_t dataWrite)
{
    uint8_t val = dataWrite & 0xff;

    ECPLAT_PRINTF(UNILOG_PLA_APP, st7567SetPixel, P_ERROR, "app_test val x %d y %d dataWrite %d",x,y,val);

    if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) {
     //   FFW_LOG_INFO("coordinate out of range!");
      //  return -1;
    }
    uint8_t col = WHICH_COLUMN(x);
    uint8_t page = WHICH_PAGE(y);
    uint8_t bit = WHICH_BIT_IN_PAGE(y);

    uint8_t oldSegVal = _dotMatrixLcdReadSegFromBuf(col, page);
    uint8_t newSegVal = _dotMatrixLcdSetSeg(oldSegVal, bit, val);

    _dotMatrixLcdSetAddr(col, page);
    LCD_DataWrite_ST(newSegVal);
    _dotMatrixLcdSetSegToBuf(col, page, newSegVal);
   // return 0;
}

static void st7567SetDispalyParam(lcdDrvFunc_t *lcd, uint8_t contrastRatio_coarse, uint8_t contrastRatio_fine, uint8_t bias)
{
    _g_inst_regRes = contrastRatio_coarse;
    _g_inst_EVReg = contrastRatio_fine;
    _g_inst_bias = bias;

    if (_g_lcd_isActive) {
        LCD_CtrlWrite_ST(INST_regRes(_g_inst_regRes)); // Select regulator register(1+(Ra+Rb))

        LCD_CtrlWrite_ST(0x81); // Set Reference Voltage 
        LCD_CtrlWrite_ST(INST_EVReg(_g_inst_EVReg)); // EV=35 => Vop =10.556V
        
        LCD_CtrlWrite_ST(INST_bias(_g_inst_bias)); // Set LCD Bias=1/9 V0
    }
}

static void st7567SendBuff(lcdDrvFunc_t *lcd, uint8_t column, uint8_t row, void *buff, uint32_t size)
{
    if ((column + size) > LCD_WIDTH) {
        return;
    }
    _dotMatrixLcdSetAddr(column, row);
    for (uint32_t i = 0; i < size; i++) {
        lcdWriteData(((uint8_t *)buff)[i]);
    }
    lspiDrv7567->send(NULL, 0); 

    memcpy(&st7567_lcdbuf[row * (LCD_WIDTH) + column], buff, size);
}

typedef enum {
    DOTLCD_DIRECT_NORMAL = 0,// SEG0 -> SEG131  COM0 -> COM63
    DOTLCD_DIRECT_MY,   // COM63 -> COM0
    DOTLCD_DIRECT_MX,   // SEG131 -> SEG0
    DOTLCD_DIRECT_MY_MX,// SEG131 -> SEG0 COM63 -> COM0
} dotlcd_direct_t;

static void st7567SetFlushDirection(lcdDrvFunc_t *lcd, uint8_t direction)
{
    // LCD_CtrlWrite_ST(0xf8);         // set window disable
    _lcdDelayMs(100);

    switch (direction) {
        case DOTLCD_DIRECT_NORMAL: 
        {
            LCD_CtrlWrite_ST(0xa0);
            LCD_CtrlWrite_ST(0xC0);
        }
        break;

        case DOTLCD_DIRECT_MY: 
        {
            LCD_CtrlWrite_ST(0xa0);
            LCD_CtrlWrite_ST(0xC8);
        }
        break;

        case DOTLCD_DIRECT_MX: 
        {
            LCD_CtrlWrite_ST(0xa1);
            LCD_CtrlWrite_ST(0xC0);
        }
        break;

        case DOTLCD_DIRECT_MY_MX: 
        {
            LCD_CtrlWrite_ST(0xa1);
            LCD_CtrlWrite_ST(0xC8);
        }
        break;

        default:
        {
            LCD_CtrlWrite_ST(0xa0);
            LCD_CtrlWrite_ST(0xC0);
        }
        break;
    }
}


lcdDrvFunc_t st7567Drv = 
{
    .id                 = 0x7567,
    .width              = LCD_WIDTH,
    .height             = LCD_HEIGHT,
    .freq               = LCD_FREQ,
    .bpp                = LCD_BPP_USE,
    .initRegTbl         = initTable7567,    //todo...
    .initRegTblLen      = sizeof(initTable7567)/sizeof(initLine_t),
    .dir                = 0,
    .init               = st7567Init,
    .fill               = st7567Fill,
    .startStop          = st7567StartStop,
    .startStopPreview = st7567CamPreviewStartStop,
    .readId         = st7567ReadId,
    .drawPoint     = st7567SetPixel,
    .dotSendBuff    = st7567SendBuff,
    .dotSetDisaplayParam   = st7567SetDispalyParam,
    .dotSetFlushDir   = st7567SetFlushDirection,
};

