/*************************************************************************

            (c) Copyright 2019 by 天翼物联科技有限公司. All rights reserved.

**************************************************************************/
#ifndef __FFW_CTIOT_H__
#define __FFW_CTIOT_H__
#include <stdint.h>
#include <ffw_timer.h>
#include "ctiot_mqtt_sdk.h"
#include "ctiot_mqtt_sdk_tmodel.h"

#define CTIOT_DIAGNOSE_MAX_LEN (32)
#define CTIOT_DIAGNOSE_LOCK_CLOSE (0)
#define CTIOT_DIAGNOSE_LOCK_NO_GREEN_CHANNEL (1)
#define CTIOT_DIAGNOSE_LOCK_GREEN_CHANNEL (2)
#define CTIOT_SIGNAL_ESCALATION_CYCLE_MIN (1)
#define CTIOT_SIGNAL_ESCALATION_CYCLE_MAX (1440)
#define CTIOT_DEVICE_INFO_REPORT_PERIOD_MIN (1)
#define CTIOT_DEVICE_INFO_REPORT_PERIOD_MAX (10080)
#define CTIOT_BATTERY_VOLTAGE_MIN (0)
#define CTIOT_BATTERY_VOLTAGE_MAX (36)
#define CTIOT_LONGITUDE_MIN (-180)
#define CTIOT_LONGITUDE_MAX (180)
#define CTIOT_LATITUDE_MIN (-90)
#define CTIOT_LATITUDE_MAX (90)
#define CTIOT_ALTITUDE_MIN (-1024)
#define CTIOT_ALTITUDE_MAX (9999)
#define CTIOT_POS_SPEED_MIN (0)
#define CTIOT_POS_SPEED_MAX (500)
#define CTIOT_COORDINATE_SYS_NO0 (0)
#define CTIOT_COORDINATE_SYS_NO1 (1)
#define CTIOT_POWER_NORMAL_ALARM (0)
#define CTIOT_POWER_ON_ALARM (1)
#define CTIOT_POWER_OFF_ALARM (2)
#define CTIOT_BATTERY_STATE_NOEMAL (0)
#define CTIOT_BATTERY_STATE_LOW (1)
#define CTIOT_BATTERY_VALUE_PERCENT_MIN (0)
#define CTIOT_BATTERY_VALUE_PERCENT_MAX (100)
#define CTIOT_TASKID_MIN (0)
#define CTIOT_TASKID_MAX (65536)
#define CTIOT_ACT_RESULT_SUCCESS (0)
#define CTIOT_ACT_RESULT_FAIL (1)
#define EPS 1e-7

#define CTIOT_PARAM_PATH "/ctiot_param"
#define CTIOT_USR_PATH "/ctiot_usr"
#define CTIOT_MQTT_TOPIC_MAX_LEN 255
#define CTIOT_MQTT_MSQ_MAX_LEN 1024
#define CTIOT_MQTT_PUB_ASYNC_DEFAULT_TIMEOUT (30*1000)
#define CTIOT_MQTTPUB_TEST "+CTMQTTPUB: 255,(0-2),(1-1024)"


//extern ffw_timerp_t rda_data_report_timer;
//extern ffw_timerp_t rda_position_report_timer;
extern ffw_timerp_t info_report_timer;
extern ffw_timerp_t signal_info_report_timer;


enum ctiot_func_param
{
    //"lock"  Enable or disable the remote diagnosis function
    CTIOT_LOCK,

    //"set_dev_info"  Setting Vendor Information
    CTIOT_SET_DEV_INFO,

    //"signal_report"  Set the signal reporting period
    CTIOT_SIGNAL_REPORT,

    //"info_report"  Set the interval for reporting device information
    CTIOT_INFO_REPORT,

    //"rda_data_report"  Report service data
    CTIOT_RDA_DATA_REPORT,

    //"rda_position_report"  Report location information
    CTIOT_RDA_POSITION_REPORT,

