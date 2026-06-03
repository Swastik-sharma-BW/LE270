#ifndef _OC_TTS_H_
#define _OC_TTS_H_

#include <stdbool.h>
#include <stdint.h>
#include "fibo_opencpu.h"

#define RETRY_NUM_MAX 10

bool fibo_tts_is_playing(void);
INT32 fibo_tts_stop(void);
INT32 fibo_tts_start(const UINT8 *pData, INT8 cEncode);
void fibo_set_oc_TTS_status(int flag_status);
int fibo_get_oc_TTS_status(void);
INT32 fibo_tts_voice_speed_get(void);
INT32 fibo_tts_voice_speed(INT32 speed);
INT32 fibo_tts_voice_pitch_get(void);
INT32 fibo_tts_voice_pitch(INT32 pitch);
INT32 fibo_tts_cfg(INT32 key, INT32 mode, INT32 *value);
INT32 fibo_tts_delay_play_init(UINT32 ms);
void fibo_tts_play_delay(void);



#endif
