/*************************************************************************

            (c) Copyright 2019 by ??????????. All rights reserved.

**************************************************************************/
#ifndef _CTIOT_MQTT_SDK_H
#define _CTIOT_MQTT_SDK_H


#include <stdbool.h>
#include <stdint.h>

#include "ctiot_mqtt_config.h"

#define USERNAME_MAX_LEN 220
#define USERNAME_HEADER "tywl_"

#define MODULE_MAX_LEN 20
//MODULE 针对CPE设备，该字段建议用户设置为"厂商名缩写_产品型号"
#define MODULE "CTN168"

#define CHIPTYPE_MAX_LEN 20
//CHIPTYPE 芯片类型，建议用户根据实际情况修改
#define CHIPTYPE "EC618E"

#define SOFTVERSION_MAX_LEN 40
//SOFTVERSION 软件版本，该字段请使用默认值，用户不要修改
//#define SOFTVERSION "V3.0.0TYWL2210000000000000" //非定制模组
#define SOFTVERSION_CTIOT "V3.0.0TYWL2110000000000000"   //定制模组，用户可根据实际情况修改


#define IMEI_MAX_LEN 20
//Username 中携带的 IMEI 建议用户根据实际情况修改，可在使用该宏定义的位置使用实际的IMEI进行替换
#define UN_IMEI "12345678901234567890"

#define IMSI_MAX_LEN 20
//Username 中携带的 IMSI 建议用户根据实际情况修改，可在使用该宏定义的位置使用实际的IMSI进行替换
#define UN_IMSI "12345678901234567890"

#define SIGNAL_MAX_LENGTH 20
//Username 中携带的信号参数，建议用户根据实际情况从设备中读取
#define UN_RSRP ""
#define UN_SINR ""
#define UN_TXPOWER ""
#define UN_CELLID ""

#define UN_ICCID_MAX_LEN 20
//Username 中携带的 ICCID 建议用户根据实际情况修改，可在使用该宏定义的位置使用实际的ICCID进行替换
#define UN_ICCID "89860310707694224144"

