#ifndef APIS_GSM_H_
#define APIS_GSM_H_
#include "NK_Export.h"
NK_CPP_EXTERN_BEGIN

#include "APIS_Macro.h"
#include "APIS_System.h"

typedef enum GSM_PDP_TYPE_E_TAG{
    GSM_PDP_TYPE_IP = 0,
    GSM_PDP_TYPE_IPV6,
    GSM_PDP_TYPE_IPV4V6,
}GSM_PDP_TYPE_E;

typedef enum GSM_PDP_AUTH_TYPE_E_TAG{
    GSM_PDP_AUTH_TYPE_NONE = 0,
    GSM_PDP_AUTH_TYPE_PAP,
    GSM_PDP_AUTH_TYPE_CHAP,
}GSM_PDP_AUTH_TYPE_E;

typedef enum{
    GSM_SIMNUM_0,
    GSM_SIMNUM_1,

    GSM_SIMNUM_ALL,
}GSM_SIMNUM_E;

#define GSM_REJECT_CAUSE_STR_11     "PLMN not allowed"
/**
 * @brief 12
 * UE将EPS更新状态更新为不允许漫游，删除所有GUTI、上次访问注册的TAI、TAI列表和eKSI。
 * UE将重置跟踪区更新尝试计数器，将当前的TAI存储在“区域服务提供的禁止跟踪区域”列表中。
 * UE进入emm-deregistered.limited-service状态
*/
#define GSM_REJECT_CAUSE_STR_12     "tracking area not allowed"
/**
 * @brief 13
 * UE将EPS更新状态为不允许漫游，删除等效PLMN列表。
 * 并将重置跟踪区更新尝试计数器，UE应将当前的TAI存储在“禁止漫游的跟踪区域”列表中，并将当前的TAI从存储的TA列表中删除。
 * UE进入emm-registered.plmn-search状态。然后UE根据3GPP TS 23.122 [ 6 ]进行PLMN选择，并在新的TAC中进行联合TA / LA更新和IMSI附着
 */
#define GSM_REJECT_CAUSE_STR_13     "roaming not allowed in this tracking area"
/**
 * @brief 14
 * UE将EPS更新状态更新为不允许漫游，删除所有GUTI、上次访问注册的TAI、TAI列表和eKSI。
 * UE将重置跟踪区更新尝试计数器，将当前的PLMN存储在“禁止PLMNs GPRS服务”名单。UE进入emm-deregistered.plmn-search状态。
 * 此时看UE终端是CS/PS1（voice centric）模式还是CS/PS2（data centric）模式，在进入该状态后无论UE在CS/PS1模式还是CS / PS 2模式，IMSI仍然允许在非EPS进行附着。
 * 但UE在CS/PS1模式时会选择GERAN或UTRAN进行无线接入，在这种情况下，UE不能重选至E-UTRAN进行业务。
 * 而如果UE为CS/PS2模式则根据3GPP TS 23.122 [ 6 ]进行PLMN重新选择并进行TA / LA联合更新附着
 */
#define GSM_REJECT_CAUSE_STR_14     "EPS services not allowed in this PLMN"
/**
 * @brief 15
 *    目前实测,被锁的卡(不确定还是否有流量),是返回该值
 * UE将EPS更新状态为不允许漫游并将重置跟踪区更新尝试计数器，UE应将当前的TAI存储在“禁止漫游的跟踪区域”列表中，并将当前的TAI从存储的TA列表中删除。
 * UE进入emm-deregistered.limited-service状态。然后UE根据3GPP TS 26.304 [21 ]在同一PLMN中的新的TA/LA区中的一个新小区进行联合TA / LA更新和IMSI附着。
 */
#define GSM_REJECT_CAUSE_STR_15     "no suitable cells in tracking area"
/**
 * @brief 35
 * TODO. 后续补充
 */
#define GSM_REJECT_CAUSE_STR_35     "Requested service option not authorized in this PLMN"

typedef struct GSM_REJECT_CAUSE_ST_TAG{
    NK_Int i32RejectCauseId;
    NK_Char *szRejectReason;
}GSM_REJECT_CAUSE_ST;

