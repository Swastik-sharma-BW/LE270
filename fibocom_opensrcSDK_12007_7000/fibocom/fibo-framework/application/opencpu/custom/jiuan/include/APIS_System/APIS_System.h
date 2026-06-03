/*****************************************************************************
 * Description  : system基础头文件
 * Version      : 1.0
 * Author       : ZhongXiaoHui
 * Date         : 2021-10-09 11:16:33
 * copyright    : Copyright (C) JUAN. All rights reserved.
 ******************************************************************************/

#ifndef APIS_SYSTEM_H_
#define APIS_SYSTEM_H_
#include "APIS_Macro.h"

NK_CPP_EXTERN_BEGIN

/**
* @brief 等待时间为永久等待
*/
#define NK_SYS_WAIT_FOREVER         (0xffffffffUL)
#define NK_SYS_SEMAPHORE_MAX_CNT    (65535)
#define NK_SYS_RTC_SYNC_INTERVAL    (5)             //RTC误差达到此值后进行同步,单位s

/**
* @brief 内存统计模块数量/ID
*/
typedef enum tagOSMEMORYID{
    OS_MEMORY_ID_START,
    OS_MEMORY_ID_SYS    = OS_MEMORY_ID_START,
    OS_MEMORY_ID_LPCM,
    OS_MEMORY_ID_AT,
    OS_MEMORY_ID_P2P,
    OS_MEMORY_ID_REVERSED,

    OS_MEMORY_ID_MAX = 5
}OSMEMORYID_E;

typedef enum tagOsBootCause
{
    OS_BOOTCAUSE_UNKNOWN = 0,           ///< placeholder for unknown reason
    OS_BOOTCAUSE_PWRKEY,                ///< boot by power key
    OS_BOOTCAUSE_PIN_RST,               ///< boot by pin reset
    OS_BOOTCAUSE_ALARM,                 ///< boot by alarm
    OS_BOOTCAUSE_CHARGE,                ///< boot by charge in
    OS_BOOTCAUSE_WDG = 5,               ///< boot by watchdog
    OS_BOOTCAUSE_PIN_WAKEUP,            ///< boot by gpio wakeup
    OS_BOOTCAUSE_PANIC,                 ///< boot by panic reset
    OS_BOOTCAUSE_PSM_WAKEUP,            ///< boot by psm wakeup
    OS_BOOTCAUSE_RTC,                   ///< boot by RTC
    OS_BOOTCAUSE_API,                   ///< boot by API
}OSBootCause_E;

/**
* @brief 系统时间结构体
*/
typedef struct FwTime
{
	NK_Int year, mon, mday;
	NK_Int hour, min, sec;
}FwTime_t;

/**
* @brief 内存统计结构体
*/
typedef struct
{
    NK_Int32  s32Count;  ///< 内存数量
    NK_Int32  s32Size;   ///< 内存大小
    NK_Char   szName[8]; ///< 模块名称
    NK_Int32  s32UseCount; ///< 内存申请次数
    NK_Int32  s32FreeCount;///< 内存释放次数
}OSMemory_S;

//time[2000-01-01 00:00:00-----2100-01-01 00:00:00]
typedef struct ql_rtc_time_struct
{
    int tm_sec;	 // seconds [0,59]
    int tm_min;	 // minutes [0,59]
    int tm_hour; // hour [0,23]
    int tm_mday; // day of month [1,31]
    int tm_mon;	 // month of year [1,12]
    int tm_year; // year [2000-2100]
    int tm_wday; // wday [0-6],sunday = 0,this value has no effect when setting the time
}ql_rtc_time_t;

typedef NK_Void (*SYS_TIMER_CALLBACK)(NK_Void *ctx);

typedef enum{
    SIM_STATUS_UNKNOW,
    SIM_STATUS_ABSENT,
    SIM_STATUS_NORMAL
}SIM_STATUS_E;

/**
* @brief SYS模块初始化
* @param NK_Void
* @return 成功: NK_TRUE 失败：NK_FALSE
*/
NK_Int32 APIS_Sys_Init(NK_Void);

/**
* @brief 当前线程休眠一段时间
* @param u32TimeMs [IN] 休眠的时间，单位：ms
* @note !!!此接口会放弃CPU占用!!!
*/
NK_Void APIS_Sys_SleepMs(NK_UInt32 u32TimeMs);

