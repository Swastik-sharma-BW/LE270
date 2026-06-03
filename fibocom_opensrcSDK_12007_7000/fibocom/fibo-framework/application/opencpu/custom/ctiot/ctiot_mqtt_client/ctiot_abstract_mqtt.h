/*************************************************************************

            (c) Copyright 2019 by ??????????. All rights reserved.

**************************************************************************/
#ifndef _CTIOT_ABSTRACT_MQTT_H_
#define _CTIOT_ABSTRACT_MQTT_H_


#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif


#define MAX_MESSAGE_HANDLERS 20



typedef struct
{
	int len;
	char* data;
} MAL_len_string;

typedef struct
{
	char* cstring;
	MAL_len_string lenstring;
} MAL_sring;

typedef struct
{
	unsigned char qos;
	unsigned char retained;
	unsigned char dup;
	unsigned short id;
	void *payload;
	size_t payloadlen;
} MAL_message;

typedef struct
{
	MAL_message* message;
	MAL_sring* topicName;
} MAL_message_data;


typedef void (*MAL_message_handler)(MAL_message_data*);

typedef struct MAL_message_Handlers
{
	const char* topicFilter;
	MAL_message_handler fp;
} MAL_message_Handlers_t;


typedef struct MAL_will_options
{
	char * topicName;
	char * message;
	unsigned char retained;
	unsigned char qos;
} MAL_will_options;

typedef struct MAL_connect_params
{
	char *serverIP;
	unsigned short port;
	
	unsigned short keepAlive;
	unsigned char cleanSession;
	unsigned char willFlag;
	MAL_will_options willOptions;
	char * clientID;
	char * username;
	char * password;
} MAL_connect_params;

/************************************************************/

int MAL_network_init(int net_mode);
int MAL_network_connect(char *serverIP, int port, int timeout);

int MAL_mqtt_config(MAL_connect_params *params);
int MAL_mqtt_connect(void);
int MAL_mqtt_disconnect(void);
int MAL_mqtt_publish(const char *topic, MAL_message *msg);
int MAL_mqtt_subscribe(const char* topic, int qos, MAL_message_handler msgHandler);
int MAL_mqtt_unsubscribe(const char *topic);
int MAL_set_message_handler(const char* topic, MAL_message_handler msgHandler);
int MAL_mqtt_loop(int timeout);



#ifdef __cplusplus
}
#endif

#endif //_CTIOT_ABSTRACT_MQTT_H_
