#ifndef __OC_GNSS_H__
#define __OC_GNSS_H__

#include "fibo_opencpu_comm.h"
typedef struct
{ 
    UINT8 ucDirect; // 0-西经/南纬，1-东经/北纬 
    int degree; // 度，[0,180] 
    int minute; // 分，[0,60] 
    double second; // 秒，[0,60] 
}fibo_iot_gps_loc_tude_s;

typedef struct
{
    int loc_stat; // 状态，0 无效 1 有效 
    int satellite;// 有效卫星数 
    fibo_iot_gps_loc_tude_s longitude; // 经度 
    fibo_iot_gps_loc_tude_s latitude; // 纬度 
    char dateTime[32]; //采集GPS 的时间戳 2021-01-28T02:00:00.000 时区由应用来添加 
    int snr;
}fibo_iot_gps_notify_info_s;

#define GNSS_NMEA_RMC   "RMC"
#define GNSS_NMEA_GGA   "GGA"
#define GNSS_NMEA_GSV   "GSV"

int32_t fibo_gnss_enable(uint8_t en);

int32_t fibo_gnss_weakup(void);

int32_t fibo_gnss_sleep(void);

int32_t fibo_gnss_nmea_parse(fibo_iot_gps_notify_info_s *nmea_data, int size);

int32_t fibo_gnss_nmea_get(uint8_t *nmea_data, int size);

int32_t fibo_gnss_nmea_freq_set(int freq);

int32_t fibo_gnss_set_agnss_mode(int mode);

int32_t fibo_gnss_set_satellite(int satellite);

int32_t fibo_gnss_get_satellite(void);

int32_t fibo_gnss_nmea_config(char *nmea_names, int on);

int32_t fibo_gnss_cmd_send(void *cmd, int len);

int32_t fibo_gnss_set_agnss_server(char* host, int host_len, int port, uint8_t srv_type);

#endif

