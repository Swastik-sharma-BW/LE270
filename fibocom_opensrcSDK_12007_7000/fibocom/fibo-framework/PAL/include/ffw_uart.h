#ifndef DC6FC293_058C_406F_B473_1170B51410F4
#define DC6FC293_058C_406F_B473_1170B51410F4

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ffw_os.h"
#if defined(FIBOCOM_OPENCPU)
#include "fibo_opencpu_comm.h"
#endif

/// type define struct ffw_uart_config_s
typedef struct ffw_uart_config_s ffw_uart_config_t;

/// type define enum ffw_uart_data_bits_s
typedef enum ffw_uart_data_bits_s ffw_uart_data_bits_t;

/// type define enum racon_uart_stop_bits_s
typedef enum ffw_uart_stop_bits_s ffw_uart_stop_bits_t;

/// rx buffer size
#define UART_RX_BUF_SIZE (4 * 1024)

/// tx buffer size
#define UART_TX_BUF_SIZE (4 * 1024)

/**
* @brief Callback for recv uart data
* 
 */
typedef void (*ffw_uart_input_callback_t)(uint8_t uart_port, uint8_t *data, uint16_t len, void *arg);

/// data bit define
enum ffw_uart_data_bits_s
{
    FFW_UART_DATA_BITS_7,
    FFW_UART_DATA_BITS_8
};

/// stop bit define
enum ffw_uart_stop_bits_s
{
    FFW_UART_STOP_BITS_1,
    FFW_UART_STOP_BITS_2
};

/// parity bit define
typedef enum
{
    FFW_UART_NO_PARITY,   ///< No parity check
    FFW_UART_ODD_PARITY,  ///< Parity check is odd
    FFW_UART_EVEN_PARITY ///< Parity check is even
} ffw_uart_parity_t;

/**
 * @brief 串口配置定义
 * 
 */
struct ffw_uart_config_s
{
    /// baudrate
    uint32_t baud; 

    /// data bits                    
    ffw_uart_data_bits_t data_bits; 

    /// stop bits
    ffw_uart_stop_bits_t stop_bits;

    /// parity
    ffw_uart_parity_t parity;

    /// < enable cts or not
    bool cts_enable;

    /// < enable rts or not
    bool rts_enable;

    /// < rx buffer size
    size_t rx_buf_size;

    /// < tx buffer size
    size_t tx_buf_size;

    /// recv data timeout in ms
    uint32_t recv_timeout;
};

/**
 * @brief 打开一个串口，此时接收数据通过callback方式通知给用户
 * 
 * @param uart_port 串口号
 * @param uart_config 串口配置
 * @param recv_cb 接收数据回调函数
 * @param arg 回调上下文
 * @return int 
 * @retval FFW_R_SUCCESS: 成功
 * @retval FFW_R_FAILED: 失败
 */
int ffw_uart_init(uint8_t uart_port, ffw_uart_config_t *uart_config, ffw_uart_input_callback_t recv_cb, void *arg);


/**
 * @brief 打开一个串口，此时接收数据通过 @c ffw_uart_get 获取
 * 
 * @param uart_port 串口号
 * @param uart_config 串口配置
 * @return int 
 * @retval FFW_R_SUCCESS: 成功
 * @retval FFW_R_FAILED: 失败
 */
int ffw_uart_init1(uint8_t uart_port, ffw_uart_config_t *uart_config);

/**
* @brief 串口空闲状态
* 
* @param uart_port 串口号
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败
*/
int ffw_uart_data_to_send(int uart_port);

/**
 * @brief 从串口发送数据
 * 
 * @param uart_port 串口号
 * @param data 数据
 * @param len 数据长度
 * @return int 已发送的数据长度
 * @retval -1 发送失败
 */
int ffw_uart_put(uint8_t uart_port, const uint8_t *data, uint32_t len);

/**
 * @brief 关闭串口
 * 
 * @param uart_port 串口号
 * @return int 
 * @retval FFW_R_SUCCESS: 成功
 * @retval FFW_R_FAILED: 失败
 */

int ffw_uart_deinit(uint8_t uart_port);

/**
 * @brief 进入cmux模式
 * 
 * @param uart_port 
 * @return int 
 */
int ffw_uart_enter_cmux(uint8_t uart_port);

/**
 * @brief 退出cmux模式
 * 
 * @param uart_port 
 * @return int 
 */
int ffw_uart_exit_cmux(uint8_t uart_port);

#endif /* DC6FC293_058C_406F_B473_1170B51410F4 */
