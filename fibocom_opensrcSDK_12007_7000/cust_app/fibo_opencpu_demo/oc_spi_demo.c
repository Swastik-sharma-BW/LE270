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

SPIHANDLE spiFd;
uint8_t FlashId[4] = {0};
uint8_t dummydata [4] = {0};  // JedecdeviceID+ 3 dummydata
SPI_IOC spi_ioc = {8, 1000000, SPI_I2C_CS0, SPI_CS_ACTIVE_LOW};

#define EC718_PLAT

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

static void hello_world_demo(void *param)
{
/*  spi1
    64--cs0 --func1
    62--miso --func1
    63--mosi --func1
    49--clk --func1
*/

/*  spi0
    66--cs0 --func1
    67--mosi --func1
    28--miso --func1
    29--clk --func1
*/
    APP_LOG_INFO("application thread enter, param 0x%x", param);
    set_app_ver(app_ver);

    dummydata[0] = 0x9f;
    drvSpiConfig_t cfg = {0};
    cfg.name = DRV_NAME_SPI1;
    cfg.inputEn = true;
    cfg.baud = 1000000;
    cfg.cpol = SPI_CPOL_LOW;
    cfg.cpha = SPI_CPHA_1Edge;
    cfg.input_sel = SPI_DI_1;
    cfg.transmode = SPI_DIRECT_POLLING;
    cfg.cs_polarity0 = SPI_CS_ACTIVE_LOW;
    cfg.framesize = 8;

    fibo_spi_open(cfg, &spiFd);
    //fibo_spi_xfer(spiFd, spi_ioc,dummydata, FlashId, 4);
    //fibo_spi_send(spiFd, spi_ioc, dummydata, 4);
    fibo_spi_send_rcv(spiFd, spi_ioc, dummydata, FlashId, 4);
    APP_LOG_INFO("testspi read id %02x, %02x, %02x, %02x", FlashId[0], FlashId[1], FlashId[2], FlashId[3]); 

    while(1)
    {
        fibo_taskSleep(1000);
        fibo_spi_send_rcv(spiFd, spi_ioc, dummydata, FlashId, 4);
        APP_LOG_INFO("testspi read id %02x, %02x, %02x, %02x", FlashId[0], FlashId[1], FlashId[2], FlashId[3]); 
    }

    
}

void * appimg_enter(void *param)
{
    APP_LOG_INFO("application image enter, param 0x%x", param);

    fibo_thread_create(hello_world_demo, (INT8*)"spi_custhread", 1024*4, NULL, OSI_PRIORITY_NORMAL);
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
