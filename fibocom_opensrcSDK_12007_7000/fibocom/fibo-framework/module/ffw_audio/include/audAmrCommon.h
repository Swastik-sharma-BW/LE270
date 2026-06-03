#ifndef __AMR_COMMON_H__
#define __AMR_COMMON_H__


#include <stdint.h>


#define AMR_CODEC_TYPE              HAL_VC_AMR_NB
//#define AMR_NB_HEAD                 "#!AMR\n"
//#define AMR_WB_HEAD                 "#!AMR-WB\n"
#define AMR_FT_COUNT                16
#define AMR_BIT_OFFSET              0
#define PCM_BIT_WIDTH               16


extern const uint8_t    gAmrNbHead[];
extern const uint8_t    gAmrNbBitLength[];
extern const uint8_t    gAmrNbByteLength[];
extern uint32_t         gTimeBegin;
extern volatile uint8_t gRecordState;


int32_t  amrFifoInit(void);
bool     amrFifoIsFull(void);
bool     amrFifoIsEmpty(void);
int32_t  amrFifoWrite(uint8_t *data, uint32_t length);
int32_t  amrFifoRead(uint8_t *buffer, uint32_t size);
void     amrEngCallback(UINT32 msgId, void *msg);
void     amrEngWaitCpReply(uint32_t msgId);

#endif