/**
* @brief busy loop delay,主要用于控制驱动时序
* @param u32TimeUs [IN] 休眠的时间，单位：us
* @note !!!此接口不会放弃CPU占用!!!
*/
NK_Void APIS_Sys_DelayUs(NK_UInt32 u32TimeUs);

/**
 * @brief 重启系统
 * @param enReason [in] 重启原因, @ref NKCFGRebootReason_E
*/
NK_Void APIS_Sys_Reboot(NK_Int enReason);

/**
 * @brief 创建定时器
 * @param hTimer [out] 创建成功返回定时器句柄
 * @param pCallBack [in] 定时回调
 * @param pCtx [in] 传入回调的参数
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_TimerCreate(NK_PVoid *hTimer, SYS_TIMER_CALLBACK pCallBack, NK_PVoid pCtx);

/**
 * @brief 销毁创建的定时器
 * @param hTimer [in] 定时器句柄
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_TimerDestroy(NK_PVoid hTimer);

/**
 * @brief 等待 ms后，启动定时器 hTimer
 *        如果要循环启动某个定时器，需在定时器到期后，再次调用此接口
 * @param hTimer [in] 定时器句柄
 * @param ms [in] 定时器执行间隔
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_TimerStart(NK_PVoid hTimer, NK_UInt32 ms);

/**
 * @brief 停止定时器
 * @param hTimer [in] 定时器句柄
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_TimerStop(NK_PVoid hTimer);

/**
 * @brief 创建互斥锁
 * @param[out] phLock 创建的互斥锁句柄指针
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_MutexInit(NK_PVoid *phLock);

/**
 * @brief 销毁互斥锁
 * @param[out] phLock 互斥锁句柄
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_MutexDeInit(NK_PVoid phLock);

/**
 * @brief 上锁，休眠直到加锁成功
 * @param[in] hLock 互斥锁句柄
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_MutexLock(NK_PVoid hLock);

/**
 * @brief 解锁
 * @param[in] hLock 互斥锁句柄
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_MutexUnlock(NK_PVoid hLock);

/**
 * @brief 上锁，加锁失败则立即返回不阻塞
 * @param[in] hLock 互斥锁句柄
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_MutexTryLock(NK_PVoid hLock);

/**
 * @brief 创建信号量
 * @param[out] phSem 创建的信号量索引号
 * @param[in] u32InitCnt 信号量初始资源数
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int32 APIS_Sys_SemInit(NK_PVoid *phSem, NK_UInt32 u32InitCnt);

/**
 * @brief 销毁信号量
 * @param[in] phSem 信号量索引号
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int32 APIS_Sys_SemDeInit(NK_PVoid *phSem);

/**
 * @brief 申请信号量,超时时间为forever时，会永远等待信号
 *        超时时间为其他有效值时，则超时后返回
 * @param[in] phSem 信号量索引号
 * @param[in] nTimeOut 超时时间，单位ms
 * @return 成功：NK_TRUE 失败/超时：NK_FALSE
*/
NK_Int32 APIS_Sys_SemAcquire(NK_PVoid phSem, NK_UInt32 nTimeOutMs);

/**
 * @brief 释放信号量
 * @param[in] phSem 信号量索引号
 * @return 成功：NK_TRUE 失败：NK_FALSE
*/
NK_Int32 APIS_Sys_SemRelease(NK_PVoid phSem);

/**
 * @brief 申请内存
 * @param[IN] u32ModuleID 调用模块ID，用于内部统计内存
 * @param[in] u32Size 需要申请的内存大小（单位：bytes）
 * @return 成功：内存首地址指针 失败：NK_Nil
*/
NK_PVoid APIS_Sys_Malloc(NK_UInt32 u32ModuleID, NK_UInt32 u32Size);

/**
 * @brief 释放内存
 * @param[IN] u32ModuleID 调用模块ID，用于内部统计内存
 * @param[in] pvData 内存首地址指针
*/
NK_Void APIS_Sys_Free(NK_UInt32 u32ModuleID, NK_PVoid pvData);

/**
 * @brief 获取内存堆分配信息
 * @return 只读的内存堆指针
*/
const OSMemory_S* APIS_Sys_GetHeapInfo(NK_Void);

/**
 * @brief 获取32位随机数
 *        使用前不需要设置随机数种子，内部实现会设置随机种子
 * @return 成功：随机数
*/
NK_Int32 APIS_Sys_GetRandom(NK_Void);