#define GSM_REGISTER_TIMEOUT_SEC        (120)   //注网超时时间,至少1min以上
#define GSM_DEFAULT_CID                 (1)     //默认激活第1路cid
#define GSM_SECOND_CID                  (5)     //不支持nat的模块,需激活第5路cid
#define GSM_MCC_MAX_LENGTH              (3)     //国家码最大长度
#define GSM_MNC_MAX_LENGTH              (3)     //网络码最大长度
#define GSM_IMEI_MIN_LEN                (15)    //IMEI最小长度
#define GSM_APN_MAX_LENGTH              (100)
#define GSM_USERNAME_MAX_LENGTH         (64)
#define GSM_PASSWORD_MAX_LENGTH         (64)

typedef struct
{
    NK_Int ip_version;                          //ip类型 @ref GSM_PDP_TYPE_E
    NK_Char apn_name[GSM_APN_MAX_LENGTH];
    NK_Char username[GSM_USERNAME_MAX_LENGTH];
    NK_Char password[GSM_PASSWORD_MAX_LENGTH];
    NK_Int auth_type;
}GSM_PDN_CONF_S;


/**
 * @brief GSM 模块初始化,网络注册成功、pdp掉激活等事件上报处理、双卡切换逻辑处理
 * @return NK_TRUE:成功
 * @return NK_FALSE:失败
 */
NK_Int APIS_Gsm_Init(NK_Void);

/**
 * @description: 获取模块的IMEI
 * @param imei [in,out] 存储IMEI的内存指针
 * @param size  [in,out] IMEI指向的内存长度，以字节为单位，传出imei字符串的长度
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetImei(NK_PChar imei, NK_Int *size);
/**
 * @description: 获取模块的IMSI
 * @param imei [in,out] 存储IMSI的内存指针
 * @param size  [in,out] IMSI指向的内存长度，以字节为单位，传出imsi字符串的长度
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetImsi(NK_PChar imsi, NK_Int *size);
/**
 * @description: 获取4G信号rssi和误码率(兼容2G和4G)
 * @param rssi [in,out]
 * @param ber [in,out]
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetSignalStrength(NK_Int *rssi, NK_Int *ber);
/**
 * @brief 获取4G信号 csq(兼容2G和4G)
 * @param nSim SIM卡索引[0, 1]
 * @param pCsq 返回信号强度csq[0, 31],失败返回99
 * @return NK_Int 成功返回NK_TRUE,失败返回NK_FALSE
 */
NK_Int APIS_Gsm_GetCsq(NK_UInt8 nSim, NK_UInt8 *pCsq);
/**
 * @description: 获取SIM卡的ICCID
 * @param id [in,out] 存储ICCID的内存指针
 * @param size  [in,out] ICCID指向的内存长度，以字节为单位 
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetIccid(NK_PChar id, NK_Int *size);
/**
 * @description: 获取固件版本号，由厂商维护
 * @param version [in,out] 存储版本号的内存指针
 * @param size  [in,out] version指向的内存长度，以字节为单位。传出版本号的实际长度
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetVersion(NK_PChar version, NK_Int *size);
/**
 * @description 设置飞行模式使能
 * @param bEnable [in] ：NK_TRUE 打开飞行模式； NK_FALSE: 切回正常模式
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int  APIS_Gsm_SetFlightMode(NK_Int bEnable);
/**
 * @description 获取实时APN
 * @param nSim [in] SIM卡索引，取值0-1
 * @param cid  [in] IDP索引，取值1-7
 * @param m_defaultApn [out]：存储APN的内存指针
 * @param len [int]：存储APN的内存大小
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetRealApn(NK_UInt8 nSim, NK_Byte cid, NK_PChar m_defaultApn, NK_Int len);

/**
 * @description 获取当前PDP是否激活
 * @param nSim [in] SIM卡索引，取值0-1
 * @param pdpid  [in] IDP索引，取值1-7
 * @return NK_TRUE：激活
 * @return NK_FALSE：未激活
 */
