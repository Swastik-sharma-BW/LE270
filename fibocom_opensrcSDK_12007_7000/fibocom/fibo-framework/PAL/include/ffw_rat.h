/**
* @file ffw_rat.h
* @author sundaqing (sundaqing@fibocom.com)
* @brief 无线网络相关的接口实现，依赖于各个平台实现
*        PDP激活接口(ffw_rat_pdp_active)成功时，需要发送 @c FFW_EVT_PDP_ACTIVED 事件
*        去激活(ffw_rat_pdp_deactive)成功时，需要发送 @c FFW_EVT_PDP_DEACTIVED 事件
*        被动掉网时需要发送 @c FFW_EVT_PDP_RELEASE 事件，使用ffw_broker_send_msg发送
* @version 0.1
* @date 2021-05-20
* 
* Copyright (c) 2021 Fibocom. All rights reserved
* 
 */

#ifndef LIB_INCLUDE_FFW_RAT
#define LIB_INCLUDE_FFW_RAT

#include <ffw_utils.h>
#include <stdint.h>
#include "cbb_3gpp.h"
#include "ps_lib_api.h"

#define FFW_CID_MIN 1
#define FFW_CID_MAX 7

#define FFW_APN_MAX_LEN 99
#define FFW_PDP_ADDR_MAX_LEN 21
#define FFW_APN_USER_MAX_LEN 64
#define FFW_APN_PWD_MAX_LEN 64
// #define FFW_PDP_DNS_MAX_LEN 21 ///< 0-3: IPV4  4-19:IPV6
#define FFW_PDP_DNS_NUM 2


#define FFW_PDP_D_COMP_OFF 0 ///<  off (default if value is omitted)
#define FFW_PDP_D_COMP_ON 1  ///<  on(manufacturer preferred compression)
#define FFW_PDP_D_COMP_V42 2 ///<  V.42bis
#define FFW_PDP_D_COMP_V44 3 ///<  V.44

// PDP header compression
#define FFW_PDP_H_COMP_OFF 0     ///<  off (default if value is omitted)
#define FFW_PDP_H_COMP_ON 1      ///<  on (manufacturer preferred compression)
#define FFW_PDP_H_COMP_RFC1144 2 ///<  RFC1144 (applicable for SNDCP only)
#define FFW_PDP_H_COMP_RFC2507 3 ///<  RFC2507
#define FFW_PDP_H_COMP_RFC3095 4 ///<  RFC3095 (applicable for PDCP only)


#define FFW_CFT_STATION  0x03
#define FFW_NSFT_STATION 0x04
#define FFW_CT_STATION   0x07




/// type of ffw_pdp_profile_s
typedef struct ffw_pdp_profile_s ffw_pdp_profile_t;

/// type of ffw_cellinfo_s
typedef struct ffw_cellinfo_s ffw_cellinfo_t;

/// type of ffw_wifiinfo_s
typedef struct ffw_wifiinfo_s ffw_wifiinfo_t;

/// type of ffw_gsm_cellinfo_s
typedef struct ffw_gsm_cellinfo_s ffw_gsm_cellinfo_t;

/// type of ffw_lte_cellinfo_s
typedef struct ffw_lte_cellinfo_s ffw_lte_cellinfo_t;

typedef enum ffw_reattach_status_s
{
    /// 空闲态
    FFW_IDLE = 0,

    /// 正在去附着
    FFW_DETACHING,

    /// 去附着成功
    FFW_DETACHED,

    /// 正在附着
    FFW_ATTACHING,

    /// 附着成功，
    FFW_ATTACHED,
} ffw_reattach_status_t;

/// plateform type define
typedef enum ffw_plateform_type_s
{
    /// IMEI
    FFW_PLAT_TYPE_IMEI = 0,

    /// IMSI
    FFW_PLAT_TYPE_IMSI,

    /// soft version
    FFW_PLAT_TYPE_SOFT_VER,

    /// hard version
    FFW_PLAT_TYPE_HARD_VER,

    /// model type
    FFW_PLAT_TYPE_MODEL,

    /// ICCID
    FFW_PLAT_TYPE_ICCID,

    /// CHIP ID
    FFW_PLAT_TYPE_CHIP_TYPE,

    // SN
    FFW_PLAT_TYPE_SN,
} ffw_plateform_type_t;

