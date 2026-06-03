#ifndef __AMR_RECORD_H__
#define __AMR_RECORD_H__


#include <stdint.h>


int32_t amrRecord(char *path, RecordParamT *recordParam);
int32_t amrRecordStop(void);


#endif
