/**
 * @file bot_hal_at.h
 * @brief bot virtual at component header file
 * @details The interface in this file is used for debugging and adapted according to the actual situation
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */
#ifndef __BOT_HAL_AT_H__
#define __BOT_HAL_AT_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup virtual_at
 *  virtual at API.
 *
 *  @{
 */

#include <stdint.h>
#include "bot_system.h"

/**
 * @brief Virtual AT command data processing function pointer
 *
 * @param[in]  data virtual at receive data
 * @param[in]  len virtual at receive data len
 *
 * @return void
 *
 */
typedef void (*bot_at_rx_cb)(unsigned char *data, unsigned int len);

/**
 * @brief Send virtual at cmd to kernel
 *
 * @param[in]  cmd  at command string
 * @param[in]  len  length of at command string
 *
 * @return hal at send result
 * @retval when success, return the length of cmd data actually written
 * @retval others fail
 */
int bot_hal_at_send(unsigned char *cmd, unsigned int len);

/**
 * @brief Register virtual at command to return data processing function
 * @note This function does not need to be adapted
 * virtual at initialization for some platforms can be implemented in the interface
 * @param[in]  cb  pointer to data processing function
 *
 * @return Callback function registration result
 * @retval 0 success
 * @retval others fail
 */
int bot_hal_at_cb_register(bot_at_rx_cb cb);

/**
 * @brief virtual at command rx data callback
 * @note This function does not need to be adapted,
 * it only needs to be called when the virtual AT receives data
 * @param[in]  data  received data
 * @param[in]  len   length of received data
 *
 * @return The execution result of the callback function registered by bot_hal_at_cb_register
 * @retval 0 success
 * @retval others fail
 */
int bot_hal_at_callback(unsigned char *data, unsigned int len);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __BOT_HAL_AT_H__ */

