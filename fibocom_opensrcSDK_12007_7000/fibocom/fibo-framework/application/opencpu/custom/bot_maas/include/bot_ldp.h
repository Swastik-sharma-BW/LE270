/**
 * @file bot_ldp.h
 * @brief local differential privacy core api
 * @copyright Copyright (C) 2015-2024 Ant Group Holding Limited
 */

#ifndef __BOT_LDP_H__
#define __BOT_LDP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 宏开关 */
// #define LDP_ENABLE_MEMOIZATION  /* ldp记忆存储机制选择，开启使能memoization, 默认关闭 */

/* 宏定义 */
#define BOT_LDP_INFO_MAX_LEN                        256
#define BOT_LDP_OLH_DATA_MAX_LEN                    32
#define BOT_LDP_DEFAULT_DOUBLE_PRECISION            3
#define BOT_LDP_DEFAULT_DISCRETIZATION_PRECISION    1
#define BOT_LDP_OLH_DATA_FMT                        "%d,%d"
#define USER_LDP_ENABLE_NAME                        "CellLdpEnable"
#define USER_LDP_PUB_TEMPERATURE_FMT "{\"ldpTemperatureBit\":%d,  \
\"ldpTemperatureKrr\":%d,  \
\"ldpTemperatureOlh\":\"%s\"}"
#define USER_LDP_PUB_VOLTAGE_FMT "{\"ldpVoltageBit\":%d,  \
\"ldpVoltageKrr\":%d,  \
\"ldpVoltageOlh\":\"%s\"}"

typedef enum {
    LDP_TEMPERATURE,
    LDP_VOLTAGE,
    LDP_FLAG_MAX,
} ldp_data_flag_e;

typedef enum {
    LDP_INT,
    LDP_DOUBLE,
    LDP_TYPE_MAX,
} ldp_data_type_e;

typedef enum {
    LDP_HIGH_PRIVACY,
    LDP_MEDIUM_PRIVACY,
    LDP_LOW_PRIVACY,
    LDP_WRONG_PRIVACY,
} ldp_privacy_strength_e;

typedef union {
    int32_t int_val;
    double  double_val;
} ldp_value_u;

typedef struct {
    ldp_data_flag_e data_flag;
    ldp_data_type_e data_type;
    ldp_value_u min_val;
    ldp_value_u max_val;
    ldp_privacy_strength_e epsilon;
} ldp_para_t;

typedef struct {
    ldp_para_t para;
    ldp_value_u data;
} ldp_user_set_t;

#ifndef LDP_ENABLE_MEMOIZATION
typedef struct {
    ldp_data_flag_e data_flag;
    bool double_flag;
    uint32_t double_bits;
    int32_t min_val;
    int32_t max_val;
    uint32_t inter_width;
    double epsilon;
} ldp_config_t;
#else
typedef struct {
    ldp_data_flag_e data_flag;
    bool double_flag;
    uint32_t double_bits;
    int32_t min_val;
    int32_t max_val;
    uint32_t inter_width;
    double epsilon;
    int32_t *memo_bit;
    int32_t *memo_krr;
    bool memo_flag;
} ldp_config_t;
#endif

typedef void *ldp_handle_t;

typedef struct {
    int32_t data;
    uint32_t seed;
} olh_data_t;

/**
 * @brief create ldp handle according to user parameter;
 * @param[in] para ldp parameters;
 * @return ldp handle;
*/
ldp_handle_t bot_ldp_init(ldp_para_t *para);

/**
 * @brief destroy ldp handle;
 * @param[in] handle ldp handle;
*/
void bot_ldp_deinit(ldp_handle_t *handle);

/**
 * @brief ldp mean estimation interface (1BIT algorithm);
 * @param[in] handle ldp handle;
 * @param[in] in input data;
 * @param[in] out ldp data;
 * @return result of ldp process;
 * @retval 0, success;
 * @retval -1, failed;
*/
int32_t bot_ldp_mean_bit(ldp_handle_t handle, ldp_value_u *in, ldp_value_u *out);

/**
 * @brief ldp frequency estimation interface (KRR algorithm);
 * @param[in] handle ldp handle;
 * @param[in] in input data;
 * @param[in] out ldp data;
 * @return result of ldp process;
 * @retval 0, success;
 * @retval -1, failed;
*/
int32_t bot_ldp_freq_krr(ldp_handle_t handle, ldp_value_u *in, ldp_value_u *out);

/**
 * @brief ldp frequency estimation interface (OLH algorithm);
 * @param[in] handle ldp handle;
 * @param[in] in input data;
 * @param[in] out ldp data;
 * @return result of ldp process;
 * @retval 0, success;
 * @retval -1, failed;
*/
int32_t bot_ldp_freq_olh(ldp_handle_t handle, ldp_value_u *in, olh_data_t *out);

/**
 * @brief create and pack the ldp information by executing the above api;
 * @param[in] ldp_info ldp information;
 * @param[in] info_len length of ldp information;
 * @param[in] user_set user setting of ldp parameter;
 * @return result of ldp info get;
 * @retval 0, success;
 * @retval -1, failed;
*/
int32_t bot_ldp_info_get(char *ldp_info, uint32_t info_len, ldp_user_set_t *user_set);

#ifdef __cplusplus
}
#endif

#endif  /* __BOT_LDP_H__ */
