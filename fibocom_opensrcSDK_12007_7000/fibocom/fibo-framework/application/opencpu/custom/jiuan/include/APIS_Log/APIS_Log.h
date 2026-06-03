/**
 * @Description  : 日志信息模块
 * @Version      : 1.0
 * @Author       : ZhongXiaoHui
 * @Date         : 2021-10-12 15:10:09
 * @copyright    : Copyright (C) JUAN. All rights reserved.
 **/
/**
 * 日志信息控制单体模块。
 */

#if !defined(APIS_LOG_H_)
#define APIS_LOG_H_

#include "APIS_Macro.h"

NK_CPP_EXTERN_BEGIN

#include <stdio.h>

#ifndef OSI_LOCAL_LOG_LEVEL
#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG
#endif


/**
 * @macro
 *  单行日志输出最大长度，当单行日志输出长度大于此值时会自动截断。
 */
#define NK_LOG_MAX_LINE_SZ (1024)

/**
 * @macro
 *  日志内部缓冲容量，当内部缓冲达到此容量时会触发 onFlush 事件清除缓冲区部分数据
 *  清除的数据大小为 NK_LOG_FLUSH_SZ
 */
#define NK_LOG_CACHE_SZ    (1024 * 16)

#define NK_LOG_FLUSH_SZ    (NK_LOG_CACHE_SZ / 3)

/**
 * 日志处理等级。
 */
typedef enum Nk_LogLevel
{
    NK_LOG_ALL = (0),
    NK_LOG_LV_DEBUG,
    NK_LOG_LV_INFO,
    NK_LOG_LV_WARN,
    NK_LOG_LV_ERROR,
    NK_LOG_LV_ALERT,
    NK_LOG_NONE,

} NK_LogLevel;


/**
 * Log 单体模块句柄。
 */
#pragma pack(push, 4)
struct Nk_Log {
    /**
     * @brief
     *  设置日志保存等级。\n
     * @brief
     *  设置等级后，要求满足输出等级小于等于设置等级才会进行保存，否则一律丢弃。\n
     *  日志保存路径由接口 @ref setLogPath 进行设置。\n
     *  如设置等级为 @ref NK_LOG_LV_NOTICE，则通过接口 @ref error 和接口 @ref notice 的日志模块才会处理，\n
     *  而接口 @ref info 和接口 @ref debug 的日志模块会自动丢弃。\n
     *  不设置的情况下，默认是 @ref NK_LOG_LV_ERROR 等级。
     *
     */
    NK_Void
    (*setLogLevel)(NK_LogLevel level);

    /**
     * @brief
     *  获取日志保存等级。\n
     */
    NK_LogLevel
    (*getLogLevel)(void);

    /**
     * 警戒日志记录。
     */
    NK_Void
    (*alert)(const NK_Char* fmt, ...);

    /**
     * 错误日志记录。
     */
    NK_Void
    (*error)(const NK_Char* fmt, ...);

    /**
     * 通知日志记录。
     */
    NK_Void
    (*warn)(const NK_Char* fmt, ...);

    /**
     * 信息日志记录。
     */
    NK_Void
    (*info)(const NK_Char* fmt, ...);

    /**
     * 调试信息记录。
     */
    NK_Void
    (*debug)(const NK_Char* fmt, ...);

    /**
     * @brief
     *  日志缓冲满冲刷事件。\n
     * @details
     *  参考 @ref flush。\n
     *  须要注意的是，事件实现中不能调用 NK_Log 模块相关接口，否则由可能由于程序死循环造成的崩溃。
     */
    NK_Void
    (*onFlush)();

    /**
     * @brief
     *  获取缓存的日志
     * @param level [in]
     *  当前消息的等级，必须在有效等级以内 FIXME.暂时不支持
     * @param msg [in]
     *  消息内容
     * @param stack_size[in]
     *  传入的msgbuff大小
     * @param data_len[out]
     *  输出的有效日志长度
     */
    NK_Int
    (*getLogCache)(NK_LogLevel level, NK_PChar msg, NK_Int stack_size, NK_Int *data_len);
};
#pragma pack(pop)

/**
 * 获取单体句柄。
 */
NK_API struct Nk_Log *
NK_Log(void);


/**
 * @brief
 *  当前模块内部堆内存的空间大小（单位：字节）。
 */
NK_API NK_Size
NK_Log_HeapUsage(void);

int printf(const char *format, ...);

#if 1
#define NK_TRACE(format, args...)   NK_Log()->alert("[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#define NK_DEBUG(format, args...)   NK_Log()->debug("[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#define NK_INFO(format, args...)    NK_Log()->info("[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#define NK_WARN(format, args...)    NK_Log()->warn("[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#define NK_ERROR(format, args...)   NK_Log()->error("[%s:%d]" format, __FUNCTION__, __LINE__, ##args)

#else

#include "osi_log.h"
#ifndef OSI_LOG_TAG
#define OSI_LOG_TAG OSI_MAKE_LOG_TAG(' ', ' ', 'J', 'A')
#endif
#define NK_TRACE(format, args...)   osiTracePrintf(OSI_LOG_TAG, "[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#define NK_DEBUG(format, args...)   osiTracePrintf(OSI_LOG_TAG, "[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#define NK_INFO(format, args...)    osiTracePrintf(OSI_LOG_TAG, "[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#define NK_WARN(format, args...)    osiTracePrintf(OSI_LOG_TAG, "[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#define NK_ERROR(format, args...)   osiTracePrintf(OSI_LOG_TAG, "[%s:%d]" format, __FUNCTION__, __LINE__, ##args)
#endif


NK_CPP_EXTERN_END
#endif /* NK_UTILS_LOG_H_ */
