#ifndef  _LCD_COMM_H
#define  _LCD_COMM_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include "ec7xx.h"
#include "bsp.h"
#include "lspi.h"

#ifdef FIBOCOM_BASE
#include "ffw_lcd.h"
#define SPI_3W_I                (0)
#define SPI_3W_II               (1)
#define SPI_4W_I                (2)
#define SPI_4W_II               (3)
#define MSPI_4W_II              (4)
#define INTERFACE_8080          (5)

#define LCD_INTERFACE          (SPI_3W_I)

//LE270
#define LCD_RST_PIN             49
#define LCD_RST_PIN_GPIO_MODE   0

#define LCD_CLK_PIN             53
#define LCD_CLK_PIN_MODE        1

#define LCD_CS_PIN              52
#define LCD_CS_PIN_MODE         1

#define LCD_SIO_PIN             50
#define LCD_SIO_PIN_MODE        1

#define LCD_SDC_PIN             51
#define LCD_SDC_PIN_MODE        2

#define LCD_TE_PIN              78
#define LCD_TE_PIN_MODE         1
#endif

#define DMA_BULK_NUM                (1023*8)
#define DMA_DESC_MAX                (160) // RGB888 man need big desc chain

void lcdWriteCmd(uint8_t cmd);
void lcdWriteData(uint8_t data);
void lcdInterfaceType(uint8_t type);
void lspiCmdSend(uint8_t cmd, uint8_t *data, uint8_t num);
void lspiReadReg(uint8_t addr,uint8_t *data,uint16_t num, uint8_t dummyCycleLen);
void lspiReadRam(uint32_t *data,uint32_t num);
void lcdDrvDelay(uint32_t ms);
void lspiFifoWrite(uint32_t data);
#ifdef FIBOCOM_BASE
void lspiCmdSendByteData(uint8_t cmd, uint8_t data);
#endif

#if (BK_USE_PWM == 1)
uint32_t millis(void);
uint8_t  lcdPwmBkLevel(uint8_t level);
#endif

#if (BK_USE_GPIO == 1)
void     lcdGpioBkLevel(uint8_t level);
#endif

typedef void (*lcdDmaCb)(uint32_t event);
typedef void (*lcdUspCb)();

int     dmaInit(lcdDmaCb cb);
void    lcdRst(uint32_t highUs, uint32_t lowUs);
void    dmaStartStop(bool start);
int     lspiDefaultCfg(lcdDrvFunc_t *lcd, lcdUspCb cb, uint32_t freq, uint8_t bpp);
int     lcdDmaTrans(lcdDrvFunc_t *lcd, void *sourceAddress, uint32_t totalLength);
void    lcdMspiSet(uint8_t enable, uint8_t addrLane, uint8_t dataLane, uint8_t instruction);
void    lcdMspiHsyncSet(uint8_t hsyncAddr, uint8_t hsyncInst, uint16_t vbpNum, uint16_t vfpNum);
void    lcdMspiVsyncSet(uint8_t vsyncEnable, uint8_t vsyncInst, uint8_t lspiDiv);
void    lcdCsnHighCycleMin(uint8_t lspiDiv);
void    lcdDmaLoop(lcdDrvFunc_t *lcd, void *sourceAddress, uint32_t totalLength, uint32_t dmaTrunkLen);

#ifdef __cplusplus
}
#endif
#endif /* LCDCOMM_H */

