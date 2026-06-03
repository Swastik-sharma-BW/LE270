/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/

#ifndef _KEDA_API_H_
#define _KEDA_API_H_

#include <stdint.h>
#include <stdlib.h>

typedef enum {
SC_SIM_RETURN_SUCCESS,
SC_SIM_RETURN_FAIL,
SC_SIM_RTEURN_UNKNOW
}SC_simcard_err_e;

typedef struct
{
    void *disk;
} SCFILE;

typedef enum {
    SC_FTPS_USB,
    SC_FTPS_UART
}SC_PDP_ACTIVE_TYPE;

typedef struct{
    char host[256]; /*xx.xx.xx.xx:port, or URL*/
    char username[256]; /*note: qijun require 256, need turn*/
    char password[256];
    INT32 port;
    UINT8 serverType; /*0: FTP client, 1:FTPS SSL client 2:FTPS TLS client 3:implicit FTPS client*/
}SCftpsLoginMsg;
typedef enum{
    SC_FTPS_FILE_FLASH = 1,
    SC_FTPS_FILE_SDCARD,
    SC_FTPS_FILE_EXT_FLASH_FS
}SC_FTPS_FILE_LOCATION;


typedef enum {
    SC_FTPS_RESULT_OK,// 0
    SC_FTPS_RESULT_SSL_ALERT,// 1
    SC_FTPS_RESULT_UNKNOWN_ERROR, // 2
    SC_FTPS_RESULT_ERROR_BUSY,    // 3
    SC_FTPS_RESULT_SERVER_CLOSED,  // 4
    SC_FTPS_RESULT_ERROR_TIMEOUT,// 5
    SC_FTPS_RESULT_ERROR_TRANSFER_FAILED,   // 6  /*receive/send socket data failed*/
    SC_FTPS_RESULT_ERROR_MEMORY_ERROR,// 7
    SC_FTPS_RESULT_ERROR_INVALID_PARAM,  // 8
    SC_FTPS_RESULT_ERROR_REJ_BY_SERVER,  // 9
    SC_FTPS_RESULT_NETWORK_ERROR,  //10
    SC_FTPS_RESULT_STATE_ERROR,// 11
    SC_FTPS_RESULT_FAILED_TO_PARSE_SERVER_NAME, // 12
    SC_FTPS_RESULT_FAILED_TO_CREATE_SOCKET, // 13
    SC_FTPS_RESULT_FAILED_TO_CONNECT_SOCKET, //14
    SC_FTPS_RESULT_FAILED_TO_CLOSE_SOCKET, // 15
    SC_FTPS_RESULT_FAILED_TO_PEER_CLOSED,// 16
    SC_FTPS_RESULT_ERROR_FILE_ERROR,// 17
    SC_FTPS_DO_NOTHING,
    SC_FTPS_RESULT_ERR_SESSION_NULL,
    SC_FTPS_RESULT_ERR_CLOSE_SSL,
    SC_FTPS_RESULT_SERVER_CONN_TIMEOUT_ERROR = 421,
    SC_END_OF_MFTP_RESULT
}SC_FTPS_ERROR_CODE;
typedef SC_FTPS_ERROR_CODE SC_FTPS_RETURNCODE;


//API Network
int sKDWL_NetworkGetCgreg(int* pGreg);
SC_simcard_err_e sKDWL_SysGetIccid(char *IccidValue);
int sKDWL_NetworkGetCsq(UINT8 *pCsq);

//API msg qunue
typedef void   *sMsgQRef;
INT32 sKDWL_MsgQCreate(sMsgQRef *msgQRef, UINT32 msg_count, UINT32 msg_size);
INT32 sKDWL_MsgQSend(sMsgQRef msgQRef, const void *msg);
INT32 sKDWL_MsgQRecv(sMsgQRef msgQRef,void *msg, UINT32 timeout);
INT32 sKDWL_MsgQPoll(sMsgQRef msgQRef,UINT32* msg_id_release);

//API mutex
typedef void   *sMutexRef;
INT32 sKDWL_MutexCreate(sMutexRef *mutex_id, UINT32 time);
INT32 sKDWL_MutexLock(sMutexRef mutex_id);
INT32 sKDWL_MutexUnLock(sMutexRef mutex_id);

//API Task
typedef void   *sTaskRef;
INT32 sKDWL_TaskCreate(sTaskRef *taskRef, void *stackPtr, \
                        UINT32 stackSize,UINT8 priority,    \
                        char *taskName,void (*taskStart)(void *),void *argv);
void sKDWL_TaskResume(sTaskRef taskRef);
void sKDWL_TaskSuspend(sTaskRef taskRef);
void sKDWL_TaskSleep(UINT32 ticks);

void *sKDWL_Malloc(UINT32 Size);
void sKDWL_Free(void *buffer);

//API reset
INT32 sKDWL_SysReset();

//API Sleep
typedef enum
{
    SC_SYSTEM_SLEEP_DISABLE     =   0,
    SC_SYSTEM_SLEEP_ENABLE      =   1
}SC_SYSTEM_SLEEP_FLAG;
int sKDWL_SystemSleepSet(SC_SYSTEM_SLEEP_FLAG flag);

//API AT
int sKDWL_AtSend(const UINT8 *cmd, UINT16 len);

//API sys_ticks
UINT32 sKDWL_GetTicks(void);

//API sw_version
char *sKDWL_GetSwVersion(void);

//API aes
INT32 sKDWL_AesDecrypt(const UINT8 * key,UINT32 keybits,const UINT8 *input, UINT8 *output);
INT32 sKDWL_AesEncrypt(const UINT8 * key,UINT32 keybits,const UINT8 *input, UINT8 *output);

//API file
SCFILE *sKDWL_fopen(const char *fname, const char *mode);
int sKDWL_fclose(SCFILE *fp);
int sKDWL_fread(void *buffer, size_t size, size_t num, SCFILE *fp);
int sKDWL_fwrite(const void *buffer, size_t size, size_t num, SCFILE *fp);
int sKDWL_fseek(SCFILE *fp, long offset, int whence);
int sKDWL_fsync(SCFILE *fp);
int sKDWL_fsize(SCFILE *fp);
int sKDWL_remove(const char *fname);
long long sKDWL_GetFreeSize(char *disc);

//API APP OTA
INT32 sKDWL_AppPackageSize(void);
INT32 sKDWL_AppPackageRead(UINT8 *data, UINT32 len);
INT32 sKDWL_AppPackageDl(UINT8 *url);
INT32 sKDWL_AppPackageUpdate(INT8 *data, UINT32 len);

// API FTP
void sKDWL_FtpsInit(SC_PDP_ACTIVE_TYPE type,sMsgQRef msgQRef);
void sKDWL_FtpsDeInit(SC_PDP_ACTIVE_TYPE TYPE);
SC_FTPS_RETURNCODE sKDWL_FtpsLogin(SCftpsLoginMsg msg);
SC_FTPS_RETURNCODE sKDWL_FtpsLogout(void);
SC_FTPS_RETURNCODE sKDWL_FtpsGetCurrentDirectory(void);
SC_FTPS_RETURNCODE sKDWL_FtpsUploadFile(char* fileName,SC_FTPS_FILE_LOCATION loc,int startPos);
#endif