/// plateform type define
typedef enum ffw_gtccinfo_search_mode_s
{
    FFW_SYNC_MODE = 0,

    FFW_ASYNC_MODE,

    FFW_SYNC_CACHE_MODE,

    FFW_ASYNC_CACHE_MODE,

    FFW_SYNC_CACHE_PERIODIC_MODE,
} ffw_gtccinfo_search_mode_t;

typedef struct
{
    uint8_t value;
    char bw_str[8];
}ffw_bandwidth_map;
/**
 * @brief The callback function type for cell info query
 *
 * @param result The result of cell query, @c FFW_R_FAILED for fail and @c FFW_R_SUCCESS for ok
 * @param cellinfo The cell infomation, this value valid if @c result is @c FFW_R_SUCCESS
 * @param arg The context of callback
 */
typedef void (*ffw_cellinfo_callback_t)(int result, ffw_cellinfo_t *cellinfo, void *arg);

/**
 * @brief The callback function type for cell info query
 *
 * @param result The result of cell query, @c FFW_R_FAILED for fail and @c FFW_R_SUCCESS for ok
 * @param wifiinfo The wifi tap infomation, this value valid if @c result is @c FFW_R_SUCCESS
 * @param cnt 热点的个数
 * @param arg The context of callback
 */
typedef void (*ffw_wifiinfo_callback_t)(int result, ffw_wifiinfo_t *wifiinfo, int cnt, void *arg);

/**
* @brief PDP TYPE 定义
* 
 */
typedef enum ffw_pdp_type_s
{
    /// IP
    FFW_PDP_TYPE_IP = 1,

    /// IP Version 6
    FFW_PDP_TYPE_IPV6 = 2,

    /// DualStack
    FFW_PDP_TYPE_IPV4V6 = 3
} ffw_pdp_type_t;

///RAT 类型定义
typedef enum ffw_rat_s
{
    ///GSM
    FFW_RAT_GSM = 0,

    ///WCDMA 3G
    FFW_RAT_WCDMA = 2,

    ///LTE
    FFW_RAT_LTE = 7,

    /// NO SERVICE
    FFW_RAT_NO_SERVICE = 0XFF
} ffw_rat_t;

/// PDP 鉴权方式
typedef enum ffw_pdp_auth_proto_s
{
    /// NONE
    FFW_PDP_AUTOPROTO_NONE = 0,

    /// PAP
    FFW_PDP_AUTOPROTO_PAP = 1,

    /// CHAP
    FFW_PDP_AUTOPROTO_CHAP = 2,

    /// PAP_CHAP
    FFW_PDP_AUTOPROTO_PAP_CHAP = 3,

    /// invalid
    FFW_PDP_AUTOPROTO_INVALID,

} ffw_pdp_auth_proto_t;

/**
* @brief PDP配置定义
* 
 */
struct ffw_pdp_profile_s
{
    /// The ID of cid
    int8_t cid;

    /// 自动创建pdp context
    bool auto_create_pdpctx;

    /// PDP type, see ::ffw_pdp_type_t
    ffw_pdp_type_t pdp_type;

    /// The value of apn
    const char *apn;

    /// 鉴权用户名
    const char *apn_user;

    ///鉴权密码
    const char *apn_passwd;

    /// 鉴权方式 see ::ffw_pdp_auth_proto_t
    ffw_pdp_auth_proto_t auth_proto;

    /// auth_proto whether need to change
    bool need_set_auth;

    /// no reteach
    bool no_reteach;
};

/**
* @brief LTE网络小区信息
* 
 */
struct ffw_lte_cellinfo_s
{
    /// 接入类型
    ffw_rat_t rat;

    /// mcc
    uint8_t mcc[4];

    /// mnc
    uint8_t mnc[4];

    /// The number of mnc
    uint8_t mnc_num;

    /// tac
    uint16_t tac;

    /// The ID of cell
    uint32_t cellid;

    /// earfcn
    uint32_t earfcn;

    /// The physical ID of cell
    uint16_t physical_cellid;

    ///当 is_service_cell 为 true 时有效
    uint32_t band;

    /// 带宽
    uint32_t bandwidth;

    ///当 is_service_cell 为 true 时有效
    int16_t rssnr;

    /// rx level
    int16_t rx_level;

    /// rsrp
    int16_t rsrp;

    /// rsrq
    int16_t rsrq;

    /// sinr
    uint16_t sinr;

    /// cell prority
    uint8_t cellPrority;

    /// p_max
    uint8_t p_max;

    /// rssi
    int16_t rssi;