    //"rda_power_onoff_alarm_report"  A power-on or power-off alarm is reported
    CTIOT_RDA_POWER_ONOFF_ALARM_REPORT,

    //"rda_battery_low_alarm"  The battery low alarm was reported
    CTIOT_RDA_BATTERY_LOW_ALARM,

    //"rda_act_data_resp"  User instruction answer
    CTIOT_RDA_ACT_DATA_RESP,

    //"sim_net_notice_resp"  Sim card disconnected network command answer
    CTIOT_SIM_NET_NOTICE_RESP,
};
typedef enum ctiot_func_param ctiot_func_param_t;

typedef struct ctiot_diagnose_param
{
    uint8_t lock_enable;

    uint16_t signal_escalation_cycle;

    uint16_t device_info_report_period;

    char manufacturer_name[32];
    char terminal_type[32];
    char software_version[32];
    char hardware_version[32];
    char firmware_version[32];
    bool set_dev_info_enable; //if it is true,the dev info will be reported to plat

#if 0  /*do not report periodly*/
    uint8_t battery_state;
    double battery_voltage;
    uint8_t battery_value;
    //bool rda_data_report_enable;//if it is true,the battery data info will be reported to plat

    double longitude;
    double latitude;
    double altitude;
    uint8_t coordinate_system;
    double pos_speed;
    //bool rda_position_report_enable;//if it is true,the position info will be reported to plat
    uint8_t power_onoff_alarm;
#endif

    int32_t taskID;
    uint8_t act_result;
    char act_msg[32];
    char act_data[32];

}ctiot_diagnose_param_t;

typedef struct ctiot_diagnose_usr
{
    char mqtt_id[128];
    char mqtt_token[128];

}ctiot_diagnose_usr_t;


typedef struct
{
	void(*set_auto_control_callback)( char *);
	void(*set_report_period_callback)( char *);
	void(*air_conditioner_onoff_cmd_callback)( char *);
	 //!<
} ctiot_down_cb_t;


typedef enum
{
    GET_CELLINFO_MSG = 1,
}get_cellinfo_msg_t;

int fibo_ctiot_mqtt_init();int ctiot_mqtt_entry(void);
ctiot_diagnose_param_t fibo_get_ctiot_diagnose();
void fibo_set_ctiot_diagnose(ctiot_diagnose_param_t params);
void fibo_write_ctiot_diagnose_param_file(ctiot_diagnose_param_t params);
void fibo_read_ctiot_diagnose_param_file();
int fibo_ctiot_mqtt_execute_rda_power_onoff_alarm_report(uint8_t poewr_onoff_alarm);
int fibo_ctiot_mqtt_execute_rda_battery_low_alarm(uint8_t battery_state, double battery_voltage, uint8_t battery_value);
#if 0
void fibo_ctiot_mqtt_execute_rda_position_report(void *arg);
void fibo_ctiot_mqtt_execute_rda_data_report(void *arg);
#else
int fibo_ctiot_mqtt_execute_rda_position_report(rda_position_report_s position_info);
int fibo_ctiot_mqtt_execute_rda_data_report(float battery_voltage, int32_t battery_value);
#endif
int fibo_ctiot_mqtt_execute_push_data(void *engine, const char *topic,mqtt_qos_e qos, int payload_len);
void fibo_ctiot_mqtt_execute_info_report(void *arg);
void fibo_ctiot_mqtt_execute_signal_report(void *arg);
int fibo_ctiot_mqtt_execute_rda_act_data_resp(int32_t taskId);
int fibo_ctiot_mqtt_execute_sim_net_notice_resp(int32_t taskId);
void ctiot_auto_mqtt();
void fibo_get_siminfo_async();
void fibo_write_ctiot_usr_file(ctiot_diagnose_usr_t params);
ctiot_diagnose_usr_t fibo_read_ctiot_usr_file();


#endif
