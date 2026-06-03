#ifndef __STARCART_SOL_H__
#define __STARCART_SOL_H__
#ifdef __cplusplus
 extern "C" {
#endif

#include <math.h>
#include <stdarg.h>
#define gps_sys_mask 1
#define glo_sys_mask 2
#define gal_sys_mask 4
#define bds_sys_mask 8

#define POST_SOL_EN 1
// gnss数据结构体
typedef struct 
{
	char gga[128];
	char rmc[128];
}nmea_stuct;

// 回调结构体
typedef struct{
	void (*printf_log_func)(const char *fmt,...);
	void (*g_nmea_callback)(nmea_stuct nmea_data);
}Parse_Callbacks;

// 配置结构体
typedef struct{
    int sys_support;
    int log_level;
    int rtcm_print; // 1 print, 0 not print
}parse_sol_config;


// 解析 vrs 数据
int http_parse_vrs(unsigned char vrs_ch);
// 解析 obs 与 nav 数据
int uart_parse_obs_nav(unsigned char input_ch);

void starcart_sol_c_init(void);

#ifdef __cplusplus
}
#endif
#endif /*__STARCART_SOL_H__ */