    /// Cell Selection Criterion
    int16_t squal;
};

/**
* @brief GSM小区信息定义
* 
 */
struct ffw_gsm_cellinfo_s
{
    /// 接入类型
    ffw_rat_t rat;

    /// mcc
    uint8_t mcc[3];

    /// mnc
    uint8_t mnc[3];

    /// The number of mnc
    uint8_t mnc_num;

    /// lac
    uint16_t lac;

    /// cell id
    uint16_t cellid;

    /// arfcn
    uint16_t arfcn;

    /// basic
    uint8_t basic;

    /// rxlev
    uint8_t rxlev;

    /// rssi
    uint8_t rssi;

    ///当 is_service_cell 为 true 时有效
    uint16_t band;

    /// txpwr
    uint16_t txpwr;

    /// dtx
    bool dtx;

    /// c1
    uint8_t c1;

    /// c2
    uint16_t c2;

    ///当 is_service_cell 为 false 时有效
    uint32_t c31;

    /// c32
    uint32_t c32;

    ///当 is_service_cell 为 true 时有效
    uint8_t access_tech;

    /// amr acs
    uint8_t amr_acs;

    /// maio
    uint8_t maio;

    /// hsn
    uint8_t hsn;

    /// rx lev sub
    uint8_t rxlevsub;

    /// rx level of full
    uint8_t rxlevfull;

    /// rx qual sub
    uint8_t rxqualsub;

    /// rx qual full
    uint8_t rxqualfull;

    /// ber
    uint8_t ber_lev;
};

/**
* @brief 无线消息信息定义
* 
 */
struct ffw_cellinfo_s
{
    /// 接入类型
    ffw_rat_t rat;

    /// The information of lte cell info
    ffw_lte_cellinfo_t lte_cellinfo;

    /// The information of gsm cell info
    ffw_gsm_cellinfo_t gsm_cellinfo;

    /// LTE 的邻区信息
    ffw_lte_cellinfo_t lte_neb_cellinfo[10];

    /// GSM 的邻区信息
    ffw_gsm_cellinfo_t gsm_neb_cellinfo[10];

    /// gsm邻区信息的个数
    uint8_t gsm_neb_num;

    /// lte邻区信息的个数
    uint8_t lte_neb_num;

    /// txpower
    uint32_t txpower;
};

/**
* @brief WIFI消息信息定义
* 
 */
struct ffw_wifiinfo_s
{
    /// mac 地址
    char ap_mac[32];

    /// 信号值
    int8_t ap_rssi;

    /// 热点名字
    char ap_name[64];

    uint8_t channel;
};

typedef struct ffw_nw_status_info_s
{
    uint8_t nStatus;
    uint8_t PsType;          // 1: GPRS 2: EDGE 3: UMTS 4: NB-IoT 3: WB-EUTRAN(eg.CATM)
    uint8_t nAreaCode[5];
    uint8_t nCurrRat;
    uint8_t nCellId[4]; //cell id in umts is bitstring(28), so 4 bytes is necessary.
    uint8_t cause_type;
    uint8_t reject_cause;
    uint8_t activeTime;
    uint8_t periodicTau;
    uint16_t bsic;
    uint16_t gsmFreq;
} ffw_nw_status_info_t;

typedef struct
{
    int cid;

    uint8_t nDComp;
    uint8_t nHComp;

    // uint8_t nPcscfType[MAX_PCSCF_ADDR_NUMBER];
    // uint8_t nPcscfNumber;
    // uint8_t PcscfAddr[16 * MAX_PCSCF_ADDR_NUMBER];
    uint8_t nIsEmergency;

    /// dns type
    ffw_pdp_type_t dnstype;

    /// dns address
    char dnsaddr[2][FFW_INET_ADDRSTRLEN];

    /// dns address v6
    char dnsaddr6[2][FFW_INET6_ADDRSTRLEN];

    /// pdpaddr address
    char pdpaddr[2][FFW_INET6_ADDRSTRLEN];

    /// ipaddress for wan，如果IP NAT功能打开，可以获取这个值
    char wan_addr[2][FFW_INET6_ADDRSTRLEN];

    /// username
    char apn_username[FFW_APN_USER_MAX_LEN + 1];

    /// password
    char apn_password[FFW_APN_PWD_MAX_LEN + 1];

    /// apn
    char apn[FFW_APN_MAX_LEN + 1];

    // uint8_t nNSLPI;

    /// pdp type
    ffw_pdp_type_t pdptype;

    /// auth proto
    ffw_pdp_auth_proto_t auth_proto;
} ffw_pdp_context_t;

