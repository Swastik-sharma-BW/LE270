/**
 * @Description  : 看门狗
 * @Version      : 1.0
 * @Author       : yepeixin
 * @Date         : 2023-01-22 18:03:26
 * @copyright    : Copyright (C) JUAN. All rights reserved.
 **/
#if !defined(_APIS_WDT_H_)
#define _APIS_WDT_H_
#include "APIS_Export.h"
NK_CPP_EXTERN_BEGIN

typedef enum enAPISWTGTYPE_
{
    EN_APIS_WTG_FEED = 0x101,
    EN_APIS_WTG_UNKNOW,/////溢出类型
}enAPISWTGTYPE;


/**
 * @brief   开启看门狗
 * @return NK_FALSE 失败
 * @return NK_TRUE 成功
*/
NK_Int APIS_Wdt_Start(NK_Int sec);
/**
 * @brief   关闭看门狗
 * @return NK_FALSE 失败
 * @return NK_TRUE 成功
*/
NK_Int APIS_Wdt_Stop();
/**
 * @return NK_FALSE 失败
 * @return NK_TRUE 成功
*/
NK_Int APIS_Wdt_SetParam(enAPISWTGTYPE enmuType, NK_Int nValue);
NK_CPP_EXTERN_END
#endif