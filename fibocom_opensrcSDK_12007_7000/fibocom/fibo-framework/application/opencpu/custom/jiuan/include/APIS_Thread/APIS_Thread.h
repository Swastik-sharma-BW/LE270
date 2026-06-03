/*****************************************************************************
 * Description  : 线程基础库
 * Version      : 1.0
 * Author       : ZhongXiaoHui
 * Date         : 2021-10-08 21:08:14
 * FilePath     : \\4G_SLM322\\app\\Include\\Modules\\NK_Platform\\APIS_Thread\\APIS_Thread.h
 * copyright    : Copyright (C) JUAN. All rights reserved.
 ******************************************************************************/

#ifndef APIS_THREAD_H_
#define APIS_THREAD_H_

typedef  NK_PVoid HTHREAD;

typedef void *(*APIS_PTHREAD_FUNC)(void *);

/*
 * The definition is the same as CMSIS-RTOS.
 */
typedef enum tagThreadPriority_E
{
    THR_PRIORITY_LOW = 1,
    THR_PRIORITY_BELOW_NORMAL,
    THR_PRIORITY_NORMAL,
    THR_PRIORITY_ABOVE_NORMAL,
    THR_PRIORITY_HIGH,
    THR_PRIORITY_REALTIME,
} ThreadPriority_E;


HTHREAD     APIS_THR_Create(ThreadPriority_E nThrPriority,
                            NK_UInt32 nThrStackSize,
                            APIS_PTHREAD_FUNC pFunc,
                            NK_PVoid arg,
                            NK_PChar pThrName);
NK_Int      APIS_THR_Destroy(HTHREAD *hThread);

NK_Boolean  APIS_THR_Triger(HTHREAD hThread);
NK_Boolean  APIS_THR_Sleep(HTHREAD hThread, NK_UInt32 ms);
NK_Void     APIS_THR_Cancel(HTHREAD hThread);
NK_PVoid    APIS_THR_GetUserCtx(NK_PVoid hThread);

NK_Int      APIS_THR_Lock(HTHREAD hThread);
NK_Int      APIS_THR_UnLock(HTHREAD hThread);
NK_Void     APIS_THR_Suspend(HTHREAD hThread);
NK_Void     APIS_THR_Resume(HTHREAD hThread);

//新增接口
NK_Void     APIS_THR_SetPriority(NK_Int priority);
NK_UInt32  APIS_THR_CurrentId();

#define APIS_THR_CreateEx(pFunc,arg,pFuncName) APIS_THR_Create(0, 0, pFunc, arg, pFuncName)

#define DEF_THREAD_PRI 99

#endif
