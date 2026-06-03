#ifndef A70B7324_5FF4_45C9_9AF1_57FDD07CE6FD
#define A70B7324_5FF4_45C9_9AF1_57FDD07CE6FD

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "ffw_buffer.h"
#include "ffw_audio_plat.h"
#include "ffw_types.h"
#include "hal_i2s.h"

typedef void (*AudioCallbackT)(void);
#ifndef CONFIG_FFW_AUDIO_SIGNAL_CHANNEL
#define FEATURE_SUBSYS_MEDIA_AUDIO_CHANNEL_1TO2_ENABLE
#endif

#define AUDIO_TX_TRANSFER_SIZE                 8000 ///< Maximum TX DMA transfer size each descriptor
#define AUDIO_RX_TRANSFER_SIZE                 8000 ///< Maximum RX DMA transfer size each descriptor

#define AUDIO_FILENAME_LEN                     128

extern const uint16_t    gVolumeList[] ;
extern volatile uint8_t  gCodecTx;
extern char g_audio_whether_execute_callback;
extern char g_audio_stop_list_flag;
extern char g_audio_play_list_flag;
extern char g_audio_file_dir[32];
extern uint32_t g_isrepeat;

#define WAV_BUFFER_COUNT            2
#define LIST_ID                     "LIST"
#define DATA_ID                     "data"

#define AMR_NB_HEAD                 "#!AMR\n"
#define AMR_WB_HEAD                 "#!AMR-WB\n"

typedef struct
{
    char     riffId[4];
    uint32_t riffSize;
    char     riffType[4];
    char     fmtId[4];
    uint32_t fmtSize;
    uint16_t fmtCompressionCode;
    uint16_t fmtChannels;
    uint32_t fmtSampleRate;
    uint32_t fmtBytesPerSec;
    uint16_t fmtBlockAlign;
    uint16_t fmtBitPerSample;
    char     listOrDataId[4];
    uint32_t listOrDataLength;
} WavHeadT;

typedef struct
{
    uint8_t        type;
    bool           increase;
    bool           dual;
    uint32_t       rate;
    uint32_t       formate;
    uint32_t       length;
    char           *buffer;
    uint8_t        *data;
    uint32_t       *Wp;
    AudioCallbackT callback;
} QueueAudioT;

/*
typedef enum
{
    ES8388,
    ES7148,
    ES7149,
    ES8311,
    TM8211,
    ES7113,
    ES7111,
    HAL_CODEC_SOFTCODEC
}CodecType_e;
*/

typedef enum
{
    AUDIO_TYPE_MP3     = 0,
    AUDIO_TYPE_WAV     = 1,
    AUDIO_TYPE_PCM     = 2,
    AUDIO_TYPE_TTS     = 3,
    AUDIO_TYPE_VOLUME  = 4,
    AUDIO_PLAY_AMR     = 5,
    AUDIO_PLAY_TONE_AMR= 6,
    AUDIO_TYPE_RECORD  = 7,
    AUDIO_TYPE_STREAM  = 8,
    AUDIO_TYPE_INVALID
} AudioTypeT;

typedef enum
{
    ACTION_VOLUME_PLUS_SHORT  = 0,
    ACTION_VOLUME_PLUS_LONG   = 1,
    ACTION_VOLUME_MINUS_SHORT = 2,
    ACTION_VOLUME_MINUS_LONG  = 3,
    ACTION_MENU_SHORT         = 4,
    ACTION_MENU_LONG          = 5,
    ACTION_INVALID
} ActionT;

typedef enum
{
    HAL_CODEC_ES8388    = 0,
    HAL_CODEC_NAU88C22  = 1,
    HAL_CODEC_ES7148    = 2,
	HAL_CODEC_ES7149    = 3,
    HAL_CODEC_ES8311    = 4,
    HAL_CODEC_TM8211    = 5,
} HalCodecT;

typedef enum
{
    CODEC_TX_IDLE    = 0,
    CODEC_TX_START   = 1,
    CODEC_TX_FINISH  = 2,
    CODEC_TX_INVALID = 3
} CodecTxT;

typedef uint8_t uint8;
//typedef uint32_t uint32;
typedef struct{
    uint8 master;       /*Set data transmission to master mode or slave mode*/
    uint8 tran_mode;    /*Data transmission mode*/
    uint8 sample;       /*Sampling rate*/
    uint8 width;        /*Word width*/
    uint8 channel_count; /*Reserve,Channel num*/
    uint8 out_path;     /*Reserve,Output path: spk/lineout/headset*/
    uint8 in_path;      /*Reserve,Iutput path: mic/headset mic*/
    uint32_t* reserve;    /*Reserved fields for future expansion */
}ffw_audio_param_cfg_t;


/**
* @brief 
* 
* @return int 
 */
int ffw_audio_init(void);

/**
* @brief 
* 
* @param format 
* @param filename 
* @param on_finish 
* @return int 
 */
int ffw_audio_play_file(ffw_audio_format_t format, char *filename, AudioCallbackT on_finish, void *arg);

/**
* @brief 
* 
* @param format 
* @param data 
* @param size 
* @param on_finish 
* @return int 
 */
int ffw_audio_play_buffer(ffw_audio_format_t format, uint8_t *data, uint32_t size, AudioCallbackT on_finish, void *arg);

/**
* @brief 
* 
* @return int 
 */
int ffw_audio_stop(void);

/**
* @brief 
* 
* @return int 
 */
int ffw_audio_pause(void);

/**
* @brief 
* 
* @return int 
 */
int ffw_audio_resume(void);

/**
* @brief 
* 
* @return int 
 */
int ffw_audio_list(void);

