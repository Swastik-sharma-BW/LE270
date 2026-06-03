#ifndef _OC_CELL_H_
#define _OC_CELL_H_

#include <stdbool.h>
#include <stdint.h>

typedef void (*key_callback)(void* param);

typedef struct {
    bool cache_mode;
    uint16_t maxQueryTime; //unit s
}cell_info_cfg_t;

typedef struct
{
    uint16_t nw_drx;
    uint16_t user_drx;
}drx_cycle_cfg_t;

// add cellinfo
INT32 fibo_cell_info_get(key_callback cellinfo_refponse_cb,uint8_t nSim);
INT32 fibo_get_signal_info(int *snr,int *rsrp,int *rsrq,int *rssi);
#endif
