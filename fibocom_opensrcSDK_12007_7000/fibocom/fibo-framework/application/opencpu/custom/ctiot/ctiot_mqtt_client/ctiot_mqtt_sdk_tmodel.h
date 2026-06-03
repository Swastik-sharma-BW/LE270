/*************************************************************************

            (c) Copyright 2019 by 天翼物联科技有限公司. All rights reserved.

*************************************************************************/
#ifndef _CTIOT_MQTT_SDK_TMODEL_H
#define _CTIOT_MQTT_SDK_TMODEL_H

#ifdef __cplusplus
{
#endif

#include "ctiot_mqtt_sdk.h"

#if DATA_MODE > 0

#define MAX_PARA_COUNT 8

//*************************************************
//
//! @addtogroup CTIOT_MQTT_PARA的结构体
//!
//! @{
//
//*************************************************
typedef enum{
	CTIOT_INT,
	CTIOT_DOUBLE,
	CTIOT_FLOAT,
	CTIOT_STR,
	CTIOT_ENUM,
	CTIOT_BOOL,
	CTIOT_DATE,
}CTIOT_PARA_TYPE;
typedef struct
{
	int32_t max;
	int32_t min;
	int32_t val;
} CTIOT_INT_ITEM;
typedef struct
{
	double max;
	double min;
	double val;
} CTIOT_DOUBLE_ITEM;
typedef struct
{
	float max;
	float min;
	float val;
} CTIOT_FLOAT_ITEM;
typedef struct
{
	uint16_t maxLen;
	uint16_t minLen;
	char *val;
} CTIOT_STR_ITEM;
typedef struct
{
	int32_t maxLen;
	int32_t minLen;
int32_t val;
} CTIOT_ENUM_ITEM;
typedef struct
{
	int32_t max;
	int32_t min;
	int32_t val;
} CTIOT_BOOL_ITEM;
typedef struct
{
	uint64_t max;
	uint64_t min;
	uint64_t val;
} CTIOT_DATE_ITEM;
typedef struct
{
	CTIOT_PARA_TYPE ctiotParaType;
	char *paraName;
	union {
		CTIOT_STR_ITEM ctiotStr;
		CTIOT_FLOAT_ITEM ctiotFloat;
		CTIOT_DOUBLE_ITEM ctiotDouble;
		CTIOT_DATE_ITEM ctiotDate;
		CTIOT_INT_ITEM ctiotInt;
		CTIOT_BOOL_ITEM ctiotBool;
		CTIOT_ENUM_ITEM ctiotEnum;
	} u;
} CTIOT_PARAM_ITEMS;
typedef struct ctiot_mqtt_para
{
	uint32_t count;
	CTIOT_PARAM_ITEMS paraList[MAX_PARA_COUNT+1];
}CTIOT_MQTT_PARA;

//*************************************************
//
//! @addtogroup service为Sim卡断复网通知指令下发的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	uint32_t taskId;	//!< taskId
	char* ICCID;	//!< ICCID
	int32_t sim_net_status;	//!< Sim卡网络状态
}sim_net_notice_act_s;

//*************************************************
//
//! @addtogroup service为业务数据下发响应的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	mqtt_qos_e qos;	//!< QOS等级
	uint32_t taskId;	//!< taskId
	int32_t act_result;	//!< 指令执行结果
	char* act_msg;	//!< 指令返回信息
}rda_act_data_resp_s;

//*************************************************
//
//! @addtogroup service为Sim卡断复网通知下发响应的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	mqtt_qos_e qos;	//!< QOS等级
	uint32_t taskId;	//!< taskId
	int32_t act_result;	//!< 指令执行结果
	char* act_msg;	//!< 指令返回信息
}sim_net_notice_resp_s;

//*************************************************
//
//! @addtogroup service为上下电告警上报的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	mqtt_qos_e qos;	//!< QOS等级
	int32_t power_onoff_alarm;	//!< 上下电告警状态
	uint64_t time;	//!< 当前时间
}rda_power_onoff_alarm_report_s;

//*************************************************
//
//! @addtogroup service为业务数据下发的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	uint32_t taskId;	//!< taskId
	char* act_data;	//!< 指令下发内容
}rda_act_data_cmd_s;

//*************************************************
//
//! @addtogroup service为定位信息上报的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	mqtt_qos_e qos;	//!< QOS等级
	double longitude;	//!< 经度
	double latitude;	//!< 纬度
	float altitude;	//!< 海拔
	int32_t coordinate_system;	//!< 坐标系统
	float pos_speed;	//!< 定位当前速度
	uint64_t time;	//!< 当前时间
}rda_position_report_s;

//*************************************************
//
//! @addtogroup service为电池低电告警的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	mqtt_qos_e qos;	//!< QOS等级
	int32_t battery_state;	//!< 电池状态
	float battery_voltage;	//!< 电池电压
	int32_t battery_value;	//!< 电池电量
}rda_battery_low_alarm_s;

