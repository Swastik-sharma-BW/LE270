#ifndef _BOT_NET_ACCESS_GEN_H_
#define _BOT_NET_ACCESS_GEN_H_

/** @enum bot_sim_state_t
    @brief sim state
*/
typedef enum {
    BOT_NO_SIM,             ///< no sim card
    BOT_SIM_READY,          ///< sim ready
} bot_sim_state_t;

/** @enum bot_nwk_reg_state_t
    @brief network registration status
*/
typedef enum {
    BOT_NWK_REG_STATE_NOT_REGISTERED = 0,   ///< not registered, no registered operators are found
    BOT_NWK_REG_STATE_HOME_NETWORK = 1,     ///< registered, home network
    BOT_NWK_REG_STATE_SEARCHING = 2,        ///< not registered, but attempting to attach or search a network to register
    BOT_NWK_REG_STATE_DENIED = 3,           ///< registration denied
    BOT_NWK_REG_STATE_UNKNOWN = 4,          ///< unknown
    BOT_NWK_REG_STATE_ROAMING = 5,          ///< registered, roaming
} bot_nwk_reg_state_t;

typedef enum {
    BOT_PDP_IP_TYPE     = 0, ///< "IP"
    BOT_PDP_IPV6_TYPE   = 1, ///< "IPV6"
    BOT_PDP_IPV4V6_TYPE = 2, ///< "IPV4V6"
    BOT_PDP_PPP_TYPE    = 3, ///< "PPP"
    BOT_PDP_NONIP_TYPE  = 4, ///< "Non-IP"
    BOT_PDP_TYPE_MAX,        ///< Invalid
} bot_pdp_type_t;

/**
 * @brief get maas sdk pdp context id.
 *
 * @return maas sdk pdp context id
 *
 */
int bot_cellular_bot_cid_get(void);

/**
 * @brief get cellular sim card state.
 * @note  same as AT+CPIN?
 *
 * @param[out]  state: pointe to sim state
 *
 * @return get sim state result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_cellular_sim_state_get(bot_sim_state_t *state);

/**
 * @brief cellular EPS network registration status.
 * @note  same as AT+CEREG?
 *
 * @param[in]  cid: pdp context id
 *
 * @return register status get result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_cellular_register_status_get(bot_nwk_reg_state_t *state);

/**
 * @brief get cellular pdp context status.
 * @note  same as AT+CGACT?
 *
 * @param[in]  cid:    pdp context id
 * @param[out] state:  pointe to pdp context state
 *
 * @return get pdp status result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_cellular_pdp_status_get(int cid, int *state);


/**
 * @brief cellular pdp context active.
 * @note  function will fallow steps：
 *        1. judge whether the pdp context has been activated.        same as AT+CGACT?
 *        2. if not, set the APN first and activate the pdp context.  same as AT+CGDCONT=<cid>,<PDP_type>,<APN> and AT+CGACT=<state>,<cid>
 *        3. if has been activated, success is returned.
 *
 * @param[in]  cid: pdp context id
 * @param[in]  apn: NULL is passed by default
 * @param[in]  type: ip type
 *
 * @return pdp active result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_cellular_pdp_active(int cid, char *apn, bot_pdp_type_t type);

/**
 * @brief cellular pdp context deactive.
 * @note  function will fallow steps：
 *        1. judge whether the pdp context has been activated. same as AT+CGACT?
 *        2. if has been activated, success is returned.       same as AT+CGACT=<state>,<cid>
 *        3. if not, success is returned.
 *
 * @param[in]  cid: pdp context id
 *
 * @return pdp deactive result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_cellular_pdp_deactive(int cid);

/**
 * @brief bot_net_access_init
 * @note  General Network Activation
 *
 * @return network active result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_net_access_init(void);


/**
 * @brief bot_net_access_deinit
 * @note  General network deactivation
 * 
 * @return network deactive result
 * @retval 0 success
 * @retval others fail
 *
 */
int bot_net_access_deinit(void);
#endif