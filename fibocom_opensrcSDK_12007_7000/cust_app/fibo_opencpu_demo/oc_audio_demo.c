#include "app_image.h"
#include "fibo_opencpu.h"
#include "app.h"
#include "app_pdp.h"

#if 1
static uint8_t *gbuff = NULL;
static void memory_destory(uint8_t **ptr)
{
    if ((ptr != NULL) && (*ptr != NULL))
    {
        free(*ptr);
        *ptr = NULL;
    }
}

static void Callplayend(void)
{
    memory_destory(&gbuff);
}

static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    switch (sig)
    {
        case GAPP_SIG_AUDIO_PLAY_ERROR:
        {
            APP_LOG_INFO("zzg audio play error");
        }
        break;
        default:
        {
            break;
        }
    }
    APP_LOG_INFO("test %d",sig);
}



static void at_res_callback(UINT8 *buf, UINT16 len)
{
    APP_LOG_INFO("FIBO <--%s", buf);
}

#endif

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};

static void hello_world_demo()
{
	//fibo_pmu_setlevel(FIBO_POWER_NORMAL_GPIO, FIBO_IOVOLT_3_20V);
	//fibo_pmu_setlevel(FIBO_POWER_AON_GPIO, FIBO_IOVOLT_3_20V);
	//fibo_lpg_switch(0);

    APP_LOG_INFO("application thread enter");
    set_app_ver(app_ver);
    for (int n = 0; n < 10; n++)
    {
        APP_LOG_INFO("app image hello world %d", n);
        fibo_taskSleep(500);
    }
    fibo_audio_set_codec(OC_ES8311);
    fibo_taskSleep(3000);

    
    fibo_set_volume(AUDIO_PLAY_VOLUME, 2);

#if 0
    const char fname[][128]={"/Y05.wav"};
    int32_t fd = 0;
    int32_t first_file_len = 0;
    int32_t bytes = 0;

    fd = fibo_file_open(fname[0], O_RDONLY);

    first_file_len = fibo_file_get_size(fname[0]);
    fibo_textTrace("first_file_len [%s %d] ", __FUNCTION__, first_file_len);

    gbuff = (uint8_t *)malloc(first_file_len);
    if (NULL == gbuff)
    {
        fibo_textTrace("[%s %d] malloc failed", __FUNCTION__, __LINE__);
        return ;
    }

    bytes = fibo_file_read(fd, gbuff, first_file_len);
    if (bytes != first_file_len)
    {
        fibo_textTrace("[%s %d] read %s error", __FUNCTION__, __LINE__, fname[0]);
        fibo_file_close(fd);
        memory_destory(&gbuff);
        return ;
    }

    fibo_file_close(fd);
    fibo_textTrace(" zzg fibo_get_volume(AUDIO_PLAY_VOLUME) [ %d] ", fibo_get_volume(AUDIO_PLAY_VOLUME));

    fibo_set_volume(AUDIO_PLAY_VOLUME, 5);

    fibo_audio_mem_start(2, gbuff, first_file_len, NULL); //播放音频3

#endif
    const char fname[][128]={"/FFS/airply.mp3"};
    int32_t fd = 0;
    int32_t first_file_len = 0;
    int32_t bytes = 0;

    //fibo_audio_list_play(fname, 1); //播放音频1

    fibo_audio_start_v2(1, (INT8 *)"/FFS/airply.mp3", NULL); //播放音频2
    
    fibo_taskSleep(5000);

    fd = fibo_file_open(fname[0], O_RDONLY);

    first_file_len = fibo_file_get_size(fname[0]);
    APP_LOG_INFO("first_file_len [%s %d] ", __FUNCTION__, first_file_len);

    gbuff = (uint8_t *)malloc(first_file_len);
    if (NULL == gbuff)
    {
        APP_LOG_INFO("[%s %d] malloc failed", __FUNCTION__, __LINE__);
        return ;
    }

    bytes = fibo_file_read(fd, gbuff, first_file_len);
    if (bytes != first_file_len)
    {
        APP_LOG_INFO("[%s %d] read %s error", __FUNCTION__, __LINE__, fname[0]);
        fibo_file_close(fd);
        memory_destory(&gbuff);
        return ;
    }

    fibo_file_close(fd);
    APP_LOG_INFO(" zzg fibo_get_volume(AUDIO_PLAY_VOLUME) [ %d] ", fibo_get_volume(AUDIO_PLAY_VOLUME));

    fibo_set_volume(AUDIO_PLAY_VOLUME, 7);

    fibo_audio_mem_start(3, gbuff, first_file_len, NULL); //播放音频3
    while(1)
    {
        fibo_taskSleep(1000);

        APP_LOG_INFO(" zzg fibo_get_audio_status() [ %d] ", fibo_get_audio_status());
        if(fibo_get_audio_status())
        {
            fibo_audio_mem_stop();
            break;
        }
        
    }
    fibo_taskSleep(9000);

    fibo_audio_mem_start(2, gbuff, first_file_len, Callplayend); //播放音频3

    
    fibo_taskSleep(50000);

#if 0
    fibo_audio_mem_start(4, gbuff, first_file_len, NULL); //播放音频3

    fibo_taskSleep(1000);
    fibo_audio_stop();
    fibo_taskSleep(5000);

    fibo_audio_mem_start(4, gbuff, first_file_len, NULL); //播放音频3

    fibo_taskSleep(1000);
    fibo_audio_pause();
    fibo_taskSleep(2000);
    fibo_audio_resume();

    //fibo_audio_stop();
    fibo_taskSleep(10000);

    fibo_audio_mem_start(4, gbuff, first_file_len, NULL); //播放音频3

    fibo_taskSleep(10000);

#endif
    
    
}


void *appimg_enter(void *param)
{
    APP_LOG_INFO("app image enter");
    fibo_thread_create(hello_world_demo, (INT8 *)"mythread", 10*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
    return;
}
