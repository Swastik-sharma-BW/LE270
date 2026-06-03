/**
 * @Description  : 
 * @Version      : 1.0
 * @Author       : ZhongXiaoHui
 * @Date         : 2023-02-23 21:28:32
 * @copyright    : Copyright (C) JUAN. All rights reserved.
 **/
#ifndef APIS_SYSTEMCTL_H_
#define APIS_SYSTEMCTL_H_
NK_CPP_EXTERN_BEGIN

#include "NK_Export.h"

typedef enum {
    ONLINE_WAKELOCK,
    THREAD_WAKELOCK,
    MODE_WAKELOCK,
    POWER_WAKELOCK,
    NET_WAKELOCK,
    LPCM_WAKELOCK,
    SYS_CHECK_WAKELOCK,
    SELECT_WAKELOCK,
}WAKELOCK_E;

struct APIS_SYSCTL_S{
    /**
    * @brief 初始化唤醒锁模块
    * @return NK_TRUE：成功
    * @return NK_FALSE：失败
    */
    NK_Int (*wakeLockInit)();

    /**
    * @brief 反初始化唤醒锁模块
    * @return NK_TRUE：成功
    * @return NK_FALSE：失败
    */
    NK_Int (*wakeLockDeinit)();

    /**
    * @brief 加唤醒锁
    * @param wakelockId  唤醒锁id
    * @return NK_TRUE：成功
    * @return NK_FALSE：失败
    */
    NK_Int (*wakeLock)(WAKELOCK_E wakelockId);

    /**
    * @brief 释放唤醒锁
    * @param wakelockId  唤醒锁id
    * @return NK_TRUE：成功
    * @return NK_FALSE：失败
    */
    NK_Int (*wakeUnlock)(WAKELOCK_E wakelockId);

    /**
     * @brief 清空所有正在等待的信号量,保证下一次调用信号量等待接口时能成功阻塞
     * @param phSem 创建的信号量句柄
     * @return NK_TRUE：成功
     * @return NK_FALSE：失败
     */
    NK_Int (*clearSem)(NK_PVoid phSem);
};


NK_API struct APIS_SYSCTL_S *APIS_SysCtl(void);

NK_CPP_EXTERN_END
#endif