/**
 * @file bot_system_utils.h
 * @brief System abstraction layer interface, used to connect to different OS
 * @details This file is a unified abstraction layer for different RTOS and must be adapted
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */

#ifndef __BOT_SYSTEM_UTILS_H__
#define __BOT_SYSTEM_UTILS_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "stdarg.h"
#include "stdint.h"
#include "stddef.h"

#define _IN_

/**
 * @brief Printf log output
 *
 * @param[in]  fmt format
 *
 * @return void
 */
void bot_printf(const char *fmt, ...);

/**
 * @brief Write to the character string
 *
 * @param[out]  str buff to store the data
 * @param[in]  len length of the buffer
 * @param[in]  fmt format
 *
 * @return the number of characters printed
 */
int bot_snprintf(_IN_ char *str, const int len, const char *fmt, ...);

/**
 * @brief Write to the character string
 *
 * @param[out]  str buff to store the data
 * @param[in]  len length of the buffer
 * @param[in]  fmt format
 * @param[in]  ap  variable argument lists
 *
 * @return the number of characters printed
 */

int bot_vsnprintf(_IN_ char *str, _IN_ const int len, _IN_ const char *fmt, va_list ap);

/**
 * @brief Get sys version, e.g fw_version, sdk version
 *
 * @return pointer to the buffer storing software version
 */
const char *bot_software_version_get(void);

/**
 * @brief Get hardware version, e.g mcu_version, soc_version
 *
 * @return pointer to the buffer hardware version
 */
const char *bot_hardware_version_get(void);

/**
 * @brief Generate Random number
 *
 * @param[in]  dest pointer to the buffer store random number
 * @param[in]  size the length to erase
 *
 * @return random number generation result
 * @retval 0 success
 * @retval others fail
 */
int bot_random_generate(uint8_t *dest, unsigned size);

#if defined(__cplusplus)
}
#endif

#endif /* __BOT_SYSTEM_UTILS_H__ */