/**
* @brief 激活PDP
* 
* @param simid sim卡
* @param cid cid
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败
 */
int ffw_rat_pdp_active(int8_t simid, int8_t cid);

/**
* @brief 去激活PDP
* 
* @param simid sim卡
* @param cid cid
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败
 */
int ffw_rat_pdp_deactive(int8_t simid, int8_t cid);


/**
* @brief 判断当前cid是否激活，为了防止没有调用ffw激活接口的情况
* 
* @param simid sim卡
* @param cid cid
* @return int 
* @retval FFW_R_SUCCESS: true 激活
* @retval FFW_R_FAILED: 未激活
 */
bool ffw_rat_pdp_actived(int8_t simid, int8_t cid);

/**
* @brief 获取RAT类型
* 
* @param simid sim卡号
* @return ffw_rat_t 
* @retval FFW_RAT_GSM GSM
* @retval FFW_RAT_LTE LTE
 */
ffw_rat_t ffw_get_rat(int simid);

/**
* @brief 异步获取小区信息
* 
* @param simid sim卡号
* @param timeout 超时时间，单位毫秒
* @param cb 回调函数
* @param arg 回调上下文
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败

 */
int ffw_get_cellinfo(int simid, int32_t timeout, ffw_cellinfo_callback_t cb, void *arg);

/**
* @brief 同步获取小区信息
* 
* @param simid sim卡号
* @param timeout 超时时间，单位毫秒
* @param cellinfo 小区信息
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败
 */
int ffw_get_cellinfo_sync(int simid, int timeout, ffw_cellinfo_t *cellinfo);

/**
* @brief 异步获取WIFI SCAN信息
* 
* @param timeout 超时时间，单位毫秒
* @param cb 回调函数
* @param arg 回调上下文
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败

 */
int ffw_get_wifiinfo(int32_t timeout, ffw_wifiinfo_callback_t cb, void *arg);

/**
* @brief 异步获取WIFI SCAN信息
* 
* @param timeout 超时时间，单位毫秒
* @param[out] wifiinfo wifi 信息
* @param size wifiinfo的大小
* @param[out] cnt 扫描到的wifi热点个数
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败

 */
int ffw_get_wifiinfo_sync(int32_t timeout, ffw_wifiinfo_t *wifiinfo, int size, int *cnt);

/**
* @brief 已字符串的格式输出小区信息，@c b 参数必须是 @c ::ffw_buffer_t 类型
* 
* @param cellinfo 
* @param b 
* @return int 
 */
int ffw_cellinfo_dump(ffw_cellinfo_t *cellinfo, void *b);

/**
* @brief 获取gprs状态是否在连接状态
* 
* @param sim_id SIM卡ID
* @return true 已注册
* @return false 未注册
 */
bool ffw_gprs_reg_status(uint8_t sim_id);

/**
* @brief 获取平台信息
* 
* @param id SIM卡ID
* @param ptype 类型
* @param[out] data 存储信息数据的缓冲区
* @param len @c data 缓冲区的长度
* @return int
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败 
 */
int ffw_get_plateform_info(int id, ffw_plateform_type_t ptype, char *data, int len);

/**
* @brief 当前是否在打电话状态
* 
* @return int 
* @retval FFW_R_SUCCESS: 在打电话
* @retval FFW_R_FAILED: 未打电话
 */
int ffw_is_in_call();

/**
* @brief 
* 
* @return int 
 */
int ffw_get_default_pdninfo(int simid, ffw_pdp_context_t *pdpinfo);

/**
* @brief 修改nv里的pdp配置并保存
* 
* @param simid 
* @param cid cid 为0表示默认承载的context
* @param pdpinfo 
* @return int 
 */
int ffw_write_nv_pdninfo(int simid, int cid, ffw_pdp_context_t *pdpinfo);


/**
* @brief 读取nv里的pdp配置并保存
* 
* @param simid 
* @param pdpinfo 
* @return int 
 */
int ffw_read_nv_pdninfo(int simid, int cid, ffw_pdp_context_t *pdpinfo);

/**
* @brief 如果修改了默认context，如果需要在驻网时生效，需要将默认配置发送给协议栈，如果没有类似的接口，可以留空
* 
* @param simid 
* @param pdpinfo 
* @return int 
 */
