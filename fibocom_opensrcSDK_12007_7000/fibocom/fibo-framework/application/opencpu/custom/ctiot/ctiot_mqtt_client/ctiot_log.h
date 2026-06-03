#ifndef CTIOT_LOG_H
#define CTIOT_LOG_H

#include <stdio.h>
#include "abstract_os.h"
#define SDK_PAHO_MQTT 1
#define CTIOT_DEBUG 1

typedef enum
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERR,
    LOG_FATAL,

    LOG_MAX
} ctiot_log_e;

void ctiot_set_log_level(ctiot_log_e level);

ctiot_log_e ctiot_get_log_level(void);

#ifdef CTIOT_DEBUG
const char* ctiot_get_log_level_name(ctiot_log_e log_level);

#define CTIOT_LOG(level, fmt, ...) \
    do \
    { \
        if ((level) >= ctiot_get_log_level()) \
        { \
            (void)os_log("[%s][%s][%s:%d] " fmt "\r\n", \
            ctiot_get_log_level_name((level)), __TIME__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        } \
    } while (0)
#else
#define CTIOT_LOG(level, fmt, ...)
#endif


#endif

