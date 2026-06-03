#ifndef D7EC1E27_1C3E_46BD_921E_21A7DAF622C2
#define D7EC1E27_1C3E_46BD_921E_21A7DAF622C2

#include <stdbool.h>
#include <stdint.h>
#include "fibo_opencpu_comm.h"

#define MAX_AP_NAME_LEN 99
#define MAX_AP_ADDR_LEN 99
#define MAX_AP_NUM 30

typedef struct fibo_wifiscan_info{
    int8_t  ap_rssi;
    uint8_t ap_name[MAX_AP_NAME_LEN];
    uint8_t ap_addr[MAX_AP_ADDR_LEN];
}fibo_wifiscan_ap_info_t;

typedef struct fibo_wifiscan_res{
    int8_t  ap_num;
    fibo_wifiscan_ap_info_t ap_info[MAX_AP_NUM];
    int8_t  result;
}fibo_wifiscan_res_t;

#define LBS_RAWDATA_MAX_LEN 512
#define LBS_DESC_MAX_LEN 256
#define LBS_LOC_MAX_LEN 64

typedef struct fibo_lbs_res{
    char *loc;
    char *desc;
    char *raw;
}fibo_lbs_res_t;


INT32 fibo_lbs_get_gisinfo_v2(char *key, UINT8 type, uint8_t simid, fibo_lbs_res_t *lbsinfo, INT32 timeout);

INT32 fibo_wifiscan_v2(uint8_t simid, key_callback cb);

#endif /* D7EC1E27_1C3E_46BD_921E_21A7DAF622C2 */
