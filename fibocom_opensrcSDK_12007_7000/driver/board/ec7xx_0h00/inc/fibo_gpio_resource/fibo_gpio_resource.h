#ifndef  FIBO_GPIO_RESOURCE_H
#define  FIBO_GPIO_RESOURCE_H

#include "adc.h"
#include "slpman.h"

//adc resources struct
typedef struct {
    int32_t adc_channal_id;
    AdcChannel_e Channel_e;
}ffw_device_adc_table;

//lpm wakeup host resources struct
typedef struct{
    int32_t wake_host_mode;
    uint32_t  pin_id;
}ffw_pal_pm_wake_host_cfg_table;

//lpm resources struct
typedef struct{
    int32_t lpm_mode;
    uint16_t lpm_pin;
    uint16_t gpio;
    uint16_t irq;
}ffw_pal_pm_sleep_cfg_table;

//gpio resources
typedef struct
{
    uint32_t pin_index;
    uint32_t gpio_index;
    uint32_t pad_addr;
    uint32_t default_mode;
    uint32_t gpio_mode;
    bool is_high_resistance;
    bool is_aon; //EC Feature
}pin_map_t;

//open gpio wakeup_pad resources
typedef struct
{
    uint32_t pin_id;
    uint32_t wakeup_pad;
    uint32_t wakeup_irq;
}ffw_wakeuppad_t;
#endif

