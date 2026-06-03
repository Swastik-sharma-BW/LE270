/******************************************************************************

*(C) Copyright 2024 Fibocom International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description: LE370 GPIO ADC LPM LPG Resources
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

//LE370 adc resources
const ffw_device_adc_table device_adc_table[] =
{
    {0, ADC_CHANNEL_AIO1},
    {1, ADC_CHANNEL_AIO2}
};
const int32_t device_adc_size = (sizeof(device_adc_table) / sizeof(device_adc_table[0]));
const char *mmad_at_test = "+MMAD: (0-1),(0-3)";

//LE370 lpm wakeup host resources
const ffw_pal_pm_wake_host_cfg_table pal_pm_wake_host_cfg_table[] =
{
    {0, 20},
    {1, 0xFF},
    {2, 20}
};
const char *gtwake_at_test = "+GTWAKE: (0,2),(0,1)";

//LE370 lpm resources
const ffw_pal_pm_sleep_cfg_table pal_pm_sleep_cfg_table[] = {
    {0,     0xFF,   0xFF,           0xFF},
    {1,     0xFF,   0xFF,           0xFF},
    {2,     19,     WAKEUP_PAD_5,   PadWakeup5_IRQn}
};
const char *gtlpmmode_at_test = "+GTLPMMODE: (0,2),(0,1)";

//LE370 gpio resources
pin_map_t g_pin_table[] =
{
    {0,  13, 25, 0, 0, true, true},   //AGPIO3 VDD_EXT POWER
    {5,  10, 22, 0, 0, true, true},   //AGPIOWU0
    {6,  17,  8, 4, 4, true, false},   //GPIO
    {16, 14, 26, 0, 0, true, true},   //NET_STATUS
    {17,  8, 20, 1, 0, true, false},   //MAIN_RXD
    {18,  9, 21, 1, 0, true, false},   //MAIN_TXD
    {19, 12, 24, 0, 0, true, true},   //MAIN_DTR
    {20, 11, 23, 0, 0, true, true},   //MAIN_RI
    {21, 18,  9, 5, 4, true, false},   //MAIN_DCD
    {22, 19, 10, 5, 4, true, false},   //MAIN_RTS
    {23, 20, 11, 5, 4, true, false},   //MAIN_CTS
    {25, 15, 27, 0, 0, true, true},   //STATUS
    {28,  4, 16, 2, 0, true, false},   //AUX_RXD
    {29,  5, 17, 2, 0, true, false},   //AUX_TXD
    {38,  6, 18, 1, 0, true, false},   //DBG_RXD
    {39,  7, 19, 1, 0, true, false},   //DBG_TXD
    {62,  2, 14, 4, 0, true, false},   //USIM2_CLK
    {63,  1, 13, 4, 0, true, false},   //USIM2_RST
    {64,  3, 15, 4, 0, true, false},   //USIM2_DATA
    {81, 13, 25, 0, 0, true, true},    //AGPIO3: resve VDD_EXT POWER
    {82,  0, 12, 0, 0, true, false},   //USB_BOOT
};
const int32_t max_pin = 83; //max_pin = pin_map_t_max_pin_id + 1
const int32_t g_pin_table_size = (sizeof(g_pin_table) / sizeof(g_pin_table[0]));

//LE370 wakeup_pad resources
const ffw_wakeuppad_t g_wkepad_table[]=
{
    {79,      WAKEUP_PAD_1,   PadWakeup1_IRQn}, //SIM1_DET
    {5,       WAKEUP_PAD_3,   PadWakeup3_IRQn}, //AGPIOWU0
    {20,      WAKEUP_PAD_4,   PadWakeup4_IRQn}, //MAIN_RI
    {19,      WAKEUP_PAD_5,   PadWakeup5_IRQn}  //MAIN_DTR
};
const int32_t g_wkepad_table_size = (sizeof(g_wkepad_table) / sizeof(g_wkepad_table[0]));

//LE370 AT+GPIO command resources
const uint32_t gpio_pin_table[] = {6};
const int32_t gpio_pin_table_size = (sizeof(gpio_pin_table) / sizeof(gpio_pin_table[0]));
const char *gpio_at_test = "+GPIO: (6),(0-2),(0,1)";

//LE370 LPG resources
const uint32_t FIBO_NETLPG_PIN_NUM = 16;
const uint32_t FIBO_SYSLPG_PIN_NUM = 25;
//customer config UART_recv_task_stack_size
const uint32_t g_uart_recv_task_stack_cfg[]={
    16*1024,    //uart0 recvtask default stack
    16*1024,    //uart1 recvtask default stack
    16*1024,    //uart2 recvtask default stack
};