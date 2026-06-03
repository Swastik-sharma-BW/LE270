#ifndef __AMR_AUDIO_H__
#define __AMR_AUDIO_H__


#include <stdint.h>


int32_t amrPlay(UINT8 type, char *path, uint8_t *data, uint32_t size);
int32_t amrPlayStop(void);


#endif