/**
 * @brief 关闭所有中断
 * @return 标记是否有嵌套开、关中断的操作
*/
NK_UInt32 APIS_Sys_DisableIrq(NK_Void);

/**
 * @brief 恢复所有中断
 * @param[in] u32Flag 如果有嵌套开、关中断的操作，在内层不会恢复中断，到最外层才恢复
*/
NK_Void APIS_Sys_EnableIrq(NK_UInt32 u32Flag);

/**
 * @brief 获取当前系统日期
 * @param[in] hDate 返回的日期结果
 * @return NK_TRUE:成功 NK_FALSE:失败
*/
NK_Int APIS_Sys_GetCurDate(FwTime_t *hDate);

/**
 * @brief 设置RTC时间
 * @param[in] rtcTime 设置的rtc时间，不受休眠影响，此时间必须包含时区时间
 * @return NK_TRUE:成功 NK_FALSE:失败
*/
NK_Int APIS_Sys_SetRtc(NK_UInt64 rtcTime);

/**
 * @brief 获取RTC时间
 * @param[in] pRtcTime 返回的rtc时间，不受休眠影响
 * @return NK_TRUE:成功 NK_FALSE:失败
*/
NK_Int APIS_Sys_GetRtc(NK_UInt64 *pRtcTime);

/**
 * @brief 获取模块重启原因
 * @param[out] *retBootCauseId 返回重启原因枚举值@ref OSBootCause_E
 * @param[out] szRebootReason 返回重启原因字符串
 * @param[in] nLen 重启原因字符串长度
 * @return NK_TRUE:成功 NK_FALSE:失败
*/
NK_Int APIS_Sys_GetRebootReason(OSBootCause_E *retBootCauseId, NK_PChar szRebootReason, NK_Int nLen);

/**
* @brief 系统关机,关机后能否响应按键唤醒等外部中断，需根据平台确认
* @param NK_Void
* @return 成功: NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_PowerOff(NK_Void);

/**
 * @brief 设置模块是否工作在低功耗模式，低功耗模式时，模块会在空闲一定时间后休眠
 * @param bEnable [in] 值为非0表示设备工作在低功耗模式，否则工作在常电模式
 * @return 成功: NK_TRUE 失败：NK_FALSE
 */
NK_Int APIS_Sys_SetLowPowerMode(NK_Int bEnable);

/**
 * @brief 设置usb使能
 * @param bEnable [in] 值为非0表示usb工作,0为空闲
 * @return 成功: NK_TRUE 失败：NK_FALSE
 */
NK_Int APIS_Sys_SetUsbEnable(NK_Int bEnable);

/***
** @ TODO. p2p暂不知道函数的含义注释
****/
NK_Int APIS_Sys_SendByUART(NK_PChar uart, NK_PChar data, NK_Int datalen);
NK_Int APIS_Sys_SetTimestamp(NK_UInt32 t);
/**
 * @brief liunx 执行语句
 * @param str_cmd [in] 执行得字符串
 * @return 成功: NK_TRUE 失败：NK_FALSE
*/
NK_Int APIS_Sys_System(NK_PChar str_cmd);

/**
 * @brief 创建唤醒锁
 * @param szLockName [in] 唤醒锁名字
 * @param nNameSize [in] 唤醒锁名字长度
 * @return 成功: 唤醒锁fd  失败返回-1
 */
NK_Int APIS_Sys_WakeLocklnit(const NK_Char *szLockName, NK_Int nNameSize);

/**
 * @brief 销毁唤醒锁
 * @param szLockName [in] 唤醒锁fd
 * @return 成功: NK_TRUE 失败：NK_FALSE
 */
NK_Int APIS_Sys_WakeLockDeinit(NK_Int fd);

/**
 * @brief 加唤醒锁
 * @param szLockName [in] 唤醒锁fd
 * @return 成功: NK_TRUE 失败：NK_FALSE
 */
NK_Int APIS_Sys_Wakelock(NK_Int fd);

/**
 * @brief 释放唤醒锁
 * @param szLockName [in] 唤醒锁fd
 * @return 成功: NK_TRUE 失败：NK_FALSE
 */
NK_Int APIS_Sys_WakeUnlock(NK_Int fd);

NK_CPP_EXTERN_END
#endif