/*
 * Copyright (C) 2015-2023 Ant Group Holding Limited
 */

#ifndef __BOT_SUBDEVICE_H__
#define __BOT_SUBDEVICE_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "bot_network.h"

#define BOT_SUBDEV_PK_LEN_MAX   32
#define BOT_SUBDEV_DN_LEN_MAX   32
#define BOT_SUBDEV_PARENT_LEN_MAX   64
#define BOT_SUBDEV_DS_LEN_MAX   64
#define BOT_SUBDEV_PS_LEN_MAX   32

#ifndef BOT_ASSET_ID_MAX_SIZE
#define BOT_ASSET_ID_MAX_SIZE   64
#endif
#ifndef BOT_ASSET_TYPE_MAX_SIZE
#define BOT_ASSET_TYPE_MAX_SIZE 64
#endif
#ifndef BOT_ASSET_ADV_MAX_SIZE
#define BOT_ASSET_ADV_MAX_SIZE  16
#endif

typedef struct {
    char pk[BOT_SUBDEV_PK_LEN_MAX];
    char dn[BOT_SUBDEV_DN_LEN_MAX];
    char ds[BOT_SUBDEV_DS_LEN_MAX];
    char ps[BOT_SUBDEV_PS_LEN_MAX];
    char parent[BOT_SUBDEV_PARENT_LEN_MAX];
    char asset_id[BOT_ASSET_ID_MAX_SIZE];
    char asset_type[BOT_ASSET_TYPE_MAX_SIZE];
    char asset_adv[BOT_ASSET_ADV_MAX_SIZE];
    char *payload;
    uint32_t payload_len;
    bot_uid_type_e uid_type;
} bot_subdev_dev_t;

typedef enum {
    BOT_SUBDEV_TOPO_ADD_REPLY = 0,      //添加拓扑关系应答
    BOT_SUBDEV_TOPO_DEL_REPLY,          //删除拓扑关系应答
    BOT_SUBDEV_TOPO_GET_REPLY,          //获取拓扑信息应答
    BOT_SUBDEV_LOGIN_REPLY,             //子设备登录应答
    BOT_SUBDEV_LOGOUT_REPLY,            //子设备登出应答
    BOT_SUBDEV_DYN_REGISTER_REPLY,      //子设备动态注册应答
    BOT_PRODUCT_REGISTER_REPLY,         //产品注册通知应答
    BOT_SUBDEV_TOPO_CHANGE_NOTIPFY,     //拓扑关系改变通知
    BOT_SUBDEV_REC_RAWDATA_NOTIPFY,     //接收下行数据通知
    BOT_SUBDEV_ASSET_ACTIVATE_REPLY,    //子设备资产注册应答
    BOT_SUBDEV_MSG_TYPE_MAX
} bot_subdev_msg_type_e;

typedef struct {
    int code;
    char *device_name;
    char *product_key;
    char *data;
    unsigned int data_len;
} bot_subdev_msg_t;

typedef void (*bot_subdev_msg_cb)(bot_subdev_msg_type_e, bot_subdev_msg_t *msg);

/**
 * @brief gateway initialization;
 * @param[in] handle handle information;
 * @param[in] cb message callback;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_init(void **handle, bot_subdev_msg_cb cb);

/**
 * @brief add a topology to the gateway;
 * @param[in] handle bot_gw_init Fetch handle;
 * @param[in] dev[] subdevice group;
 * @param[in] dev_num the maximum number of sub-devices cannot exceed 30;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_topo_add(void *handle, bot_subdev_dev_t dev[], unsigned int dev_num);

/**
 * @brief gets the topology relationship, which is returned from the callback function;
 * @param[in] handle bot_gw_init Fetch handle;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_topo_get(void *handle);

/**
 * @brief deleting a topology relationship;
 * @param[in] handle bot_gw_init Fetch handle;
 * @param[in] dev[] subdevice group;
 * @param[in] dev_num the maximum number of sub-devices cannot exceed 30;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_topo_rm(void *handle, bot_subdev_dev_t dev[], unsigned int dev_num);

/**
 * @brief subdevice login;
 * @param[in] handle bot_gw_init Fetch handle;
 * @param[in] dev[] subdevice group;
 * @param[in] dev_num the maximum number of sub-devices cannot exceed 30;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_login(void *handle, bot_subdev_dev_t dev[], unsigned int dev_num);

/**
 * @brief subdevice logout;
 * @param[in] handle bot_gw_init Fetch handle;
 * @param[in] dev[] subdevice group;
 * @param[in] dev_num the maximum number of sub-devices cannot exceed 30;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_logout(void *handle, bot_subdev_dev_t dev[], unsigned int dev_num);

/**
 * @brief subdevice registration obtains the device key;
 * @param[in] handle bot_gw_init Fetch handle;
 * @param[in] dev[] subdevice group;
 * @param[in] dev_num the maximum number of sub-devices cannot exceed 30;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_dyn_reg(void *handle, bot_subdev_dev_t dev[], unsigned int dev_num);

/**
 * @brief subdevice asset registration, which can be invoked after device login;
 * @param[in] handle bot_gw_init Fetch handle;
 * @param[in] dev[] subdevice group;
 * @param[in] dev_num the maximum number of sub-devices cannot exceed 30;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_asset_activate(void *handle, bot_subdev_dev_t dev[], unsigned int dev_num);

/**
 * @brief obtain the subdevice asset registration status;
 * @param[in]   subdevice_dn device name;
 * @param[out]  sta 0 - unregistered, 1 - registered;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_asset_status_get(char *subdevice_dn, int *sta);

/**
 * @brief subdevice data is reported;
 * @param[in] handle bot_gw_init Fetch handle;
 * @param[in] dev[] subdevice group;
 * @param[in] dev_num the maximum number of sub-devices cannot exceed 30;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_subdev_data_publish(void *handle, bot_subdev_dev_t dev[], unsigned int dev_num);

/**
 * @brief gateway data is reported;
 * @param[in] handle  bot_gw_init Fetch handle;
 * @param[in] payload valid data;
 * @param[in] payload_len Valid data len;
 * @retval 0, success
 * @retval -1, failed
*/
int bot_gw_gateway_data_publish(void *handle, uint8_t *payload, uint32_t payload_len);

#if defined(__cplusplus)
}
#endif
#endif /* __BOT_SERVICE_API_H__ */