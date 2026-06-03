#ifndef A0E214BC_1F33_4A35_8AC6_A62D1B43907A
#define A0E214BC_1F33_4A35_8AC6_A62D1B43907A

#include <stdint.h>
#if 0

#include "ffw_audio_plat.h"

int fibo_audio_start_v2(ffw_audio_format_t fmt, const char *filepath, ffw_playback_finish_t on_finish);

int fibo_audio_resume(void);

int fibo_audio_pause(void);

int fibo_audio_mem_start(uint8_t fmt, uint8_t *buff, uint32_t size, ffw_playback_finish_t on_finish);

int fibo_audio_stream_start(uint8_t fmt);

int fibo_audio_stream_data(uint8_t *data, uint32_t size);

int fibo_audio_stop(void);

int fibo_audio_list_play(const char *fnames[], int32_t num);
#endif

#endif /* A0E214BC_1F33_4A35_8AC6_A62D1B43907A */
