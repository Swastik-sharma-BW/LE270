/**
 * @Description  : at命令外部接口
 * @Version      : 1.0
 * @Author       : ZhongXiaoHui
 * @Date         : 2021-10-21 18:03:37
 * @copyright    : Copyright (C) JUAN. All rights reserved.
 **/

#if !defined(APIS_ATCMD_H_)
#include "NK_Export.h"
#define APIS_ATCMD_H_

#define ATLENMAX 500
struct APIS_ATCMD_S{
    /**
     * @brief 初始化定制AT回调，以及实现所有定制AT回调接口
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*init)(NK_Void);
    /**
     * @brief 4G模块自行执行系统at指令, 因为很多接口不开放api的形式调用,只能通过AT的方式调用;另一方面远程shell查设备信息,通过AT可查询的内容更易于扩展
     *        目前主要是p2p shell远程执行AT命令、
     * @param cmd [in] 虚拟at指令
     * @param atResponse [out] 返回的at信息
     * @param len [in] atResponse的存储深度
     * @param pFinalRsp [in] 期望的返回值内容结尾标识符,如果没读到该标识符会继续读直到超时
     * @param nTimeout [in] 超时时间
     * @param nTry [in] 重发次数
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*execAtCmd)(NK_PChar cmd,NK_PChar atResponse, NK_Int len, NK_PChar pFinalRsp, NK_Int nTimeout, NK_Int nTry);
};


NK_API struct APIS_ATCMD_S *APIS_AtCmd(void);

#endif