/**
 * @Description  : 升级外部接口
 * @Version      : 1.0
 * @Author       : ZhongXiaoHui
 * @Date         : 2021-11-26 13:46:53
 * @copyright    : Copyright (C) JUAN. All rights reserved.
 **/
#if !defined(_APIS_UPGRADE_H_)
#define _APIS_UPGRADE_H_
NK_CPP_EXTERN_BEGIN

typedef enum en_UPGRADE_TYPE
{
    UPGRADE_TYPE_ROM,
    UPGRADE_TYPE_FOTA,
}UPGRADE_TYPE;

typedef struct en_UpgradeInfo{
    NK_Int fw_size; //升级包大小
    UPGRADE_TYPE upgradetype;
}UpgradeInfo;

struct APIS_UPGRADE_S{

    /**
     * @brief 初始化升级模块，清除flash内的上次升级遗留的差分包，避免下次升级、或保存flash参数空间不足
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*init)(NK_Void);

    /**
     * @brief 反初始化升级模块
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*deinit)(NK_Void);

    /**
     * @brief 升级前调用，检测系统空间、内存是否充足，满足条件则切到升级状态
     * @param[in] fw_size：固件大小
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*prepare)(UpgradeInfo info);

    /**
     * @brief 下载固件，接收固件数据到4Gflash 或 ram
     * @param[in] offset 当前数据相对整个升级固件包起始位置的偏移
     * @param[in] pu8Buf 升级固件数据
     * @param[in] u32Len 升级固件数据长度
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*download)(NK_UInt32 offset, NK_UInt8 *pu8Buf, NK_UInt32 u32Len);

   /**
     * @brief 下载成功后的处理，校验大小、md5(是否校验md5，根据平台处理，因固件较大的情况下，校验md5要消耗较大的内存，如果固件也是放在内存，可能不够内存)
     * @param[in] md5 固件的md5
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*downloadOk)(NK_PChar md5);

   /**
     * @brief 下载失败后的处理，释放升级前申请的内存
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*downloadFail)();

    /**
     * @brief 固件安装
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*install)();

};


NK_API struct APIS_UPGRADE_S *APIS_Upgrade(void);


NK_CPP_EXTERN_END
#endif