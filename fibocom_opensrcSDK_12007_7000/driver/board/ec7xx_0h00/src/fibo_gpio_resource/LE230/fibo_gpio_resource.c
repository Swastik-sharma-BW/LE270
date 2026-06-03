/******************************************************************************

*(C) Copyright 2024 Fibocom International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description: LE230 GPIO ADC LPM LPG Resources
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

//LE230 adc resources
const ffw_device_adc_table device_adc_table[] =
{
    {0, ADC_CHANNEL_AIO1},
    {1, ADC_CHANNEL_AIO2},
    {2, ADC_CHANNEL_AIO3}
};
const int32_t device_adc_size = (sizeof(device_adc_table) / sizeof(device_adc_table[0]));
const char *mmad_at_test = "+MMAD: (0-2),(0-3)";

//LE230 lpm wakeup host resources
const ffw_pal_pm_wake_host_cfg_table pal_pm_wake_host_cfg_table[] =
{
    {0, 40},
    {1, 0xFF},
    {2, 40}
};
const char *gtwake_at_test = "+GTWAKE: (0,2),(0,1)";

//LE230 lpm resources
const ffw_pal_pm_sleep_cfg_table pal_pm_sleep_cfg_table[] = {
    {0,     0xFF,   0xFF,           0xFF},
    {1,     49,     WAKEUP_PAD_0,   PadWakeup0_IRQn},
    {2,     39,     WAKEUP_PAD_5,   PadWakeup5_IRQn}
};
const char *gtlpmmode_at_test = "+GTLPMMODE: (0-2),(0,1)";

//LE230 gpio resources
pin_map_t g_pin_table[] =
{
    {15, 7,  22, 1, 0, true, false},   //CAM_SPI_DATA1
    {0,  23, 48, 0, 0, true, true},    //AGPIO3 VDD_EXT POWER
    {1,  11, 26, 1, 0, true, false},   //SPI_CLK
    {2,  10, 25, 1, 0, true, false},   //SPI_RXD
    {3,  9,  24, 1, 0, true, false},   //SPI_TXD
    {4,  8,  23, 1, 0, true, false},   //SPI_CS
    {10, 3,  18, 1, 0, true, false},   //CAM_MCLK
    {11, 18, 13, 2, 4, true, false},   //CAM_I2C_SCL
    {12, 19, 14, 2, 4, true, false},   //CAM_I2C_SDA
    {13, 4 , 19, 1, 0, true, false},   //CAM_SPI_CLK
    {14, 6 , 21, 1, 0, true, false},   //CAM_SPI_DATA0
    {16, 5,  20, 1, 0, true, false},   //CAM_PWDN
    {31, 18, 33, 1, 0, true, false},   //MAIN_RXD
    {32, 19, 34, 1, 0, true, false},   //MAIN_TXD
    {33, 1,  16, 3, 0, true, false},   //MAIN_RTS
    {34, 2,  17, 3, 0, true, false},   //MAIN_CTS
    {39, 22, 47, 0, 0, true, true},   //MAIN_DTR
    {40, 27, 52, 0, 0, true, true},   //MAIN_RI
    {48, 17, 12, 4, 4, true, false},   //MAIN_DCD
    {51, 21, 46, 0, 0, true, true},   //W_DISABLE
    {53, 26, 51, 0, 0, true, true},   //SLEEP_IND
    {54, 25, 50, 0, 0, true, true},   //NET_STATUS
    {55, 0 , 15, 0, 0, true, false},   //USB_BOOT
    {56, 8,  23, 2, 0, true, false},   //I2C_SDA
    {57, 9,  24, 2, 0, true, false},   //I2C_SCL
    {58, 30, 36, 1, 0, true, false},   //IIS_LRCLK
    {59, 31, 37, 1, 0, true, false},   //IIS_DI
    {60, 32, 38, 1, 0, true, false},   //IIS_DOUT
    {61, 29, 35, 1, 0, true, false},   //IIS_BCLK
    {62, 36, 42, 1, 0, true, false},   //LCD_FMARK
    {63, 38, 44, 1, 0, true, false},   //LCD_SPI_SDC
    {64, 15, 30, 4, 0, true, false},   //LCD_SPI_RST
    {65, 35, 41, 1, 0, true, false},   //LCD_SPI_CS
    {66, 37, 43, 1, 0, true, false},   //LCD_SPI_SIO
    {67, 34, 40, 1, 0, true, false},   //LCD_SPI_CLK
    {69, 33, 39, 1, 0, true, false},   //IIS_MCLK
    {70, 20, 45, 0, 0, true, true},   //USIM2_DET
    {71, 17, 32, 1, 0, true, false},   //DBG_TXD
    {72, 16, 31, 1, 0, true, false},   //DBG_RXD
    {81, 23, 48, 0, 0, true, true},    //AGPIO3: resve VDD_EXT POWER, Internal use
    {100,24, 49, 0, 0, true, true},   //RESERVED:CODEC power supply
    {120,16, 11, 4, 4, true, false},   //CAM_RST
    {123,10, 25, 3, 0, true, false},   //UART2_RXD
    {124,11, 26, 3, 0, true, false},   //UART2_TXD
    {145,13, 28, 4, 0, true, false},   //USIM2_RST
    {146,12, 27, 4, 0, true, false},   //USIM2_DATA
    {147,14, 29, 4, 0, true, false},   //USIM2_CLK
};
const int32_t max_pin = 148; //max_pin = pin_map_t_max_pin_id + 1
const int32_t g_pin_table_size = (sizeof(g_pin_table) / sizeof(g_pin_table[0]));

//LE230 wakeup_pad resources
const ffw_wakeuppad_t g_wkepad_table[]=
{
    {49,      WAKEUP_PAD_0,   PadWakeup0_IRQn}, //WAKEUP_IN
    {9 ,      WAKEUP_PAD_2,   PadWakeup2_IRQn}, //USIM_DET
    {70,      WAKEUP_PAD_3,   PadWakeup3_IRQn}, //USIM2_DET
    {51,      WAKEUP_PAD_4,   PadWakeup4_IRQn}, //W_DISABLE
    {39,      WAKEUP_PAD_5,   PadWakeup5_IRQn}  //MAIN_DTR
};
const int32_t g_wkepad_table_size = (sizeof(g_wkepad_table) / sizeof(g_wkepad_table[0]));

//LE230 AT+GPIO command resources
const uint32_t gpio_pin_table[] = {};
const int32_t gpio_pin_table_size = (sizeof(gpio_pin_table) / sizeof(gpio_pin_table[0]));
const char *gpio_at_test = "+GPIO: (),(0-2),(0,1)";

//LE230 LPG resources
const uint32_t FIBO_NETLPG_PIN_NUM = 54;
const uint32_t FIBO_SYSLPG_PIN_NUM = 0xFF;
//customer config UART_recv_task_stack_size
const uint32_t g_uart_recv_task_stack_cfg[]={
    16*1024,    //uart0 recvtask default stack
    16*1024,    //uart1 recvtask default stack
    16*1024,    //uart2 recvtask default stack
    16*1024     //uart3 recvtask default stack
};