NK_Int APIS_Gsm_IsPdpActive(NK_UInt8 nSim, NK_Int pdpid);
/**
 * @description 激活PDP上下文
 * @param nSim [in] SIM卡索引，取值0-1
 * @param cid  [in] IDP索引，取值1-7
 * @param pdptype [in] IP类型：@ref GSM_PDP_TYPE_E
 * @param apn  [in] apn名称
 * @param Usr  [in] 用户名
 * @param Pwd  [in] 密码
 * @param nEncrypt  [in] 加密方式
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_SetPdpActive(NK_UInt8 nSim, NK_Byte cid, GSM_PDP_TYPE_E pdptype, NK_PChar apn, NK_PChar Usr, NK_PChar Pwd,NK_Int nEncrypt);


/**
 * @brief 设置RRC释放时间间隔，以降低功耗
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_SetRRCRel();

/**
 * @brief 设置NAT使能，使能nat后，不需要再双路拨号
 * @param bNeedReboot [out] 是否要重启
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_SetNatEnable(NK_Boolean *bNeedReboot);

/**
 * @brief 获取NAT使能开关
 * @param bEnable [out] 是否使能
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetNatEnable(NK_Boolean *bEnable);

/**
 * @description: 获取芯片温度值
 * @param 无
 * @return >0 温度值
 * @return -1 失败
 */
NK_Int APIS_Gsm_GetChipTemp();

/**
 * @description 获取指定sim卡缓存状态
 * @param nSimId： [in] SIM卡索引，取值0-1
 * @param nStatus  [out] sim卡状态 @ref SIM_STATUS_E
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetSIMStatus(NK_UInt8 nSimId, SIM_STATUS_E *nStatus);

/**
 * @description 获取指定sim卡实时状态
 * @param nSimId： [in] SIM卡索引，取值0-1
 * @param nStatus  [out] sim卡状态 @ref SIM_STATUS_E
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetSIMRealTimeStatus(NK_UInt8 nSimId, SIM_STATUS_E *nStatus);

/**
 * @description 获取设备型号。
 * @param model [out] 存储model的内存指针
 * @param size  [out] model指向的内存长度，以字节为单位，传出model字符串的长度
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetModuleModel(NK_PChar model, NK_Int *size);
/**
 * @description 关闭PDP上下文
 * @param nSim [in] SIM卡索引，取值0-1
 * @param cid  [in] IDP索引，取值1-7
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_SetPdpDeactive(NK_UInt8 nSim, NK_Byte uCid);
/**
 * @description: 设置LTE连接基站(注网)超时
 * @param timeout_minute [in]　超时时长,单位分钟
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_SetLTEReigsterTimeout(NK_Int timeout_minute);
/**
 * @description: 获取当前的基站连接超时设置
 * @param timeout_minute [out] 超时时长
 * @return NK_TRUE：成功
 * @return NK_FALSE：失败
 */
NK_Int APIS_Gsm_GetLTETimeout(int *timeout_minute);

/**
 * @brief 获取经纬度信息,如果不支持,可改为上报lac、cid
 * @param longitude: 不支持可上报 lac
 * @param latitude : 不支持可上报 cid
 * @return 成功返回NK_TRUE, 失败返回NK_FALSE
 */
NK_Int APIS_Gsm_GetLocation(NK_Float *longitude, NK_Float *latitude);

/**
 * @brief 是否支持注网成功事件回调
 * @return NK_Int 支持返回NK_TRUE; 不支持返回NK_FALSE
 */
NK_Int APIS_Gsm_SupportRegisterReport(NK_Void);

/**
 * @brief sim卡插拔定时检测回调,内部要实现自动重启逻辑,提供给外部线程定时调用
 * @param bNeedAudio: 是否需要播报开机提示音
 * @param pSimId: 返回设备当前的sim卡索引 0-1
 * @return NK_Void
 */
NK_Void APIS_Gsm_SimSwitchDetOnTime(NK_Boolean bNeedAudio, NK_Int *pSimId);

/**
 * @brief 检测注网状态,如果设备不支持注网事件上报,NK_NetWork模块会轮询检测注网状态
 * @param retRegStatus 注网成功返回NK_TRUE, 注网失败返回NK_FALSE
 * @return NK_Int 获取成功返回NK_TRUE; 获取失败返回NK_FALSE
 */
