#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"

static uint8_t *gbuff = NULL;
static void memory_destory(uint8_t **ptr)
{
    if ((ptr != NULL) && (*ptr != NULL))
    {
        free(*ptr);
        *ptr = NULL;
    }
}

static void audio_playend(void)
{
    APP_LOG_INFO("zzg audio_play end !!!");
    fibo_audio_stream_stop();

}

static void hello_world_demo()
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        APP_LOG_INFO("Hello world %d", i);
        fibo_taskSleep(100);
    }
    fibo_audio_set_codec(OC_ES8311);
    fibo_taskSleep(2000);
    
    //const char fname[][128]={"/FFS/fixoverflow.pcm"};
    const char fname[][128]={"/FFS/wavtest.wav"};
    int32_t fd = 0;
    int32_t first_file_len = 0;
    int32_t bytes = 0;

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


    fibo_audio_stream_start_v2(1, 8000, NULL, NULL, NULL, 0, 300, audio_playend);//pcm

    for(i = 0; i < 3; i++) //test
    {
        fibo_audio_set_data(gbuff, first_file_len);
        fibo_taskSleep(3000);
    }

    for(i = 0; i < 3; i++) //test
    {
        fibo_audio_set_data(gbuff, first_file_len);
        fibo_taskSleep(5000);
    }

    //fibo_audio_stream_stop();

    fibo_taskSleep(20000);

    return ;
}


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

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    fibo_thread_create(hello_world_demo, (INT8 *)"apptask2", 10 * 1024, NULL, 24);
    
    return (void *)&user_callback;
}

void appimg_exit(void)
{

    APP_LOG_INFO("application image exit");
}

