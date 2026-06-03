#ifndef FFW_WIFISCAN
#define FFW_WIFISCAN

#include <stdint.h>  // for int32_t
#include "ffw_result.h"
#include "ffw_rat.h"

#ifdef CONFIG_FFW_WIFISCAN_ENABLE

#define FFW_AP_MAC_ADDR_LEN 65
#define FFW_AP_SSID_LEN 65
#define FFW_AP_MAX_NUM 30

#define FFW_WIFISCAN_CHANNELID_MAX                 14

typedef struct ffw_wifi_para
{
    int32_t timeout;
    uint8_t maxBssidNum;
    uint8_t report_flag;
    uint8_t channelCount;
    uint8_t channeID[FFW_WIFISCAN_CHANNELID_MAX];
    uint8_t current_num;
    int64_t start_ms;
    uint8_t report_channel;
    bool all_channel_scan;
}ffw_wifiscan_para_t;


typedef struct
{
    char ap_mac[FFW_AP_MAC_ADDR_LEN];
    int8_t ap_rssi;
    char ap_name[FFW_AP_SSID_LEN];
    uint8_t channel;
}ffw_ap_info_t;

typedef struct
{
    ffw_ap_info_t ap_info[FFW_AP_MAX_NUM];
    int8_t ap_num;
}ffw_wifi_info_t;

typedef struct ffw_wifi_context_s
{
    uint8_t simid;
    void *arg;
    ffw_wifi_info_t wifi;
    int result;
    bool pending;
    void *sem;
    void *lock;
    void *timer;
    bool init;
    
    ffw_wifiinfo_callback_t cb;
} ffw_wifi_context_t;

typedef void (*ffw_wifiscan_callback_t)(int result, ffw_wifi_info_t *wifi, void *arg);

int ffw_wifi_scan_start(void);

/**
* @brief 
* 
* @param ap_info 
* @param result 
* @return int 
 */
int ffw_wifi_scan_end(ffw_wifi_info_t ap_info, ffw_result_t result);

/**
* @brief 
* 
* @param simid 
* @param timeout 
* @param wifi_info 
* @return int 
 */
int ffw_get_ap_info(int simid, int32_t timeout, ffw_wifi_info_t *wifi_info);

/**
* @brief 将数组类型的WIFI名称转换成字符串类型
* 
* @param from         WIFI名
* @param from_size    WIFI名长度
* @return void*       转换成字符串类型的WIFI名地址
 */
void* get_wifi_ssid(void *from, int from_size);

int ffw_get_wifiinfo1(ffw_wifiinfo_callback_t cb, void *arg);


#endif

#endif /* FFW_WIFISCAN */


