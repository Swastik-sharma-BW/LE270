#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"


void voice_pa_init(void)
{
    fibo_gpio_mode_set(101,0);
    fibo_gpio_cfg(101,1);
    fibo_gpio_set(101,0);
}
void voice_pa_open(void)
{
    fibo_gpio_set(101,1);
    fibo_taskSleep(20);
}
void voice_pa_close(void)
{
    fibo_gpio_set(101,0);
    fibo_taskSleep(20);
}

static void tts_thread_entry(void *param)
{
    fibo_textTrace("application thread enter, param 0x%x", param);

    paSpec_t cfg = {NULL};
    cfg._init = voice_pa_init;
    cfg._open = voice_pa_open;
    cfg._close = voice_pa_close;
    
    //cfg._init = NULL;
    //cfg._open = NULL;
    //cfg._close = NULL;
    cfg.mode = 1;
    cfg.pin_id = 101;
    fibo_external_pa_cfg(&cfg);//pa 配置

    fibo_audio_set_codec(OC_ES8311);

    char *pUtf8 = "E8BF99E9878CE698AF545453E6B58BE8AF95"; /*UTF8 This is the TTS test*/
    char *pGb2312 = "D5E2C0EFCAC7545453B2E2CAD4"; /*GB2312 This is the TTS test*/
    char *pUnicode = "D98FCC912F665400540053004B6DD58B"; /* UNICODE This is the TTS test , UTF16*/
    int value = 0;

    //fibo_taskSleep(5000);
    fibo_tts_cfg(0,0,(INT32 *)&value);
    fibo_textTrace("TTS UTF8 play get1 %d", value);
    value = -30000;
    fibo_tts_cfg(0,1,(INT32 *)&value);

    if(GAPP_TTS_SUCCESS == fibo_tts_start((const UINT8 *)pUtf8, CTTS_STRING_ENCODE_TYPE_UTF8))
    {
        fibo_textTrace("TTS UTF8 play success");
    }
    else
    {
        fibo_textTrace("TTS UTF8 play error");
    }

    while(fibo_tts_is_playing())
    {
        fibo_textTrace("TTS wait play!");
        fibo_taskSleep(1000);
    }
    fibo_taskSleep(1000);

    if(GAPP_TTS_SUCCESS == fibo_tts_start((const UINT8 *)pGb2312, CTTS_STRING_ENCODE_TYPE_GB2312))
    {
        fibo_textTrace("TTS GB2312 play success");
    }
    else
    {
        fibo_textTrace("TTS GB2312 play error");
    }

    while(fibo_tts_is_playing())
    {
        fibo_textTrace("TTS wait play!");
        fibo_taskSleep(1000);
    }
    fibo_taskSleep(1000);

    if(GAPP_TTS_SUCCESS == fibo_tts_start((const UINT8 *)pUnicode, CTTS_STRING_ENCODE_TYPE_UNICODE))
    {
        fibo_textTrace("TTS UNICODE play success");
    }
    else
    {
        fibo_textTrace("TTS UNICODE play error");
    }
    fibo_taskSleep(1000);
    while(fibo_tts_is_playing())
    {
        fibo_textTrace("TTS wait play!");
        fibo_taskSleep(1000);
    }
    fibo_taskSleep(1000);
    fibo_audio_start_v2(1, (INT8 *)"/FFS/airply.mp3", NULL); //播放音频2
    fibo_taskSleep(10000);

    
}


static void hello_world_demo()
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        APP_LOG_INFO("Hello world %d", i);
        fibo_taskSleep(100);
    }
    tts_thread_entry(NULL);

    return ;
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    return app_init(hello_world_demo);
}
