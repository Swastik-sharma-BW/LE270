/******************************************************************************

*(C) Copyright 2024 Fibocom International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description: LE270 GPIO ADC LPM LPG Resources
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

//LE270 adc resources
const ffw_device_adc_table device_adc_table[] =
{
    {0, ADC_CHANNEL_AIO1},
    {1, ADC_CHANNEL_AIO2}
};
const int32_t device_adc_size = (sizeof(device_adc_table) / sizeof(device_adc_table[0]));
const char *mmad_at_test = "+MMAD: (0-1),(0-3)";

//LE270 lpm wakeup host resources
const ffw_pal_pm_wake_host_cfg_table pal_pm_wake_host_cfg_table[] =
{
    {0, 20},
    {1, 0xFF},
    {2, 20}
};
const char *gtwake_at_test = "+GTWAKE: (0,2),(0,1)";

//LE270 lpm resources
const ffw_pal_pm_sleep_cfg_table pal_pm_sleep_cfg_table[] = {
    {0,     0xFF,   0xFF,           0xFF},
    {1,     0xFF,   0xFF,           0xFF},
    {2,     19,     WAKEUP_PAD_5,   PadWakeup5_IRQn}
};
const char *gtlpmmode_at_test = "+GTLPMMODE: (0,2),(0,1)";

//LE270 gpio resources
pin_map_t g_pin_table[] =
{
    {0,  23, 48, 0, 0, true, true},   //AGPIO3 VDD_EXT POWER
    {5,  20, 45, 0, 0, true, true},   //AGPIOWU0
    {6,  21, 46, 0, 0, true, true},   //AGPIOWU1
    {16, 25, 50, 0, 0, true, true},   //NET_STATUS
    {17, 18, 33, 1, 0, true, false},   //MAIN_RXD
    {18, 19, 34, 1, 0, true, false},   //MAIN_TXD
    {19, 22, 47, 0, 0, true, true},   //MAIN_DTR
    {20, 27, 52, 0, 0, true, true},   //MAIN_RI
    {21, 17, 12, 0, 4, true, false},   //MAIN_DCD
    {22, 1 , 16, 3, 0, true, false},   //MAIN_RTS
    {23, 2 , 17, 3, 0, true, false},   //MAIN_CTS
    {25, 26, 51, 0, 0, true, true},   //STATUS
    {26, 33, 39, 1, 0, true, false},   //I2S_MCLK
    {28, 10, 25, 1, 0, true, false},   //AUX_RXD
    {29, 11, 26, 1, 0, true, false},   //AUX_TXD
    {30, 29, 35, 1, 0, true, false},   //PCM_CLK
    {31, 30, 36, 1, 0, true, false},   //PCM_SYNC
    {32, 31, 37, 1, 0, true, false},   //PCM_DIN
    {33, 32, 38, 1, 0, true, false},   //PCM_DOUT
    {38, 16, 31, 1, 0, true, false},   //DBG_RXD
    {39, 17, 32, 1, 0, true, false},   //DBG_TXD
    {49, 15, 30, 4, 0, true, false},   //LCD_RST
    {50, 37, 43, 1, 0, true, false},   //LCD_SIO
    {51, 38, 44, 1, 0, true, false},   //LCD_SDC
    {52, 35, 41, 1, 0, true, false},   //LCD_CS
    {53, 34, 40, 1, 0, true, false},   //LCD_CLK
    {54, 3 , 18, 1, 0, true, false},   //CAM_MCLK
    {55, 6 , 21, 1, 0, true, false},   //CAM_DATA0
    {56, 7 , 22, 1, 0, true, false},   //CAM_DATA1
    {57, 18, 13, 2, 4, true, false},   //CAM_I2C0_SCL
    {58, 19, 14, 2, 4, true, false},   //CAM_I2C0_SDA
    {62, 14, 29, 4, 0, true, false},   //USIM2_CLK
    {63, 13, 28, 4, 0, true, false},   //USIM2_RST
    {64, 12, 27, 4, 0, true, false},   //USIM2_DATA
    {66, 8 , 23, 2, 0, true, false},   //I2C1_SDA
    {67, 9 , 24, 2, 0, true, false},   //I2C1_SCL
    {78, 36, 42, 1, 0, true, false},   //LCD_TE
    {80, 4 , 19, 1, 0, true, false},   //CAM_SPI_CLK
    {81, 5 , 20, 1, 0, true, false},   //CAM_PWDN
    {82, 0 , 15, 0, 0, true, false},   //USB_BOOT
    {100,23, 48, 0, 0, true, true},   //RESERVED
    {101,24, 49, 0, 0, true, true},   //RESERVED
    {103,16, 11, 0, 4, true, false},   //CAM_RST_N
};
const int32_t max_pin = 104; //max_pin = pin_map_t_max_pin_id + 1
const int32_t g_pin_table_size = (sizeof(g_pin_table) / sizeof(g_pin_table[0]));

//LE270 wakeup_pad resources
const ffw_wakeuppad_t g_wkepad_table[]=
{
    {79,     WAKEUP_PAD_2,   PadWakeup2_IRQn}, //SIM1_DET
    {5,      WAKEUP_PAD_3,   PadWakeup3_IRQn}, //AGPIOWU0
    {6,      WAKEUP_PAD_4,   PadWakeup4_IRQn}, //AGPIOWU1
    {19,     WAKEUP_PAD_5,   PadWakeup5_IRQn}  //MAIN_DTR
};
const int32_t g_wkepad_table_size = (sizeof(g_wkepad_table) / sizeof(g_wkepad_table[0]));

//LE270 AT+GPIO command resources
const uint32_t gpio_pin_table[] = {100, 101};
const int32_t gpio_pin_table_size = (sizeof(gpio_pin_table) / sizeof(gpio_pin_table[0]));
const char *gpio_at_test = "+GPIO: (100,101),(0-2),(0,1)";

//LE270 LPG resources
const uint32_t FIBO_NETLPG_PIN_NUM = 16;
const uint32_t FIBO_SYSLPG_PIN_NUM = 25;

//customer config UART_recv_task_stack_size
const uint32_t g_uart_recv_task_stack_cfg[]={
    16*1024,    //uart0 recvtask default stack
    16*1024,    //uart1 recvtask default stack
    16*1024,    //uart2 recvtask default stack
    16*1024     //uart3 recvtask default stack
};