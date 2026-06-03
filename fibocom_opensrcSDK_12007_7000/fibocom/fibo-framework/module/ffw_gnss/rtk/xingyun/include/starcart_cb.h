#ifndef _STARCART_CB_H_
#define _STARCART_CB_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "starcart_rtk.h"

void starcart_outputSdkStatus(starcart_status status);

int starcart_getCorsData(char *corsData, int *corsDataLen, char *url);

void starcart_outputSdkNmeaData(nmea_data_t *nmeaData);
void starcart_outputUartNmeaData(const char *nmeaData);
void starcart_logPrint(const char *s, ...);
#ifdef __cplusplus
}
#endif
#endif