NK_Int APIS_Gsm_CheckRegister(NK_Int *retRegStatus);
/**
 * @brief 获取netinfo,用于P2P shell
 * @return  NK_TRUE
 */
NK_Int APIS_Gsm_Getnetinfo();

/**
 * @brief 注网前获取当前SIM卡的PLMN,以便设置对应的APN
 * @param nSim [in] sim id
 * @param pMcc [out] 返回mcc
 * @param pMnc [out] 返回mnc
 * @return NK_Int 成功返回NK_TRUE, 失败返回NK_FALSE
 */
NK_Int APIS_Gsm_GetMccMnc(NK_UInt8 nSim, NK_Int *pMcc, NK_Int *pMnc);

/**
 * @brief 获取默认承载pdn配置
 * @param nSim  [in] sim id
 * @param pConf [out] 返回pdn配置信息 @ref GSM_PDN_CONF_S
 * @return NK_Int 成功返回NK_TRUE, 失败返回NK_FALSE
 */
NK_Int APIS_Gsm_GetDefaultPdnCfg(NK_UInt8 nSim, GSM_PDN_CONF_S *pConf);

/**
 * @brief 设置默认承载pdn配置,重启生效
 * @param nSim  [in] sim id
 * @param pConf [in] 设置的pdn配置信息 @ref GSM_PDN_CONF_S
 * @param bNeedReboot [out] 设置后是否要重启生效
 * @return NK_Int 成功返回NK_TRUE, 失败返回NK_FALSE
 */
NK_Int APIS_Gsm_SetDefaultPdnCfg(NK_UInt8 nSim, GSM_PDN_CONF_S *pConf, NK_Boolean *bNeedReboot);

/**
 * @brief 获取xlat使能
 * @param nSim     [in] sim id
 * @param pbEnable [out] 返回是否开启xlat使能
 * @return NK_Int 成功返回NK_TRUE, 失败返回NK_FALSE
 */
NK_Int APIS_Gsm_GetXlatEnable(NK_UInt8 nSim, NK_Boolean *pbEnable);

/**
 * @brief 设置xlat开关
 * @param nSim         [in] sim id
 * @param bEnable      [in] 设置xlat使能
 * @param pbNeedReboot [out] 返回是否要重启生效
 * @return NK_Int 成功返回NK_TRUE, 失败返回NK_FALSE
 */
NK_Int APIS_Gsm_SetXlatEnable(NK_UInt8 nSim, NK_Boolean bEnable, NK_Boolean *pbNeedReboot);

/**
 * @brief 设置需要操作的SIM卡卡槽ID,默认值为GSM_SIMNUM_0; 仅适用于双卡单待应用，以切换操作的物理卡槽
 * @param nSim      [in] SIM卡卡槽ID
 * @return NK_Int 成功返回NK_TRUE, 失败返回NK_FALSE
*/
NK_Int APIS_Gsm_SetSimOperateId(GSM_SIMNUM_E nSim);

/**
 * @brief 获取当前操作的SIM卡卡槽ID;仅适用于双卡单待应用
 * @return GSM_SIMNUM_E 返回SIM id
 */
GSM_SIMNUM_E APIS_Gsm_GetSimOperateId(void);
/*
    检测sim0卡是否在位
    @return -1 未知
            0 不在
            1 在位
*/
NK_Int APIS_Gsm_CheckSimDet();
/**
 * @brief 等待网络注网超时
 * @param nSim         [in] sim id
 * @param waittime      [in] 等待时间 单位s
 * @return 注网成功返回NK_TRUE, 失败返回NK_FALSE
 */
NK_Int APIS_Gsm_WaitRegister(NK_UInt8 nSim,NK_UInt64 waittime);

/**
 * @brief 判断当前使用的apn是否异常
 * @return 异常返回NK_RUE,正常返回NK_FALSE
 */
NK_Int APIS_Gsm_IsApnError();

NK_Int APIS_get_signal_info(int *snr,int *rsrp,int *rsrq,int *rssi);

NK_CPP_EXTERN_END
#endif
