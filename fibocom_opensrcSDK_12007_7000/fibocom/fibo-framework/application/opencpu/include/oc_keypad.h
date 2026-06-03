#ifndef __OC_KEYPAD_H__
#define __OC_KEYPAD_H__

#include "fibo_opencpu_comm.h"

typedef enum {
    KEY_RELEASE,
    KEY_PRESS,
    SHORT_TIME_PRESS,
    LONG_TIME_PRESS,
}keypad_status;

typedef struct {
    int32_t key_id;
    int8_t long_or_short_press;
    int8_t press_or_release;
}keypad_info_t;

bool fibo_pwrkeypad_config(UINT32 timeout, bool Ispwroff);
UINT8 fibo_get_pwrkeystatus(void);
INT32 fibo_set_pwr_callback_ex(key_callback pwr_cb,UINT16 long_press,void *arg);
#endif