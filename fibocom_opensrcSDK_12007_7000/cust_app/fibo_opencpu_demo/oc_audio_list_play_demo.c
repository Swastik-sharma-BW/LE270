#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"

const char fname[][128] = {
    "/FFS/airply.mp3",
    "/FFS/1.mp3",
    "/FFS/airply.mp3",
    "/FFS/1.mp3",
};

static void hello_world_demo()
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        APP_LOG_INFO("Hello world %d", i);
        fibo_taskSleep(1000);
    }

    fibo_audio_set_codec(OC_ES8311);
    fibo_taskSleep(3000);
    fibo_audio_list_play( fname, 4);
    fibo_taskSleep(6000);

    fibo_audio_stop();
    
    fibo_taskSleep(1000);
    fibo_audio_list_play( fname, 4);
    while(1)
    fibo_taskSleep(3000);
    return ;
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    return app_init(hello_world_demo);
}
