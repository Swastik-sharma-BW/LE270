/**
 * @file bot_network.h
 * @brief bot network component header file
 * @details This file defines the application interface of the bot network component,
 * which depends on bot_net_cellular.h and does not need to be adapted
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */

#ifndef __BOT_NETWORK_H__
#define __BOT_NETWORK_H__

#include <stdint.h>
#include <stddef.h>
#if defined(__cplusplus)
extern "C" {
#endif

#define BOT_NETWORK_MAC_LEN   16

/** @addtogroup network
 *  network hal API.
 *
 *  @{
 */

/** @enum bot_uid_type_e
    @brief device register uid type
*/
typedef enum {
    BOT_DEV_UID_IMEI = 0,
    BOT_DEV_UID_MAC,
    BOT_DEV_UID_SN,
    BOT_DEV_UID_INVALID
} bot_uid_type_e;

/** @enum bot_net_type_e
    @brief network access type
*/
typedef enum {
    BOT_NET_TYPE_ETHERNET = 0,
    BOT_NET_TYPE_WIFI,
    BOT_NET_TYPE_CELLULAR_CAT1,
    BOT_NET_TYPE_CELLULAR_CAT4,
    BOT_NET_TYPE_CELLULAR_NB,
    BOT_NET_TYPE_BLUETOOTH,
    BOT_NET_TYPE_HYBRID,
    BOT_NET_TYPE_INVALID
} bot_net_type_e;

/** @enum bot_net_status_e
    @brief network status
*/
typedef enum {
    BOT_NET_STATUS_DISCONNECT = 0,
    BOT_NET_STATUS_CONNECTED,
    BOT_NET_STATUS_RECONNECTED,
    BOT_NET_STATUS_INVALID
} bot_net_status_e;

/** @struct bot_sim_info_t
    @brief sim info
*/
typedef struct {
    char imsi[32];
    char iccid[32];
    char apn[40];
} bot_sim_info_t;

/** @struct bot_net_signal_t
    @brief network signal
*/
typedef struct {
    int rssi;
    int rsrp;
    int rsrq;
    int csq;
} bot_net_signal_t;

/** @struct bot_cell_info_t
    @brief cell info
*/
typedef struct {
    int mcc;
    int mnc;
    int cellid;
    int lac;
    bot_net_signal_t signal;
} bot_cell_info_t;

/**
 * @brief get device uid type as device register name
 *
 * @return uid type
 */
bot_uid_type_e bot_dev_uid_type(void);

/**
 * @brief get imei information
 *
 * @param[out] imei information
 * @return get imei result
 * @retval 0 success
 * @retval others fail
 */
int bot_dev_imei_get(char *imei);

/**
 * @brief get mac addr information
 *
 * @param[out]  mac information
 * @return get mac addr info result
 * @retval 0 success
 * @retval others fail
 */
int bot_dev_mac_get(char mac[BOT_NETWORK_MAC_LEN]);

/**
 * @brief get sn information
 *
 * @param[out]  sn information
 * @return get sn info result
 * @retval 0 success
 * @retval others fail
 */
int bot_dev_sn_get(char *sn);

/**
 * @brief get model information
 *
 * @param[out]  model information
 * @return get model info result
 * @retval 0 success
 * @retval others fail
 */
#if BOT_AT_EXTEND
const char * bot_dev_model_get(void);
#endif

/**
 * @brief Network status change callback function
 *
 * @param[in]  bot_net_type_e Network type
 * @param[in]  bot_net_status_e Network status
 * @param[in]  void* reserve
 *
 * @return reserve
 *
 */
typedef int (*bot_net_event_callback_t)(bot_net_type_e, bot_net_status_e, void *);

/**
 * @brief netowrk init. It must be first called before calling other network function.
 *
 * @param[in]  callback Network state change callback function
 *
 * @return network init result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_network_init(bot_net_event_callback_t callback);

/**
 * @brief network deinit.
 *
 * @return network deinit result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_network_deinit(void);

/**
 * @brief get network type
 *
 * @return net type
 */
bot_net_type_e bot_network_type(void);

/**
 * @brief get network status
 *
 * @return net status
 */
bot_net_status_e bot_network_status(void);

/**
 * @brief get network signal
 *
 * @param[out]  signal  received signal information
 * @return get signal result
 * @retval 0 success
 * @retval others fail
 */
int bot_network_signal_get(bot_net_signal_t *signal);

/**
 * @brief get sim information
 *
 * @param[out]  sim_info sim information
 * @return get sim info result
 * @retval 0 success
 * @retval others fail
 */
int bot_network_sim_info_get(bot_sim_info_t *sim_info);

/**
 * @brief get cell information, if the interface is implemented in blocking mode, the call time must be evaluated!
 *
 * @param[out]  cell_info cell information
 * @return get cell info result
 * @retval 0 success
 * @retval others fail
 */
int bot_network_cell_info_get(bot_cell_info_t *cell_info);

/** @} */

#if defined(__cplusplus)
}
#endif
#endif /* __BOT_NETWORK_H__ */

