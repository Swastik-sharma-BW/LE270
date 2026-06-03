/******************************************************************************

*(C) Copyright 2024 Fibocom International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description: LE371 GPIO ADC LPM LPG Resources
*
*  History: initiated by xxxx
*
*  Notes:
*
******************************************************************************/

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "fibo_gpio_resource.h"

//LE371 adc resources
const ffw_device_adc_table device_adc_table[] =
{
    {0, ADC_CHANNEL_AIO1},
    {1, ADC_CHANNEL_AIO2},
    {2, ADC_CHANNEL_AIO3},
    {3, ADC_CHANNEL_AIO4}
};
const int32_t device_adc_size = (sizeof(device_adc_table) / sizeof(device_adc_table[0]));
const char *mmad_at_test = "+MMAD: (0-3),(0-3)";

//LE371 lpm wakeup host resources
const ffw_pal_pm_wake_host_cfg_table pal_pm_wake_host_cfg_table[] =
{
    {0, 20},
    {1, 0xFF},
    {2, 20}
};
const char *gtwake_at_test = "+GTWAKE: (0,2),(0,1)";

//LE371 lpm resources
const ffw_pal_pm_sleep_cfg_table pal_pm_sleep_cfg_table[] = {
    {0,     0xFF,   0xFF,           0xFF},
    {1,     0xFF,   0xFF,           0xFF},
    {2,     19,     WAKEUP_PAD_3,   PadWakeup3_IRQn}
};
const char *gtlpmmode_at_test = "+GTLPMMODE: (0,2),(0,1)";

//LE371 gpio resources
pin_map_t g_pin_table[] =
{
    { 5, 12, 24, 0, 0, true, true},    //RESERVED/AGPIOWU2:GNSSEN
    {16,  0, 12, 0, 0, true, false},   //NET_STATUS(USB_BOOT)
    {17,  8, 20, 1, 0, true, false},   //MAIN_RXD
    {18,  9, 21, 1, 0, true, false},   //MAIN_TXD
    {19, 10, 22, 0, 0, true, true},    //MAIN_DTR
    {20, 13, 25, 0, 0, true, true},    //MAIN_RI
    {21, 11, 23, 0, 0, true, true},    //RESERVED/MAIN_DCD:GNSS BOOT
    {22, 19, 10, 5, 4, true, false},   //RESERVED/MAIN_RTS
    {23, 20, 11, 5, 4, true, false},   //RESERVED/MAIN_CTS:GNSS RESET
    {25, 15, 27, 0, 0, true, true},    //STATUS
    {28,  4, 16, 2, 0, true, false},   //RESERVED/AUX_RXD:GNSS UART TX
    {29,  5, 17, 2, 0, true, false},   //RESERVED/AUX_TXD:GNSS UART RX
    {38,  6, 18, 1, 0, true, false},   //DBG_RXD
    {39,  7, 19, 1, 0, true, false},   //DBG_TXD
    {62,  2, 14, 4, 0, true, false},   //USIM2_CLK
    {63,  1, 13, 4, 0, true, false},   //USIM2_RST
    {64,  3, 15, 4, 0, true, false},   //USIM2_DATA
    {66, 18,  9, 4, 4, true, false},   //I2C_SDA
    {67, 17,  8, 4, 4, true, false},   //I2C_SCL
    {81, 14, 26, 0, 0, true, true},    //AGPIO4/LDO_EN/GNSS_AVD33BAK:GNSS backup power
    {82,  0, 12, 0, 0, true, false},   //USB_BOOT
};
const int32_t max_pin = 83; //max_pin = pin_map_t_max_pin_id + 1
const int32_t g_pin_table_size = (sizeof(g_pin_table) / sizeof(g_pin_table[0]));

//LE371 wakeup_pad resources
const ffw_wakeuppad_t g_wkepad_table[]=
{
    {79,      WAKEUP_PAD_1,   PadWakeup1_IRQn}, //SIM1_DET
    {19,      WAKEUP_PAD_3,   PadWakeup3_IRQn}, //MAIN_DTR
    {21,      WAKEUP_PAD_4,   PadWakeup4_IRQn}, //RESERVED
    { 5,      WAKEUP_PAD_5,   PadWakeup5_IRQn}  //RESERVED
};
const int32_t g_wkepad_table_size = (sizeof(g_wkepad_table) / sizeof(g_wkepad_table[0]));

//LE371 AT+GPIO command resources
const uint32_t gpio_pin_table[] = {};
const int32_t gpio_pin_table_size = (sizeof(gpio_pin_table) / sizeof(gpio_pin_table[0]));
const char *gpio_at_test = "+GPIO: ( ),(0-2),(0,1)";

//LE371 LPG resources
const uint32_t FIBO_NETLPG_PIN_NUM = 16;
const uint32_t FIBO_SYSLPG_PIN_NUM = 25;
//customer config UART_recv_task_stack_size
const uint32_t g_uart_recv_task_stack_cfg[]={
    16*1024,    //uart0 recvtask default stack
    16*1024,    //uart1 recvtask default stack
    16*1024,    //uart2 recvtask default stack
};