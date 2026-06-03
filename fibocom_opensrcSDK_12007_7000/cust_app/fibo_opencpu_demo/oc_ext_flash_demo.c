#include "app_image.h"
#include "fibo_opencpu.h"

/*****************************************************************************
*
* Note:
*   App receives fibocom events callback
*
*****************************************************************************/
static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    fibo_textTrace("sig_res_callback  sig = %d (200 is opencpu test.)", sig);
}

/*****************************************************************************
*
* Note:
*   fibo_send_at callback
*
*****************************************************************************/
static void at_res_callback(UINT8 *buf, UINT16 len)
{
    fibo_textTrace("FIBO <--%s", buf);
}

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};


static void fibo_ext_flash_demo()
{
    fibo_textTrace("ext_flash_thread enter");
#if 0
    //LE230 flash spi
    fibo_gpio_mode_set(1,1);
    fibo_gpio_mode_set(2,1);
    fibo_gpio_mode_set(3,1);
    fibo_gpio_mode_set(4,1);
    //LE250 flash spi
    fibo_gpio_mode_set(82,1);
    fibo_gpio_mode_set(83,1);
    fibo_gpio_mode_set(84,1);
    fibo_gpio_mode_set(85,1);
#endif
    //LE270 flash spi
    fibo_gpio_mode_set(28,1);
    fibo_gpio_mode_set(29,1);
    fibo_gpio_mode_set(66,1);
    fibo_gpio_mode_set(67,1);
    uint8_t Pinsel = 0;
    UINT32 pulId,pulCapacity;
    UINT32 faddr = 0x100000;
    unsigned char *writeBuf = (unsigned char *)malloc(10 * sizeof(unsigned char));
    unsigned char *readBuf = (unsigned char *)malloc(10 * sizeof(unsigned char));
    memcpy(writeBuf,"test!",sizeof("test!"));
    memset(readBuf,0,10);
    fibo_textTrace("extern flash writeBuf context:%s,len:%d",writeBuf,sizeof(writeBuf));
    fibo_ext_flash_init(Pinsel);
    fibo_ext_flash_info(&pulId, &pulCapacity);
    fibo_textTrace("extern flash pulId:0x%x,pulCapacity:0x%x",pulId,pulCapacity);
    fibo_ext_flash_write(faddr, writeBuf, sizeof(writeBuf));
    fibo_ext_flash_read(faddr, readBuf, sizeof(writeBuf));
    fibo_textTrace("extern flash read:%s.", readBuf);
    fibo_taskSleep(1000);
    fibo_ext_flash_erase(faddr, sizeof(writeBuf));
    if(0 == fibo_ffsmountExtflash(0,1024*1024,"/ext",Pinsel,false,false))
    {
        fibo_textTrace("extern flash mount ffs success!");
    }

    
}


void *appimg_enter(void *param)
{
    fibo_textTrace("app image enter");
    fibo_thread_create(fibo_ext_flash_demo, (INT8 *)"ext_flash_thread", 10*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    fibo_textTrace("application image exit");
    return;
}