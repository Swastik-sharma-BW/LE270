#ifndef __OC_UART_H__
#define __OC_UART_H__
#include "ffw_uart.h"

typedef uint8_t hal_uart_port_t;
typedef struct ffw_uart_config_s ffw_uart_config_t;

typedef enum hal_uart_data_bits_s hal_uart_data_bits_t;
typedef enum hal_uart_stop_bits_s hal_uart_stop_bits_t;
enum hal_uart_data_bits_s
{
    HAL_UART_DATA_BITS_7 = FFW_UART_DATA_BITS_7,
    HAL_UART_DATA_BITS_8 = FFW_UART_DATA_BITS_8
};
enum hal_uart_stop_bits_s
{
    HAL_UART_STOP_BITS_1 = FFW_UART_STOP_BITS_1,
    HAL_UART_STOP_BITS_2 = FFW_UART_STOP_BITS_2
};
typedef enum
{
    HAL_UART_NO_PARITY = FFW_UART_NO_PARITY,   ///< No parity check
    HAL_UART_ODD_PARITY = FFW_UART_ODD_PARITY,  ///< Parity check is odd
    HAL_UART_EVEN_PARITY = FFW_UART_EVEN_PARITY, ///< Parity check is even
}hal_uart_parity_t;

typedef struct {
   uint32_t baud;      //波特率  (1200,2400,4800,9600,19200,38400,57600,115200,230400,460800,921600,1000000,1500000,2000000)
   hal_uart_data_bits_t data_bits; //数据位 7位数据位：HAL_UART_DATA_BITS_7 8位数据位：HAL_UART_DATA_BITS_8
   hal_uart_stop_bits_t stop_bits; //停止位 1位停止位：HAL_UART_STOP_BITS_1 2位停止位：HAL_UART_STOP_BITS_2
   hal_uart_parity_t parity; //校验模式 无校验0:HAL_UART_NO_PARITY 奇校验1:HAL_UART_ODD_PARITY 偶校验2:HAL_UART_EVEN_PARITY
   bool cts_enable;         //cts配置 1：使能 0：不使能
   bool rts_enable;         //rts配置 1：使能 0：不使能
   size_t rx_buf_size;      //接收缓存大小 UART_RX_BUF_SIZE （2*1024）
   size_t tx_buf_size;      //发送缓存大小 UART_TX_BUF_SIZE （2*1024）
   uint32_t recv_timeout;   //接收超时上报时间(毫秒)
}hal_uart_config_t;

/**
* @brief Callback for recv uart data
 */
typedef void (*uart_input_callback_t)(hal_uart_port_t uart_port, UINT8 *data, UINT16 len, void *arg);

/**
* @brief
*
* @param
* @param
* @return
 */
INT32 fibo_hal_uart_init(hal_uart_port_t uart_port, hal_uart_config_t *uart_config, uart_input_callback_t recv_cb, void *arg);

/**
* @brief
*
* @param
* @param
* @return
 */
INT32 fibo_hal_uart_deinit(hal_uart_port_t uart_port);

/**
* @brief
*
* @param
* @param
* @return
 */
INT32 fibo_hal_uart_put(hal_uart_port_t uart_port, UINT8 *buff, UINT32 len);

INT32 fibo_hal_uart_data_to_send(int uart_port);


#endif
