#ifndef LIB_PLATEFORM_UNISOC_INCLUDE_FFW_PLAT_HEADER
#define LIB_PLATEFORM_UNISOC_INCLUDE_FFW_PLAT_HEADER

#include <stdint.h>
//#include <sys/time.h>
#include <unilog.h>
#include <debug_trace.h>
#include <lwip/sockets.h>
#include "cmsis_os2.h"
#include "mem_zone.h"
#include "atc_decoder.h"
#include "at_util.h"
#include "atc_reply.h"
#include "wdt.h"
#include "mem_map.h"
// #include <lwip/netdb.h>
// #include <errno.h>
// #include "fupdate.h"
#ifdef CONFIG_FFW_TTS_SUPPORT
#include "ivTTS.h"
#endif

#if defined(FIBOCOM_OTA_DELTA_CHECK)
extern bool fibo_fota_delta_check(void);
#endif

extern void delay_us(uint32_t us);

/**
 * thread priority
 *
 * The definition is independent of implementation. Though some
 * implementation will use larger value for higher priority and
 * others will use smaller value for highe priority, this enum will
 * use larger value for higher priority.
 *
 * \p OSI_PRIORITY_IDLE and \p OSI_PRIORITY_HISR are reserved, can't
 * be used.
 *
 */
typedef enum
{
    FFW_OSI_PRIORITY_IDLE = 1, // reserved
    FFW_OSI_PRIORITY_LOW = 8,
    FFW_OSI_PRIORITY_BELOW_NORMAL = 16,
    FFW_OSI_PRIORITY_NORMAL = 24,
    FFW_OSI_PRIORITY_ABOVE_NORMAL = 32,
    FFW_OSI_PRIORITY_HIGH = 40,
    FFW_OSI_PRIORITY_REALTIME = 48,
    FFW_OSI_PRIORITY_HISR = 56, // reserved
} ffw_osi_priority_t;

typedef enum
{
    FFW_OSI_RESET_REASON_CLEAR = 0x0,
    FFW_OSI_RESET_REASON_HARDFAULT,
    FFW_OSI_RESET_REASON_ASSERT,
    FFW_OSI_RESET_REASON_WDT,
    FFW_OSI_RESET_REASON_BAT,
    FFW_OSI_RESET_REASON_TEMP,
    FFW_OSI_RESET_REASON_FOTA,
    FFW_OSI_RESET_REASON_MAX = 0x10,   
} ffw_reset_reason_t;


/// 文件分隔符的定义
#define FFW_FS_SEP "/"

/// FFW操作文件的文件基目录
#if defined(TYPE_EC618S)
#define FFW_BASE_DIRNAME "/"
#else
#define FFW_BASE_DIRNAME "/fibocom"
#endif

/// 文件操作句柄类型的定义
typedef void * ffw_fs_handle_t;

/// 无效文件句柄的定义
#define FFW_FS_INVALID_HANDLE NULL

//int ffw_log(const char *fmt, ...);
/**
* @brief 打印日志函数
* 
* @param fmt 格式化字符串
* @param ... 
 */
//#define ffw_log(format, ...) swLogPrintf(123456, P_INFO, ##__VA_ARGS__);

//int ffw_rat_get_cid_by_context(int context_id);

//extern char *strcasestr (const char *, const char *);

#endif /* LIB_PLATEFORM_UNISOC_INCLUDE_FFW_PLAT_HEADER */
