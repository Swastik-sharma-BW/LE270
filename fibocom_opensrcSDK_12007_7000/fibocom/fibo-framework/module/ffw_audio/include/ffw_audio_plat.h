#ifndef BB2DE9E3_C9F9_4CB1_8990_FE699A97B348
#define BB2DE9E3_C9F9_4CB1_8990_FE699A97B348

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ffw_types.h"
#include "codecDrv.h"

typedef HalCodecType_e  CodecType_e;
typedef void (*ffw_playback_finish_t)(void *arg);

typedef enum
{
    /// pcm
    FFW_AUDIO_FMT_PCM = 1,

    /// wav
    FFW_AUDIO_FMT_WAV,

    /// mp3
    FFW_AUDIO_FMT_MP3,

    /// amr
    FFW_AUDIO_FMT_AMR,

    /// awb
    FFW_AUDIO_FMT_AWB,

    /// unknown
    FFW_AUDIO_FMT_UNKNOWN
} ffw_audio_format_t;

typedef enum
{
    /// voice
    FFW_AUDIO_VOL_VOICE = 1,

    /// tone
    FFW_AUDIO_VOL_TONE,

    /// voice and tone
    FFW_AUDIO_VOL_VOICE_TONE,

    /// MIDI
    FFW_AUDIO_VOL_MIDI,

    /// voice and midi
    FFW_AUDIO_VOL_VOICE_MIDI,

    /// tone and midi
    FFW_AUDIO_VOL_TONE_MIDI,

    /// vocie tone and midi
    FFW_AUDIO_VOL_VOICE_TONE_MIDI,

} ffw_audio_vol_type_t;

/**
* @brief 
* 
* @param player 
* @return int 
 */
int ffw_audio_player_plat_create(ffw_handle_t *player);

/**
* @brief 
* 
* @param player 
* @return int 
 */
int ffw_audio_player_plat_delete(ffw_handle_t player);

/**
* @brief 
* 
* @param player 
* @param fmt 
* @param filename 
* @return int 
 */
int ffw_audio_player_plat_file_start(ffw_handle_t player, ffw_audio_format_t fmt, const char *filename, ffw_playback_finish_t on_finish);

/**
* @brief 
* 
* @param player 
* @param fmt 
* @param data 
* @param size 
* @return int 
 */
int ffw_audio_player_plat_buffer_start(ffw_handle_t player, ffw_audio_format_t fmt, const void *data, uint32_t size, ffw_playback_finish_t on_finish);

/**
* @brief 
* 
* @param player 
* @return int 
 */
int ffw_audio_player_plat_pause(ffw_handle_t player);

/**
* @brief 
* 
* @param player 
* @return int 
 */
int ffw_audio_player_plat_resume(ffw_handle_t player);

/**
* @brief 
* 
* @param player 
* @return int 
 */
int ffw_audio_player_plat_stop(ffw_handle_t player);

/**
* @brief 
*  
* @return int 
 */
int ffw_audio_tone_start();

/**
* @brief 
*  
* @return int 
 */
int ffw_audio_tone_stop();
#endif /* BB2DE9E3_C9F9_4CB1_8990_FE699A97B348 */
