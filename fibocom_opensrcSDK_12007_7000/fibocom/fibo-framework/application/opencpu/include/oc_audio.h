#ifndef FFW_AUD_OC_AUDIO
#define FFW_AUD_OC_AUDIO

#include <stdint.h>
#include <stdbool.h>
#include "fibo_opencpu_comm.h"


typedef void (*auPlayerCallback_t)(void);
typedef char  auAmrnbMode_t;

typedef enum
{
    OC_ES8388,
    OC_ES7148,
    OC_ES7149,
    OC_ES8311,
    OC_TM8211,
    OC_ES7113,
    OC_ES7111,
    OC_PWMSOFT,
    OC_AW88266,
    OC_TM8311,
    OC_HAL_CODEC_SOFTCODEC
}fibo_code_type_t;

/* for external pa cfg */
typedef struct
{
    void (*_init)(void);
    void (*_open)(void);
    void (*_close)(void);
    INT32 pin_id;
    INT32 mode;
}paSpec_t;

/* for external codec cfg */
typedef struct
{
    bool codeci2c_en; //i2c使能 赋值0,1
    void (*codec_init)(void); //给带i2c芯片预留的初始化配置函数
    void (*codec_output)(void); //给带i2c芯片预留的音频输出配置函数
    void (*codec_input)(void); //给带i2c芯片预留的音频输入配置函数
    void (*codec_close)(void); //给带i2c芯片预留的关闭配置函数
    INT32 codecen_pinid; //codec使能管脚 0:不需要配置 非0使能管脚pin  必须配置
    UINT8 codecen_mode; //codecen_pinid 拉高使能或拉低使能 1 拉高使能,2拉低使能
    UINT8 i2s_sel; //i2s 序列号1,2
    bool i2sin_en; //sdati_pi 使能配置 :0 不使用i2s输入脚  1:使用ii2s输入脚
}codec_Spec_t;


typedef enum
{
    AUDIO_VOICE_VOLUME,
    AUDIO_PLAY_VOLUME,
    AUDIO_TONE_VOLUME,
} AUDIO_VOLUME_MODE_T;

typedef struct{
    UINT8 master;       /*Set data transmission to master mode or slave mode (just support 0:master mode)*/
    UINT8 tran_mode;    /*Data transmission mode (just support 1:PCM mode)*/
    UINT8 sample;       /*Sampling rate (just support 0:8k 1:16k)*/
    UINT8 width;        /*Word width (just support 1:16bit)*/
    UINT8 channel_count; /*Reserve,Channel num*/
    UINT8 out_path;     /*Reserve,Output path: spk/lineout/headset*/
    UINT8 in_path;      /*Reserve,Iutput path: mic/headset mic*/
    UINT32* reserve;    /*Reserved fields for future expansion */
}f_audio_param_cfg_t;


typedef void (*auPlayerCallback_t)(void);
extern const uint16_t     gVolumeList[] ;

INT32 fibo_audio_start_v2(INT32 type,INT8 *filepath,auPlayerCallback_t cb_ctx);

bool fibo_audio_status_get(void);

bool fibo_get_audio_status(void);

INT32 fibo_audio_play(INT32 type,INT8 *filename);
INT32 fibo_audio_stop(void);
INT32 fibo_audio_pause(void);
INT32 fibo_audio_resume(void);

INT32 fibo_audio_mem_start(UINT8 format, UINT8 *buff, UINT32 size, auPlayerCallback_t cb_ctx);
INT32 fibo_audio_mem_stop(void);
INT32 fibo_set_volume(AUDIO_VOLUME_MODE_T mode, uint8_t level);
INT32 fibo_get_volume(AUDIO_VOLUME_MODE_T mode);
INT32 fibo_audio_set_codec(fibo_code_type_t codec);
INT32 fibo_external_pa_cfg(paSpec_t *cfg);
INT32 fibo_extern_codec_cfg(codec_Spec_t *cfg);
INT32 fibo_audio_recorder_stream_start(uint8_t format, uint8_t *audio_buff,  uint32_t *Wp, uint32_t buffsize, auAmrnbMode_t amr_mode);
INT32 fibo_audio_recorder_stream_stop(void);
INT32 fibo_get_volume_external(AUDIO_VOLUME_MODE_T mode);
INT32 fibo_set_volume_external(AUDIO_VOLUME_MODE_T mode,uint8_t level);
INT32 fibo_audio_list_play(const char fname[][128], int32_t file_num);
INT32 fibo_audio_set_data(void * data, uint32_t size);
INT32 fibo_audio_stream_start(uint8_t format,uint8_t *buff, uint32_t *Rp, uint32_t *Wp, uint32_t buffsize, int32_t timeout);
INT32 fibo_audio_stream_stop(void);
INT32 fibo_audio_stream_start_v2(uint8_t format, uint32_t samplerate, uint8_t *buff, uint32_t *Rp, uint32_t *Wp, uint32_t buffsize, int32_t timeout, auPlayerCallback_t cb_ctx);
int32 fibo_audio_set_cfg(f_audio_param_cfg_t *cfg);
int32 fibo_audio_get_cfg(f_audio_param_cfg_t *cfg);
int32 fibo_set_microphone_gain_value(uint8_t value);
int32 fibo_get_microphone_gain_value(uint8_t *value);

#endif /* FFW_AUD_OC_AUDIO */
