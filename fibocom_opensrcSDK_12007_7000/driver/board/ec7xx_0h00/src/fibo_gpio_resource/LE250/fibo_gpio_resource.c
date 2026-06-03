/******************************************************************************

*(C) Copyright 2024 Fibocom International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description: LE250 GPIO ADC LPM LPG Resources
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

//LE250 adc resources
const ffw_device_adc_table device_adc_table[] =
{
    {1, ADC_CHANNEL_AIO1},
    {2, ADC_CHANNEL_AIO2},
    {3, ADC_CHANNEL_AIO3},
    {4, ADC_CHANNEL_AIO4}
};
const int32_t device_adc_size = (sizeof(device_adc_table) / sizeof(device_adc_table[0]));
const char *mmad_at_test = "+MMAD: (1-4),(0-3)";

//LE250 lpm wakeup host resources
const ffw_pal_pm_wake_host_cfg_table pal_pm_wake_host_cfg_table[] =
{
    {0, 61},
    {1, 61},
    {2, 0xFF}
};
const char *gtwake_at_test = "+GTWAKE: (0,1),(0,1)";

//LE250 lpm resources
const ffw_pal_pm_sleep_cfg_table pal_pm_sleep_cfg_table[] = {
    {0,     0xFF,   0xFF,           0xFF},
    {1,     41,     WAKEUP_PAD_5,   PadWakeup5_IRQn},
    {2,     0xFF,   0xFF,           0xFF}
};
const char *gtlpmmode_at_test = "+GTLPMMODE: (0,1),(0,1)";

//LE250 gpio resources
pin_map_t g_pin_table[] =
{
    {0,  23, 48, 0, 0, true, true},    //AGPIO3 VDD_EXT POWER
    {15, 12, 27, 4, 0, true, false},   //SIM2_DATA
    {16, 14, 29, 4, 0, true, false},   //SIM2_CLK
    {17, 13, 28, 0, 0, true, false},   //SIM2_RST
    {18, 18, 33, 1, 0, true, false},   //UART1_RX
    {19, 19, 34, 1, 0, true, false},   //UART1_TX
    {20, 1,  16, 3, 0, true, false},   //MAIN_RTS
    {21, 2,  17, 3, 0, true, false},   //MAIN_CTS
    {38, 25, 50, 0, 0, true, true},   //LPG
    {39, 20, 45, 0, 0, true, true},   //RESERVED:GNSSEN
    {40, 23, 48, 0, 0, true, true},    //RESERVED
    {41, 22, 47, 0, 0, true, true},   //WAKEUP_IN
    {52, 19, 14, 4, 4, true, false},   //I2C_SDA0
    {53, 26, 51, 0, 0, true, true},   //RESERVED:WAKEUP BLE
    {54, 34, 40, 0, 0, true, false},   //LCD_CLK:GNSS UART TX
    {56, 18, 13, 4, 4, true, false},   //I2C_SCL0
    {58, 37, 43, 0, 0, true, false},   //LCD_SIO
    {61, 27, 52, 0, 0, true, true},   //WAKEUP_OUT
    {62, 38, 44, 0, 0, true, false},   //LCD_SDC
    {69, 35, 41, 0, 0, true, false},   //LCD_CS:GNSS UART RX
    {71, 0,  15, 0, 0, true, false},   //USB_BOOT
    {74, 15, 30, 0, 0, true, false},   //LCD_RST
    {75, 30, 36, 1, 0, true, false},   //I2S_LRCK
    {76, 29, 35, 1, 0, true, false},   //I2S_BCLK
    {77, 32, 38, 1, 0, true, false},   //I2S_DOUT
    {78, 31, 37, 1, 0, true, false},   //I2S_DIN
    {79, 5 , 20, 0, 0, true, false},   //CAM_PWDN
    {80, 3 , 18, 0, 0, true, false},   //CAM_MCLK
    {82, 11, 26, 0, 0, true, false},   //SPI_CLK
    {83, 10, 25, 0, 0, true, false},   //SPI_RXD
    {84, 9 , 24, 0, 0, true, false},   //SPI_TXD
    {85, 8 , 23, 0, 0, true, false},   //SPI_CS
    {87, 36, 42, 0, 0, true, false},   //RESERVED:GNSS RESET
    {88, 17, 12, 4, 4, true, false},   //RESERVED:GNSS BOOT
    {89, 7 , 22, 0, 0, true, false},   //CAM_SI1: BLE_UART_RX
    {90, 6 , 21, 0, 0, true, false},   //CAM_SI0: BLE_UART_TX
    {91, 16, 11, 4, 4, true, false},   //CAM_RST
    {92, 4 , 19, 0, 0, true, false},   //CAM_CLK
    {93, 17, 32, 1, 0, true, false},   //DEBUG_TXD
    {94, 16, 31, 1, 0, true, false},   //DEBUG_RXD
    {99, 24, 49, 0, 0, true, true},   //RESERVED:GNSS backup power
    {100,21, 46, 0, 0, true, true},   //RESERVED:CODEC power supply
    {101,33, 39, 1, 0, true, false},   //I2S_MCLK
};
const int32_t max_pin = 102; //max_pin = pin_map_t_max_pin_id + 1
const int32_t g_pin_table_size = (sizeof(g_pin_table) / sizeof(g_pin_table[0]));

//LE250 wakeup_pad resources
const ffw_wakeuppad_t g_wkepad_table[]=
{
    {98,     WAKEUP_PAD_0,   PadWakeup0_IRQn}, //BLE wakeup LE250
    {27,     WAKEUP_PAD_2,   PadWakeup2_IRQn}, //SIM1_DET
    {39,     WAKEUP_PAD_3,   PadWakeup3_IRQn}, //RESERVED
    {100,    WAKEUP_PAD_4,   PadWakeup4_IRQn}, //RESERVED
    {41,     WAKEUP_PAD_5,   PadWakeup5_IRQn}  //WAKEUP_IN
};
const int32_t g_wkepad_table_size = (sizeof(g_wkepad_table) / sizeof(g_wkepad_table[0]));

//LE250 AT+GPIO command resources
const uint32_t gpio_pin_table[] = {40};
const int32_t gpio_pin_table_size = (sizeof(gpio_pin_table) / sizeof(gpio_pin_table[0]));
const char *gpio_at_test = "+GPIO: (40),(0-2),(0,1)";

//LE250 LPG resources
const uint32_t FIBO_NETLPG_PIN_NUM = 38;
const uint32_t FIBO_SYSLPG_PIN_NUM = 0xFF;
//customer config UART_recv_task_stack_size
const uint32_t g_uart_recv_task_stack_cfg[]={
    16*1024,    //uart0 recvtask default stack
    16*1024,    //uart1 recvtask default stack
    16*1024,    //uart2 recvtask default stack
    16*1024     //uart3 recvtask default stack
};