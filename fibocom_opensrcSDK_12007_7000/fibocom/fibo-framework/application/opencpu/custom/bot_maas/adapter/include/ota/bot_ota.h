/**
 * @file bot_ota.h
 * @brief bot ota component header file
 * @details This file defines the application interface of the bot ota component,
 * which is used to verify the ota function and can be adapted according to the actual situation
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */

#ifndef __BOT_OTA_H__
#define __BOT_OTA_H__

#include <stdint.h>
#include <stddef.h>
#if defined(__cplusplus)
extern "C" {
#endif

/** @addtogroup ota
 *  ota hal API.
 *
 *  @{
 */

/** @enum bot_ota_type_e
    @brief the protocol type to get firmware
*/
typedef enum {
    BOT_OTA_HTTP = 0,
    BOT_OTA_HTTPS,
    BOT_OTA_FTP
} bot_ota_type_e;

/** @struct bot_ota_para_t
    @brief ota parameter
*/
typedef struct {
    bot_ota_type_e type;
    uint8_t *url;
    uint8_t *filename;
    uint8_t *username;
    uint8_t *password;
} bot_ota_para_t;

/**
 * @brief app ota
 *
 * @param[in] para ota para
 * type. 0: http, 1: https, 2: ftp
 * url for pac downloading
 * filename only for ftp
 * username only for ftp
 * password only for ftp
 * @return ota process result
 * @retval 0 success
 * @retval others fail
 */
int bot_ota_appfota(bot_ota_para_t *para);

/** @} */

#if defined(__cplusplus)
}
#endif
#endif /* __BOT_OTA_H__ */