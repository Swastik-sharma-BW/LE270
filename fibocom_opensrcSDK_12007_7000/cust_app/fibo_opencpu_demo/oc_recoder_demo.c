#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"
//fibo_audio_recorder_stream_start  只支持录制wav和pcm格式
static void hello_world_demo()
{
    fibo_taskSleep(1000);
    fibo_audio_set_codec(OC_ES8311);
    fibo_taskSleep(1000);

//保存录音到内存audio_buff
#if 1
    uint8_t *audio_buff;
    int len = 40960;
    audio_buff = (uint8_t *)fibo_malloc(len);
    if (audio_buff == NULL)
    {
        APP_LOG_INFO("[%s-%d]malloc failed!", __FUNCTION__, __LINE__);
        return ;
    }
    memset(audio_buff, 0, len);
    
    APP_LOG_INFO("zzg audio_buff %x, %d", audio_buff, len);
    fibo_audio_recorder_stream_start(2, audio_buff,  NULL, len, 0);//wav录音,空间耗完了会自动结束录音
    while(1)
    {
        fibo_taskSleep(10);
        if(fibo_get_audio_status() == false)
        {
            break;
        }
    }
    fibo_taskSleep(1000);

    fibo_audio_mem_start(2, audio_buff, len, NULL); //播放音频
#endif

#if 0
    uint8_t *audio_buff;
    int len = 40960;
    uint32_t wp = 0;
    audio_buff = (uint8_t *)fibo_malloc(len);
    if (audio_buff == NULL)
    {
        APP_LOG_INFO("[%s-%d]malloc failed!", __FUNCTION__, __LINE__);
        return ;
    }
    memset(audio_buff, 0, len);
    
    APP_LOG_INFO("zzg audio_buff %x, %d", audio_buff, wp);
    fibo_audio_recorder_stream_start(2, audio_buff,  &wp, len, 0);//wav录音,wp记录偏移的大小


    fibo_taskSleep(2000);

    fibo_audio_recorder_stream_stop();
    fibo_taskSleep(2000);

    APP_LOG_INFO("zzg audio_buff %x, %d", audio_buff, wp);
    fibo_audio_mem_start(2, audio_buff, wp, NULL); //播放音频
#endif

//保存录音到文件
#if 0
    fibo_file_delete("/audio1.wav");
    int32_t fd= fibo_file_open("/audio1.wav", O_RDWR | O_CREAT | O_TRUNC);//创建文件,查看at+fslstfile="/"
    if (fd < 0)
    {
        fibo_textTrace("create file failed");
        return ;
    }
    fibo_file_close(fd);
    fibo_audio_recorder_stream_start(2, NULL,(uint32_t *)"/audio1.wav", 0, 0);//wav录音
    fibo_taskSleep(5000);
    fibo_audio_recorder_stream_stop();

    fibo_audio_start_v2(1, (INT8 *)"/audio1.wav", NULL); //播放音频/audio1.wav
#endif

    while(1)
    {
        fibo_taskSleep(2000);
    }
    return ;
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    return app_init(hello_world_demo);
}