//*************************************************
//
//! @addtogroup service为信号数据上报的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	mqtt_qos_e qos;	//!< QOS等级
	int32_t rsrp;	//!< 参考信号接收功率
	int32_t sinr;	//!< 信号与干扰加噪声比
	int32_t pci;	//!< 物理小区标识
	int32_t ecl;	//!< 无线信号覆盖等级
	int32_t cell_id;	//!< 小区位置信息
}signal_report_s;

//*************************************************
//
//! @addtogroup service为电池数据上报的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	mqtt_qos_e qos;	//!< QOS等级
	float battery_voltage;	//!< 电池电压
	int32_t battery_value;	//!< 电池电量
	uint64_t time;	//!< 当前时间
}rda_data_report_s;

//*************************************************
//
//! @addtogroup service为设备信息上报的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
	mqtt_qos_e qos;	//!< QOS等级
	char* manufacturer_name;	//!< 终端厂家名称
	char* terminal_type;	//!< 终端型号
	char* module_type;	//!< 模组型号
	char* hardware_version;	//!< 终端硬件版本
	char* software_version;	//!< 终端软件版本
	char* firmware_version;	//!< 模组固件版本
	char* IMEI;	//!< IMEI
	char* ICCID;	//!< ICCID
}info_report_s;

//*************************************************
//!
//! @addtogroup service为ctiot_srv_cb_s的数据结构体
//!
//! @{
//
//*************************************************
typedef struct
{
#if (DATA_MODE == 1 || DATA_MODE == 2)
	void (*sim_net_notice_act_callback)(char *);	//!< Sim卡断复网通知指令下发的回调
	void (*rda_act_data_cmd_callback)(char *);	//!< 业务数据下发的回调
#elif 0//DATA_MODE == 3
	void (*sim_net_notice_act_callback)(sim_net_notice_act_s *);	//!< Sim卡断复网通知指令下发的回调
	void (*rda_act_data_cmd_callback)(rda_act_data_cmd_s *);	//!< 业务数据下发的回调
#endif
} ctiot_srv_cb_s;

enum CTIOT_MQTT_SIM_NET_STATUS_8651{
	ctiot_mqtt_sim_net_status_8651_0 = 0,//!<   复网
	ctiot_mqtt_sim_net_status_8651_1 = 1,//!<   断网
};
enum CTIOT_MQTT_ACT_RESULT_9601{
	ctiot_mqtt_act_result_9601_0 = 0,//!<   执行成功
	ctiot_mqtt_act_result_9601_1 = 1,//!<   执行失败
};
enum CTIOT_MQTT_ACT_RESULT_9651{
	ctiot_mqtt_act_result_9651_0 = 0,//!<   执行成功
	ctiot_mqtt_act_result_9651_1 = 1,//!<   执行失败
};
enum CTIOT_MQTT_POWER_ONOFF_ALARM_1601{
	ctiot_mqtt_power_onoff_alarm_1601_0 = 0,//!<   正常
	ctiot_mqtt_power_onoff_alarm_1601_1 = 1,//!<   上电告警
	ctiot_mqtt_power_onoff_alarm_1601_2 = 2,//!<   下电告警
};
enum CTIOT_MQTT_COORDINATE_SYSTEM_604{
	ctiot_mqtt_coordinate_system_604_0 = 0,//!<   WGS_84
	ctiot_mqtt_coordinate_system_604_1 = 1,//!<   GCJ_02
};
enum CTIOT_MQTT_BATTERY_STATE_1602{
	ctiot_mqtt_battery_state_1602_0 = 0,//!<   正常
	ctiot_mqtt_battery_state_1602_1 = 1,//!<   低电
};

//**************************************************
//
//! @brief 下行回调
//!
//! @param void*
//!
//! @retval  无
//!
//**************************************************
void ctiot_dn_mqtt_data(void *md);

#if (DATA_MODE == 2 || DATA_MODE == 3  )
//**************************************************
//
//! @brief 初始化json
//!
//! @param 无
//!
//! @retval  无
//!
//**************************************************
void ctiot_tmodel_init(void);

//**************************************************
//
//! @brief 分离模式下释放编码后的payload空间
//!
//! @param void*
//!
//! @retval  无
//!
//**************************************************
void ctiot_tmodel_free(void *p);

//**************************************************
//
//! @brief 数据上报报文编码功能
//!
//! @param para 参数结构体
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_msg_encode(CTIOT_MQTT_PARA para, char **payload);

//**************************************************
//
//! @brief 指令响应报文编码功能
//!
//! @param para 参数结构体
//! @param taskId 消息ID
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_msg_response_encode(CTIOT_MQTT_PARA para, uint32_t taskId, char **payload);

//**************************************************
//
//! @brief service为Sim卡断复网通知指令下发
//!
//! @param payload 输入payload
//! @param sim_net_notice_act_s 参见@ref sim_net_notice_act_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_decode_sim_net_notice_act_para(char *payload,sim_net_notice_act_s* outPara);

