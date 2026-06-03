/**
 * @file ffw_event.h
 * @author sundaqing (sundaqing@fibocom.com)
 * @brief 全局事件定义
 * @version 0.1
 * @date 2021-10-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef C427EE94_5798_4144_AF3E_0D1ECB0ADF20
#define C427EE94_5798_4144_AF3E_0D1ECB0ADF20

#include <stdint.h>
#include <ffw_utils.h>

/**
 * @brief 全局事件定义
 * 
 */
typedef enum ffw_event_s
{
    /// pdp cp 激活事件, 内部移植时使用，网卡创建成功，可以做tcp业务了。
    FFW_EVT_CP_PDP_ACTIVED = 0,
    
    /// pdp cp 激活事件, 区别于 @FFW_EVT_CP_PDP_ACTIVED，仅仅是PDN激活，网卡不一定创建成功
    FFW_EVT_CP_PDN_ACTIVED,
    
    /// pdp cp 去激活事件，内部移植时使用
    FFW_EVT_CP_PDP_DEACTIVED,

    /// pdp cp 收到sms，内置事件，其他应用模块无法使用
    FFW_EVT_CP_SMS_IN,

    /// 网络驻网变化, 如果系统网络有变化，需要发送这个事件，ffw内部使用，其他用户接口不得使用
    FFW_EVT_CP_GPRS_NETWORK_CHANGE,

    /// 驻网事件 ffw内部使用，其他用户接口不得使用
    FFW_EVT_CP_GPRS_ATTACHED,

    /// 去驻网事件  ffw内部使用，其他用户接口不得使用
    FFW_EVT_CP_GPRS_DEATTACHED,

    /// pdp 激活事件
    FFW_EVT_PDP_ACTIVED = 100,

    /// 第一个pdp激活事件
    FFW_EVT_PDP_FIRST_ACTIVED,

    /// pdp 去激活事件
    FFW_EVT_PDP_DEACTIVED,

    /// pdp 被动去激活事件
    FFW_EVT_PDP_ABORT,

    /// 最后一个pdp去激活
    FFW_EVT_PDP_LAST_DEACTIVED,

    /// pdp 掉网事件
    FFW_EVT_PDP_RELEASE,

    /// 收到短信事件
    FFW_EVT_SMS_IN,

    /// GTSET有改变
    FFW_EVT_GTSET_CHANGED,

    /// 网络驻网变化
    FFW_EVT_GPRS_NETWORK_CHANGE,

    /// TCP在使用事件
    FFW_EVT_TCP_CONNECTTED,

    /// TCP未使用事件
    FFW_EVT_TCP_DISCONNECTTED,

    //ATTACH事件
    FFW_EVT_REATTACHED,

    //PSM RESTORE ACT
    FFW_EVT_CP_PDP_PSM_ACTIVED,

    // SLEEP ACT
    FFW_EVT_ENTER_SLEEP,

    // WAKEUP ACT
    FFW_EVT_EXIT_SLEEP,

    // 收到数据收发统计值
    FFW_EVT_DATA_STATIS_RESP,

    //RECEV SMS
    FFW_EVT_SMS_RECEV,

    //PSM EVENT
    FFW_EVT_PSM,

    //LAN BIND EVENT
    FFW_EVT_LAN_BIND_EVENT,

    // FFW BEARER CHANGE
    FFW_EVT_BEARER_CHANGE_EVENT,

    //GTRNDIS EVENT
    FFW_EVT_GTRNDIS_EVENT,

    //PPP EVENT
    FFW_EVT_PPP_EVENT,

    //RECEV TCPIP_DATA
    FFW_EVT_TCPIP_DATA_RECV,

    //RECEV URC_WAKEUP_HOST_EVENT
    FFW_EVT_URC_WAKEUP_HOST_EVENT,

    /// MAX EVENT
    FFW_EVT_MAX
} ffw_event_t;

/// PDP激活/去激活相关(FFW_EVT_PDP_ACTIVED/FFW_EVT_PDP_DEACTIVED)事件数据
typedef struct ffw_evt_pdp_s
{
    /// sim id
    uint8_t simid;

    /// cid
    uint8_t cid;

    /// ip 类型
    uint8_t iptype; /// 1: ip 2: ipv6 3: ipv4v6

    /// ipv4 address
    char ipv4[FFW_INET_ADDRSTRLEN];

    /// ipv6 address
    char ipv6[FFW_INET6_ADDRSTRLEN];
} ffw_evt_pdp_t;

typedef struct ffw_cp_evt_pdp_s
{
    /// 结果
    int result;

    /// sim id
    uint8_t simid;

    /// cid
    uint8_t cid;
} ffw_cp_evt_pdp_t;

/// sms事件数据 FFW_EVT_SMS_IN
typedef struct ffw_evt_sms_s
{
    /// sim id
    uint8_t simid;

    /// 短信ID
    uint32_t index;
} ffw_evt_sms_t;

/// GTSET变更事件数据 FFW_EVT_GTSET_CHANGED
typedef struct ffw_evt_gtset_s
{
    /// name
    const char *name;

    /// 数据
    int32_t *data;

    /// 数据长度
    uint32_t len;
} ffw_evt_gtset_t;


/// GPRS/LTE 网络变化事件
typedef struct ffw_evt_gprs_s
{
    /// sim id
    uint8_t simid;
    
    /// 是否已经驻网
    bool attached;
} ffw_evt_gprs_t;

typedef struct ffw_evt_data_statis_resp_s
{
    /// 发送请求的 handle
    uint32_t handle;

    /// 发送数据大小
    uint64_t  pkgSentBytes;

    /// 接收数据大小
    uint64_t  pkgRecvBytes;
} ffw_evt_data_statis_resp_t;

typedef struct ffw_evt_lan_bind_ind_s
{
    bool       bound;
    uint8_t    lanIpType;
    uint8_t    lanBindIp4Cid;
    uint8_t    lanBindIp6Cid;
} ffw_evt_lan_bind_ind_t;

typedef struct ffw_evt_bearer_ind_s
{
    bool       bactived;
    uint8_t    cid;
} ffw_evt_bearer_ind_t;

typedef struct ffw_evt_gtrndis_ind_s
{
    bool       bactived;
    uint8_t    cid;
} ffw_evt_gtrndis_ind_t;

typedef struct ffw_evt_ppp_ind_s
{
    bool       bactived;
    uint8_t    cid;
} ffw_evt_ppp_ind_t;

#endif /* C427EE94_5798_4144_AF3E_0D1ECB0ADF20 */