#ifdef __cplusplus
extern "C"
{
#endif


#define STRING_MAX_LEN 256
#define IS_VALID_NAME_LEN(name) (strlen((name)) <= STRING_MAX_LEN)

//*************************************************
//
//! @} error code
//
//*************************************************

typedef enum
{
	CTIOT_SUCCESS             = 0,
	CTIOT_PARA_ERROR          = -1,
	CTIOT_BUF_OVERFLOW        = -2,
	CTIOT_DECODE_ERROR        = -3,
	CTIOT_ENCODE_ERROR        = -4,
	
	CTIOT_LOGIN_TIMEOUT       = -20,
	
	CTIOT_QUEUE_FULL          = -30,
	CTIOT_SEND_ERROR          = -31,
	
	CTIOT_SES_NOK_NOTINITIAL  = -60,
	CTIOT_SES_NOK_NOTLOGIN    = -61,
	CTIOT_SES_NOK_LOGINING    = -62,
	CTIOT_SES_NOK_LOGINED     = -63,
	CTIOT_SES_NOK_LOGOUTING   = -64,
	
	CTIOT_MQTT_CONNECT_ERROR            = -80,
	CTIOT_MQTT_UNNACCEPTABLE_PROTOCOL   = -81,
	CTIOT_MQTT_CLIENTID_REJECTED        = -82,
	CTIOT_MQTT_SERVER_UNAVAILABLE       = -83,
	CTIOT_MQTT_BAD_USERNAME_OR_PASSWORD = -84,
	CTIOT_MQTT_NOT_AUTHORIZED           = -85,
	CTIOT_MQTT_DISCONNECT_ERROR         = -86,
	CTIOT_MQTT_INIT_ERROR               = -87,
	CTIOT_MQTT_SUB_ERROR                = -88,
	CTIOT_MQTT_UNSUB_ERROR              = -89,
	
	CTIOT_MUTEX_INIT_ERROR             = -180,
	CTIOT_MUTEX_LOCK_ERROR             = -181,
	CTIOT_MUTEX_UNLOCK_ERROR           = -182,
	CTIOT_THREAD_CREATE_ERROR          = -183,
	
	CTIOT_NET_INIT_ERROR               = -191,
	CTIOT_NET_CONNECT_ERROR            = -192,
	
	CTIOT_TLS_ROOT_CERTIFICATE_ERROR   = -193,
	CTIOT_TLS_CLIENT_CERTIFICATE_ERROR = -194,
	CTIOT_TLS_CONFIG_DEFAULTS_ERROR    = -195,
	CTIOT_TLS_CONF_OWN_CERT_ERROR      = -196,
	CTIOT_TLS_SETUP_ERROR              = -197,
	CTIOT_TLS_SET_HOSTNAME_ERROR       = -198,
	CTIOT_TLS_HANDSHAKE_ERROR          = -199,
	CTIOT_TLS_OTHER_ERROR              = -200,
	
	CTIOT_MALLOC_ERROR                 = -220,
	
} CTIOT_RESULT_CODE;


typedef enum
{
	CTIOT_ASYNC_MSG_MQTT_ERROR         = 1,
	CTIOT_ASYNC_MSG_MQTT_TIMEOUT_ERROR = 2,
} CTIOT_ASYN_MSG_STATUS;

typedef enum
{
	CTIOT_EVENT_CLIENT_NOTLOGIN = 1,
} CTIOT_CLIENT_EVENT;

typedef enum
{
	CTIOT_CONNECT_MODE_CLEAR = 0,
	CTIOT_CONNECT_MODE_TLS   = 1,
} CTIOT_CONNECT_MODE;


typedef enum
{
	MQTT_QOS_MOST_ONCE,  //MQTT QOS 0
	MQTT_QOS_LEAST_ONCE, //MQTT QOS 1
	MQTT_QOS_ONLY_ONCE,  //MQTT QOS 2
	MQTT_QOS_MAX
} mqtt_qos_e;


typedef enum
{
	CTIOT_MQTT_REPORT_CNT       = 0,
	CTIOT_MQTT_RESP_CNT         = 1,
	CTIOT_MQTT_RECV_CNT         = 2,
	CTIOT_MQTT_REPORT_ERROR_CNT = 3,
} CTIOT_MQTT_CNT_TYPE;

typedef enum
{
	CTIOT_CLIENT_NOT_INITIAL = 0x01,
	CTIOT_CLIENT_NOT_LOGIN   = 0x02,
	CTIOT_CLIENT_LOGINING    = 0x04,
	CTIOT_CLIENT_LOGINED     = 0x08,
	CTIOT_CLIENT_LOGOUTING   = 0x10,
 } CTIOT_CLIENT_STATUS;


//*************************************************
//
//! @} struct
//
//*************************************************

typedef void (*CTIOT_MQTT_MSG_STATUS_API)(uint32_t, int8_t);
typedef void (*CTIOT_MQTT_EVENT_CALLBACK)(int8_t);

typedef struct
{
	uint16_t msgID;
} ctiot_mqtt_msg_s;

typedef struct
{
	char *serverIp;
	uint16_t serverPort;
	char *password;
	char *deviceId;
	uint32_t connect_mode;
	
	CTIOT_MQTT_MSG_STATUS_API msgStatusCallback;
	CTIOT_MQTT_EVENT_CALLBACK eventStatusCallback;
} mqtt_param_s;

struct mqtt_client_tag_s
{
	mqtt_param_s params;
	uint8_t mqttLoginStatus;
	uint16_t MessageId;
	
	void *downDataCallback;
	CTIOT_MQTT_MSG_STATUS_API msgStatusCallback;
	CTIOT_MQTT_EVENT_CALLBACK eventStatusCallback;
};

typedef struct mqtt_client_tag_s mqtt_client_s;

#if DATA_MODE == 0
	typedef struct
	{
		void (*trans_callback)(char *);
	} ctiot_srv_cb_s;
#endif

//*************************************************
//
//! @addtogroup 公共API接口
//!
//! @{
//
//*************************************************

//**************************************************
//
//! @brief ctiot_mqtt参数初始化
//!
//! @param mqtt_param_s 初始化结构体
//! @param mqtt_client_s 返回初始化的mqtt_client
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_init(mqtt_param_s *params, void *downCallback);

//**************************************************
//
//! @brief ctiot_mqtt登录
//!
//! @param mqtt_info_s 设备信息结构体
//! @param mqtt_client_s mqtt_client句柄
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_login(uint32_t timeout);

//**************************************************
//
//! @brief ctiot_mqtt登出
//!
//! @param mqtt_client_s mqtt_client句柄
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_logout(void);

//**************************************************
//
//! @brief 数据上报功能
//!
//! @param serviceID 服务标志
//! @param qos 传输质量
//! @param payload 数据
//!
//! @retval  CTIOT_MSG_STATUS 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_data(const char *topic, mqtt_qos_e qos, char *payload, ctiot_mqtt_msg_s *msg);

//**************************************************
//
//! @brief  获取当前sdk状态
//!
//! @retval  CTIOT_CLIENT_STATUS 状态码
//
//**************************************************

CTIOT_CLIENT_STATUS ctiot_mqtt_get_status(void);



#ifdef __cplusplus
}
#endif

#endif //_CTIOT_MQTT_SDK_H
