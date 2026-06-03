/**
 * @file bot_app_entry.h
 * @brief 应用示例代码头文件
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */

#ifndef __BOT_APP_ENTRY_H__
#define __BOT_APP_ENTRY_H__
#if defined(__cplusplus)
extern "C" {
#endif

#include "bot_api.h"
#include "bot_default_config.h"
#include "bot_ldp.h"

/* 测试使用的配置Token，量产Token请找蚂蚁同学获取 */
#if BOT_GATEWAY
#define BOT_CONFIG_TOKEN    "gw_rawdata&RZbmD9CH921l5tTnE7bzsiW606eQejWXzC8GX2RdRLbEWPE1QSLVB4ErebiKQBGNdeAlJ1CqFhel6IyQ0fNLWP8="
#elif BOT_SERVICE_COAP      //可信平台-测试环境
#define BOT_CONFIG_TOKEN    "iBot-CoAP&RUrrp0/SrhCDCVZL92ERZF/xMnYfzJF8dp6WLypKGNUrdswohWb/CWKIkR2hFTHXSw=="
#else
#define BOT_CONFIG_TOKEN    "iBot-rawData&RUBDPmYzoj2ftkNLEUKBbLjYUNoVHtFZUj0fKTgF8fpkXZbQcbOaNkFuL69rPZ3M0g=="
#endif

/* 资产数据字段版本号(assetDataVersion)由蚂蚁定义，默认值为"ADV1.0"，无需修改 */
#define BOT_ASSET_DATA_VERSION         "ADV1.0"

/* 资产编号最大长度 */
#define BOT_ASSET_ID_MAX_SIZE          64
/* 资产类型最大长度 */
#define BOT_ASSET_TYPE_MAX_SIZE        64
/* 资产数据版本最大长度 */
#define BOT_ASSET_ADV_MAX_SIZE         16
/* 业务数据包最大长度 */
#define BOT_USER_DATA_STRING_MAX_SIZE  BOT_MAX_DATA_PACKET_LEN

/* 资产注册响应超时时间 （单位秒）*/
#define BOT_REG_WAIT_TIMEOUT_S         (2 * 60)
/* 资产注册失败最大重试次数 */
#define BOT_REG_RETRY_COUNT_MAX        (3)
/* 资产注册失败重试间隔 （单位秒）*/
#define BOT_REG_RETRY_INTERVAL_S       (10)
/* 资产注册异常重试间隔，异常原因如下
 * 1. 注册失败超过最大重试次数
 * 2. 注册参数错误
*/
#define BOT_CHAIN_INVALID_RETRY_S      (30 * 60)


/* 资产数据周期上报间隔 （单位秒）*/
#define BOT_PUB_INTERVAL_S (20)


/* 资产配置数据结构 */
typedef struct {
    char id[BOT_ASSET_ID_MAX_SIZE];
    char type[BOT_ASSET_TYPE_MAX_SIZE];
    char adv[BOT_ASSET_ADV_MAX_SIZE];
} bot_assetInfo_t;

/* 地理位置数据结构 */
typedef struct {
    int coordinateSystem;       /* 坐标系，1-WGS_84，2-GCJ_02 */
    double longitude;           /* 经度 */
    double latitude;            /* 纬度 */
    double altitude;            /* 海拔高度 */
    int source;                 /* 定位源 */
} bot_asset_location_t;

/* 隐私保护数据结构 */
typedef struct {
    ldp_value_u bit;
    ldp_value_u krr;
} bot_asset_ldp_t;

/**
 * @brief 获取应用状态（仅供参考，用户根据实际情况具体实现）
 *
 * @return 应用状态
 */
bot_msg_type_e bot_app_sta_get(void);

#if BOT_GATEWAY

/**
 * @brief 获取资产信息（仅供参考，用户根据实际情况具体实现）
 *
 * @param[in] asset_info:   存储资产信息的内存地址
 *
 * @return 资产信息获取结果
 * @retval 0 成功
 * @retval -1 失败
 */
int batterycabinet_asset_info_get(bot_assetInfo_t *asset_info);

/**
 * @brief 获取资产数据（仅供参考，用户根据实际情况具体实现）
 *
 * @param[out] data:  存储业务数据的内存地址
 *
 * @return 资产数据获取结果
 * @retval 0 成功
 * @retval -1 失败
 */
int batterycabinet_data_pack(char *data, unsigned int data_len);
int batterycabinet_price_data_pack(char *data, unsigned int data_len);

/**
 * @brief 获取资产信息（仅供参考，用户根据实际情况具体实现）
 *
 * @param[in] asset_info:   存储资产信息的内存地址
 *
 * @return 资产信息获取结果
 * @retval 0 成功
 * @retval -1 失败
 */
int batterypack_asset_info_get(bot_assetInfo_t *asset_info);

/**
 * @brief 获取资产数据（仅供参考，用户根据实际情况具体实现）
 *
 * @param[out] data:  存储业务数据的内存地址
 *
 * @return 资产数据获取结果
 * @retval 0 成功
 * @retval -1 失败
 */
int batterypack_data_pack(char *data, unsigned int data_len);

/**
 * @brief 获取资产信息（仅供参考，用户根据实际情况具体实现）
 *
 * @param[in] asset_info:   存储资产信息的内存地址
 *
 * @return 资产信息获取结果
 * @retval 0 成功
 * @retval -1 失败
 */
int batterycluster_asset_info_get(bot_assetInfo_t *asset_info);

/**
 * @brief 获取资产数据（仅供参考，用户根据实际情况具体实现）
 *
 * @param[out] data:  存储业务数据的内存地址
 *
 * @return 资产数据获取结果
 * @retval 0 成功
 * @retval -1 失败
 */
int batterycluster_data_pack(char *data, unsigned int data_len);

#else

/**
 * @brief 获取资产信息（仅供参考，用户根据实际情况具体实现）
 *
 * @param[in] asset_info:   存储资产信息的内存地址
 *
 * @return 资产信息获取结果
 * @retval 0 成功
 * @retval -1 失败
 */
int user_asset_info_get(bot_assetInfo_t *asset_info);

/**
 * @brief 获取资产数据（仅供参考，用户根据实际情况具体实现）
 *
 * @param[out] data:  存储业务数据的内存地址
 *
 * @return 资产数据获取结果
 * @retval 0 成功
 * @retval -1 失败
 */
int user_asset_data_get(char *data, int data_len);
#endif

#if defined(__cplusplus)
}
#endif
#endif /* __BOT_APP_BMS_H__ */

