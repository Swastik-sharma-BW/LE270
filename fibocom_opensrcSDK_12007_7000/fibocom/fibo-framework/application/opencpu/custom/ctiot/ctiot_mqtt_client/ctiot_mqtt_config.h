/*************************************************************************

            (c) Copyright 2019 by 天翼物联科技有限公司. All rights reserved.

**************************************************************************/
#ifndef __CTIOT_MQTT_CONFIG_H__
#define __CTIOT_MQTT_CONFIG_H__

//**********************************************

#ifndef MQTT_SENDBUF_SIZE
#define MQTT_SENDBUF_SIZE             (1024 * 2)
#endif

#ifndef MQTT_READBUF_SIZE
#define MQTT_READBUF_SIZE             (1024 * 2)
#endif

#ifndef MQTT_COMMAND_TIMEOUT_MS
#define MQTT_COMMAND_TIMEOUT_MS       (10000)
#endif

#ifndef MQTT_KEEPALIVE_INTERVAL_S
#define MQTT_KEEPALIVE_INTERVAL_S     (100)
#endif

#ifndef MAX_MSG_COUNT
#define MAX_MSG_COUNT                 (26)
#endif

#ifndef CTIOT_TRANSPRANT_DN_TOPIC
#define CTIOT_TRANSPRANT_DN_TOPIC     "device_control"
#endif

#ifndef DATA_MODE
#define DATA_MODE                     (2)
#endif

/*#ifndef CTIOT_SYNC_SEND
#define CTIOT_SYNC_SEND
#endif*/



//**************************************
#endif