int ffw_send_def_pdpctx_stack(int simid, ffw_pdp_context_t *pdpinfo);

/**
* @brief 
* 
* @param simid 
* @param cid 
* @param pdpinfo 
* @return int 
 */
int ffw_get_pdpcontext(int simid, int cid, ffw_pdp_context_t *pdpinfo);

/**
* @brief 设置或者增加一个context
* 
* @param simid 
* @param cid 
* @param pdpinfo 
* @return int 
 */
int ffw_set_pdpcontext(int simid, int cid, ffw_pdp_context_t *pdpinfo);

/**
* @brief 删除一个context
* 
* @param simid 
* @param cid 
* @return int 
 */
int ffw_remove_pdpcontext(int simid, int cid);

/**
* @brief 
* 
* @param simid 
* @param attach 
* @return int 
 */
int ffw_rat_attdeatt(int simid, bool attach);

/**
 * @brief 获取网络信息
 *
 * @param nw_status 网络信息结构体
 * @param simid
 * @return int
 */
int ffw_get_nw_info(ffw_nw_status_info_t *nw_status,uint8_t simid);

/**
 * @brief 获取rndis 或者 ppp 激活的cid，如果没有激活，则返回-1
 *
 * @param
 * @return int
 */
int ffw_rat_get_rndis_ppp_cid();

int ffw_get_cellinfo_ex(int simid, int32_t timeout, ffw_cellinfo_callback_t cb, void *arg);
#if defined(FFW_XINYUAN_CONFIG)
int ffw_get_mac(uint8_t* mac, uint16_t len);
#endif
/**
* @brief set local release time.
* 
* @param loc release value,unit:second
* @param simId
* @return int
 */
int ffw_locrel_set(int level,uint8_t simId);
int ffw_set_locrel_V2(void *engine,int simid);
int ffw_set_jdc_params(void *engine,fibo_jdc_cfg_t * jdc_cfg);
int ffw_get_jdc_params(void *engine);

struct ffw_drx_s
{
    uint16_t nw_drx;
    uint16_t user_drx;
};
typedef struct ffw_drx_s ffw_drx_t;
int ffw_set_drxtm(void *engine,int32_t drxValue);
typedef void (*ffw_get_drx_callback_t)(int result, ffw_drx_t *drxInfo, void *arg);
int ffw_get_drxtm(int simid, ffw_get_drx_callback_t cb, void *engine);
int ffw_set_rrm(void *engine,bool rrmValue);
int ffw_get_rrm(void *engine,int32_t *rrmValue);
int ffw_get_chipdata(uint32_t *year,uint32_t *mon,uint32_t *day);
int ffw_get_imsi_by_simid(uint8_t *imsi,uint8_t simId);
int ffw_get_iccid_by_simid(uint8_t *iccid,uint8_t simId);
int ffw_get_imei_by_simid(uint8_t *imei,uint8_t simId);
#ifdef CONFIG_CUST_KR_FEATURE
int ffw_set_cdrx(void *engine,int32_t CdrxValue);
int ffw_get_cdrx(void *engine,int32_t *CdrxValue);
#endif
int ffw_get_cellscan(int simid, int32_t timeout, ffw_cellinfo_callback_t cb, void *arg);

int ffw_gtcb_get(void *engine,int32_t station,int32_t *status);
int ffw_gtcb_set(void *engine,int32_t station,int32_t status);

int ffw_earfcn_to_frequency(int band, int direction, int earfcn,int *freq);

int ffw_check_band_and_earfcn(int band, int direction, int earfcn);


int  ffw_calculate_uplink_freq(int dl_freq, int band,int *ul_freq);
void ffw_set_cfun_default_value(uint8_t val);
int ffw_ccinfo_read(int simid, void *engine);
int ffw_get_sCellInfo(int simid, void *engine);
int ffw_get_nCellInfo(int simid, void *engine, int32_t mode, int32_t update_time);
void ffw_get_cellinfo_cache(BasicCellListInfo *info);
void ffw_get_cellinfo_bg();
int ffw_get_phyInfo(int simid, void *engine, int32_t interval);
int ffw_set_max_txpower(uint32_t handle,int32_t power);
int ffw_set_txpower_for_band(uint32_t handle,int32_t *data);
int ffw_set_eccfg(uint32_t handle,const char *paraStr,int32_t *data, int count);
#endif /* LIB_INCLUDE_FFW_RAT */
