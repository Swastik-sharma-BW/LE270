/* Copyright Statement:
*
* (C) 2021  fibocom  All rights reserved.
* 
*  chenlu  create initial version
*
 **************************************************************************
 * File Description
 * ----------------
 *
 * fib_lpg.h
 *
 * chenlu create initial version  20211220

 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include "ffw_nv.h"


#ifndef _FIBLPG_MAIN_H_
#define _FIBLPG_MAIN_H_

typedef struct
{
    void *lpg_event_timer;
    bool timer_flag;
    int16_t high_period;
    int16_t low_period;
    int16_t lpg_cont;
    int16_t lpg_on;
    int16_t lpg_status;
    int16_t gpio_num;   // this is the pin_index
    char flag;
} lpg_timer_config_t;

typedef struct
{
    int16_t lpg_status;
    int16_t lpgon_time;
    int16_t lpgoff_time;
} lpg_info_t;

bool ffw_sleep_lpg_switch(bool is_on);
bool ffw_handleLpgSwitch(int32_t is_on);
bool ffw_lpg_cfg_get();
void ffw_tgpio_lpg_switch(void);
extern void ffw_lpg_start(void);


#endif