/**
* @brief 开始播放或者停止播放tone音
* 
* @param op 0 开始播放 1 停止播放
* @param tone_id 
* @param tone_duration 时间长度，单位毫秒
* @param mix_factor 音量
* @return int 
 */
int ffw_tone_play(int op, uint8_t tone_id, uint32_t tone_duration, uint8_t mix_factor);

/**
* @brief 
* 
* @param vol_type 
* @param vol 
* @return int 
 */
int ffw_set_audio_volume(ffw_audio_vol_type_t vol_type, uint8_t vol);

/**
* @brief 设置音量的增益
* 
* @param value 0-15
* @return int 
 */
int ffw_set_microphone_gain_value(uint8_t value);

/**
* @brief 获取音量的增益
* 
* @param value 
* @return int 
 */
int ffw_get_microphone_gain_value(uint8_t *value);


/**
* @brief 设置回声抑制，噪声抑制，频响曲线等声学回声消除调节参数
* 
* @param mode 
* @param path 
* @param ctrl 
* @param param 
* @param param_len 
* @return int 
 */
int ffw_set_acoustic_echo_canceler(uint8_t mode, uint8_t path, uint8_t ctrl, const char *param, uint16_t param_len);

/**
* @brief 
* 
* @param mode 
* @param path 
* @param ctrl 
* @param param 
* @return int 
 */
int ffw_get_acoustic_echo_canceler(uint8_t mode, uint8_t path, uint8_t ctrl, char *param);


//int fibo_audio_stream_play(uint8_t format, uint8_t *buff, uint32_t *Rp, uint32_t *Wp, uint32_t buffsize, int32_t timeout);

/**
* @brief 
* 
* @param filenames 
* @param num 
* @return int 
 */
int ffw_audio_list_play(const char **filenames, int32_t num);

/**
* @brief 
* 
* @param filename 
* @param num 
* @param b 
* @return int 
 */
int ffw_audio_merge_file(const char **filename, int num, ffw_buffer_t *b);

/**
 * @brief 
 * 
 * @param handle 
 * @param cb 
 * @param arg 
 * @return int 
 */
int ffw_audio_ts_create(ffw_handle_t *handle, void (*cb)(void *data, uint32_t len, uint32_t stream_type, void *arg), void *arg);

/**
 * @brief 
 * 
 * @param handle 
 * @param data 
 * @param len 
 * @return int 
 */
int ffw_audio_ts_decode(ffw_handle_t handle, uint8_t *data, uint32_t len);

/**
 * @brief 
 * 
 * @param handle 
 * @return int 
 */
int ffw_audio_ts_delete(ffw_handle_t handle);

/**
* @brief 
* 
* @param url 
* @param on_finish 
* @param arg 
* @return int 
 */
int ffw_audio_play_url(const char *url, ffw_playback_finish_t on_finish, void *arg);

int ffw_audio_tts_play(char *text, AudioCallbackT callback);

uint16_t ffw_audio_get_volume(void);
uint16_t ffw_audio_set_volume(uint16_t volume);
void ffw_audio_set_sample_channel(uint32_t rate, I2sChannelSel_e channel);
void ffw_audio_start_transfer_data(uint8_t *data, uint32_t length);
void ffw_audio_start_play_loop_data(uint8_t *data, uint32_t length);
void ffw_audio_stop_transfer_data(void);
void ffw_audio_play_mute(uint32_t len);
#ifdef CONFIG_FFW_FEATURE_MP3_DECODER_IN_RAM
PLAT_FM_RAMCODE void ffw_audio_volume_gain_algorithm(int16_t* srcbuf, uint32_t srctotalnum, uint16_t volscale);
#else
void ffw_audio_volume_gain_algorithm(int16_t* srcbuf, uint32_t srctotalnum, uint16_t volscale);
#endif
void ffw_audio_clean_queue(void);
uint8_t ffw_audio_getstopflag(void);
void ffw_audio_fix_gCodecTx(uint8_t status);

bool ffw_audio_isbusy(void);
bool ffw_audio_judge_file_name_exist_dir(char *file_dir, const char *Filenameffs);
ffw_audio_format_t ffw_audio_get_format(const char *fname);
uint8_t ffw_audio_set_pause_resumeflag(uint8_t data);
uint8_t ffw_audio_get_pause_resumeflag(void);
void ffw_audio_loop_start(void);
void ffw_audio_loop_stop(void);
void ffw_audio_play_mute1(uint32_t len);
void ffw_audio_CodecInit(CodecType_e halCodec);
void ffw_audio_record(char *path, AudioCallbackT callback, uint32_t format, uint32_t size, uint8_t *data, uint32_t *Wp);
void ffw_audio_set_record_stop(void);
int ffw_audio_save_stream_data(uint8_t *buff, uint32_t buffsize);
int ffw_audio_stream_ring_create(uint8_t format,uint8_t *buff, uint32_t *Rp, uint32_t *Wp, uint32_t buffsize, int32_t timeout);
int ffw_audio_stream_ring_create_v2(uint8_t format, uint32_t samplerate, uint8_t *buff, uint32_t *Rp, uint32_t *Wp, uint32_t buffsize, int32_t timeout, AudioCallbackT callback);
INT32 ffw_audio_stream_stop(void);
int ffw_audio_set_cfg(ffw_audio_param_cfg_t *cfg);
int ffw_audio_get_cfg(ffw_audio_param_cfg_t *cfg);
uint16_t ffw_audio_get_mic(void);
uint16_t ffw_audio_set_mic(uint16_t volume);
void ffw_audio_play_finish(void);

#endif /* A70B7324_5FF4_45C9_9AF1_57FDD07CE6FD */