//**************************************************
//
//! @brief service为业务数据下发响应
//!
//! @param rda_act_data_resp_s 参见@ref rda_act_data_resp_s
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_encode_rda_act_data_resp(rda_act_data_resp_s* para,char** payload);

//**************************************************
//
//! @brief service为Sim卡断复网通知下发响应
//!
//! @param sim_net_notice_resp_s 参见@ref sim_net_notice_resp_s
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_encode_sim_net_notice_resp(sim_net_notice_resp_s* para,char** payload);

//**************************************************
//
//! @brief service为上下电告警上报
//!
//! @param rda_power_onoff_alarm_report_s 参见@ref rda_power_onoff_alarm_report_s
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_encode_rda_power_onoff_alarm_report(rda_power_onoff_alarm_report_s* para,char** payload);

//**************************************************
//
//! @brief service为业务数据下发
//!
//! @param payload 输入payload
//! @param rda_act_data_cmd_s 参见@ref rda_act_data_cmd_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_decode_rda_act_data_cmd_para(char *payload,rda_act_data_cmd_s* outPara);

//**************************************************
//
//! @brief service为定位信息上报
//!
//! @param rda_position_report_s 参见@ref rda_position_report_s
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_encode_rda_position_report(rda_position_report_s* para,char** payload);

//**************************************************
//
//! @brief service为电池低电告警
//!
//! @param rda_battery_low_alarm_s 参见@ref rda_battery_low_alarm_s
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_encode_rda_battery_low_alarm(rda_battery_low_alarm_s* para,char** payload);

//**************************************************
//
//! @brief service为信号数据上报
//!
//! @param signal_report_s 参见@ref signal_report_s
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_encode_signal_report(signal_report_s* para,char** payload);

//**************************************************
//
//! @brief service为电池数据上报
//!
//! @param rda_data_report_s 参见@ref rda_data_report_s
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_encode_rda_data_report(rda_data_report_s* para,char** payload);

//**************************************************
//
//! @brief service为设备信息上报
//!
//! @param info_report_s 参见@ref info_report_s
//! @param payload 输出编码后的payload
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_encode_info_report(info_report_s* para,char** payload);

#if (DATA_MODE == 3)
//**************************************************
//
//! @brief service为业务数据下发响应
//!
//! @param rda_act_data_resp_s 参见@ref rda_act_data_resp_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_rda_act_data_resp_data(rda_act_data_resp_s* para,mqtt_qos_e qos, ctiot_mqtt_msg_s *ptrMsg);

//**************************************************
//
//! @brief service为Sim卡断复网通知下发响应
//!
//! @param sim_net_notice_resp_s 参见@ref sim_net_notice_resp_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_sim_net_notice_resp_data(sim_net_notice_resp_s* para,mqtt_qos_e qos, ctiot_mqtt_msg_s *ptrMsg);

//**************************************************
//
//! @brief service为上下电告警上报
//!
//! @param rda_power_onoff_alarm_report_s 参见@ref rda_power_onoff_alarm_report_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_rda_power_onoff_alarm_report_data(rda_power_onoff_alarm_report_s* para,mqtt_qos_e qos, ctiot_mqtt_msg_s *ptrMsg);

//**************************************************
//
//! @brief service为定位信息上报
//!
//! @param rda_position_report_s 参见@ref rda_position_report_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_rda_position_report_data(rda_position_report_s* para,mqtt_qos_e qos, ctiot_mqtt_msg_s *ptrMsg);

//**************************************************
//
//! @brief service为电池低电告警
//!
//! @param rda_battery_low_alarm_s 参见@ref rda_battery_low_alarm_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_rda_battery_low_alarm_data(rda_battery_low_alarm_s* para,mqtt_qos_e qos, ctiot_mqtt_msg_s *ptrMsg);

//**************************************************
//
//! @brief service为信号数据上报
//!
//! @param signal_report_s 参见@ref signal_report_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_signal_report_data(signal_report_s* para,mqtt_qos_e qos, ctiot_mqtt_msg_s *ptrMsg);

//**************************************************
//
//! @brief service为电池数据上报
//!
//! @param rda_data_report_s 参见@ref rda_data_report_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_rda_data_report_data(rda_data_report_s* para,mqtt_qos_e qos, ctiot_mqtt_msg_s *ptrMsg);

//**************************************************
//
//! @brief service为设备信息上报
//!
//! @param info_report_s 参见@ref info_report_s
//!
//! @retval  CTIOT_RESULT_CODE 返回结果码
//!
//**************************************************
CTIOT_RESULT_CODE ctiot_mqtt_send_info_report_data(info_report_s* para,mqtt_qos_e qos, ctiot_mqtt_msg_s *ptrMsg);

#endif
#endif

#endif // DATA_MODE > 0

#ifdef __cplusplus
}
#endif

#endif //#define _CTIOT_MQTT_SDK_TMODEL_H
