/**
 * @file bot_hal_uart.h
 * @brief bot uart component header file
 * @details The interface in this file is used for debugging and adapted according to the actual situation
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */
#ifndef __BOT_HAL_UART_H__
#define __BOT_HAL_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup uart
 *  uart hal API.
 *
 *  @{
 */

#include <stdint.h>
#include "bot_system.h"

/** @def HAL_WAIT_FOREVER
    @brief Define the wait forever timeout macro
*/
#define HAL_WAIT_FOREVER 0xFFFFFFFFU

/** @enum bot_uart_logic_port_e
    @brief UART logic port
*/
typedef enum {
    UART_LOGIC_PORT0 = 0,
    UART_LOGIC_PORT1 = 1,
    UART_LOGIC_PORT2 = 2,
    UART_LOGIC_PORT_MAX
} bot_uart_logic_port_e;

/** @enum bot_uart_data_width_e
    @brief UART data width
*/
typedef enum {
    DATA_WIDTH_5BIT,
    DATA_WIDTH_6BIT,
    DATA_WIDTH_7BIT,
    DATA_WIDTH_8BIT,
    DATA_WIDTH_9BIT
} bot_uart_data_width_e;

/** @enum bot_uart_stop_bits_e
    @brief UART stop bits
*/
typedef enum {
    STOP_BITS_1,
    STOP_BITS_2
} bot_uart_stop_bits_e;

/** @enum bot_uart_flow_control_e
    @brief UART flow control
*/
typedef enum {
    FLOW_CONTROL_DISABLED,  /**< Flow control disabled */
    FLOW_CONTROL_CTS,       /**< Clear to send, yet to send data */
    FLOW_CONTROL_RTS,       /**< Require to send, yet to receive data */
    FLOW_CONTROL_CTS_RTS    /**< Both CTS and RTS flow control */
} bot_uart_flow_control_e;

/** @enum bot_uart_parity_e
    @brief UART parity
*/
typedef enum {
    NO_PARITY,      /**< No parity check */
    ODD_PARITY,     /**< Odd parity check */
    EVEN_PARITY     /**< Even parity check */
} bot_uart_parity_e;

/** @enum bot_uart_mode_e
    @brief UART mode
*/
typedef enum {
    MODE_TX,        /**< Uart in send mode */
    MODE_RX,        /**< Uart in receive mode */
    MODE_TX_RX      /**< Uart in send and receive mode */
} bot_uart_mode_e;

/** @struct bot_uart_config_t
    @brief UART configuration
*/
typedef struct {
    unsigned int            baud_rate;      /**< Uart baud rate */
    bot_uart_data_width_e   data_width;     /**< Uart data width */
    bot_uart_parity_e       parity;         /**< Uart parity check mode */
    bot_uart_stop_bits_e    stop_bits;      /**< Uart stop bit mode */
    bot_uart_flow_control_e flow_control;   /**< Uart flow control mode */
    bot_uart_mode_e         mode;           /**< Uart send/receive mode */
} bot_uart_config_t;

/** @struct bot_uart_dev_t
    @brief UART dev handle
*/
typedef struct {
    bot_uart_logic_port_e   port;   /**< uart port */
    bot_uart_config_t       config; /**< uart config */
    void                    *priv;  /**< priv data */
} bot_uart_dev_t;

/**
 * @brief UART data receive callback
 *
 * @param[in]  bot_uart_logic_port_e logic uart port
 * @param[in]  data receive data
 * @param[in]  len receive data len
 * @param[in]  arg reserve
 *
 * @return void
 *
 */
typedef void (*bot_uart_rx_cb)(bot_uart_logic_port_e port, void *data, unsigned int len, void *arg);


/**
 * @brief Initialises a UART interface
 *
 * @param[in]  uart  uart handle
 *
 * @return uart init result
 * @retval 0 success
 * @retval others fail
 */
int bot_hal_uart_init(bot_uart_dev_t *uart);

/**
 * @brief Transmit data on a UART interface
 *
 * @param[in]  uart     uart handle
 * @param[in]  data     pointer to the start of data
 * @param[in]  size     number of bytes to transmit
 * @param[in]  timeout  timeout in miliseconds, set this value to HAL_WAIT_FOREVER
 *                      if you want to wait forever
 *
 * @return uart send data result
 * @retval 0 success
 * @retval others fail
 */
int bot_hal_uart_send(bot_uart_dev_t *uart, const void *data, unsigned int size, unsigned int timeout);

/**
 * @brief Receive data on a UART interface
 *
 * @param[in]   uart         uart handle
 * @param[out]  data         pointer to the buffer which will store incoming data
 * @param[in]   expect_size  number of bytes to receive
 * @param[out]  recv_size    number of bytes trully received
 * @param[in]   timeout      timeout in miliseconds, set this value to HAL_WAIT_FOREVER
 *                           if you want to wait forever
 *
 * @return uart data receiving result
 * @retval 0 success
 * @retval others fail
 */
int bot_hal_uart_recv(bot_uart_dev_t *uart, void *data, unsigned int expect_size,
                      unsigned int *recv_size, unsigned int timeout);

/**
 * @brief Register UART data receive call back
 *
 * @param [in]   uart         uart handle
 * @param [in]   cb           Non-zero pointer is the rx callback handler;
 *                            NULL pointer for rx_cb unregister operation
 *                            uart in rx_cb must be the same pointer with uart pointer passed to hal_uart_recv_cb_reg
 *                            driver must notify upper layer by calling cb if data is available in UART's hw or rx buffer
 * @param [in]   args         user args for rx_cb
 *
 * @return uart data receiving result
 * @retval 0 success
 * @retval others fail
 */
int bot_hal_uart_recv_cb_register(bot_uart_dev_t *uart, bot_uart_rx_cb cb, void *args);

/**
 * @brief Deinitialises a UART interface
 *
 * @param[in]  uart  uart handle
 *
 * @return uart deinit result
 * @retval 0 success
 * @retval others fail
 */
int bot_hal_uart_deinit(bot_uart_dev_t *uart);


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __BOT_HAL_UART_H__ */

