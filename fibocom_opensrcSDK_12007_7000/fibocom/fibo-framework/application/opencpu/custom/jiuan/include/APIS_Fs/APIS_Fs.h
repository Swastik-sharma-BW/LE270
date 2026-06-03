/**
 * @Description  : 文件系统模块
 * @Version      : 1.0
 * @Author       : ZhongXiaoHui
 * @Date         : 2021-10-18 18:13:23
 * @copyright    : Copyright (C) JUAN. All rights reserved.
 **/
#ifndef APIS_FS_H_
#define APIS_FS_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "NK_Export.h"

#define NK_FS_SEEK_SET   (0)
#define NK_FS_SEEK_CUR   (1)
#define NK_FS_SEEK_END   (2)

typedef enum{
    EN_FS_CFG_TYPE_UNKNOW,
    EN_FS_CFG_TYPE_MCU,
    EN_FS_CFG_TYPE_APP,
    EN_FS_CFG_TYPE_APP_BAK,
    EN_FS_CFG_TYPE_MCU_V2,
}NKFSConfigType_E;

typedef enum
{
    EN_FS_O_RDONLY = (NK_UInt32)(1<<0),
    EN_FS_O_WRONLY = (NK_UInt32)(1<<1),
    EN_FS_O_RDWR   = (NK_UInt32)(1<<2),
    EN_FS_O_CREAT  = (NK_UInt32)(1<<3),
    EN_FS_O_EXCL   = (NK_UInt32)(1<<4),
    EN_FS_O_TRUNC  = (NK_UInt32)(1<<5),
    EN_FS_O_APPEND = (NK_UInt32)(1<<6),
} NKFSOpenMode_E;

struct APIS_FS_S{
    /**
    * @brief 打开一个文件
    * @param  ps32Fd   [OUT] 创建的文件描述符
    * @param  pcPath   [IN]  要打开或创建的目标文件
    * @param  enFlags  [IN]  文件打开模式
    * @return NK_TRUE：成功
    * @return NK_FALSE：失败
    */
    NK_Int32 (*open)(NK_Int32 *ps32Fd, const NK_Char *pcPath, NKFSOpenMode_E enFlags);

    /**
    * @brief 从文件中，读取指定大小的文件数据
    * @param  s32Fd  [IN]  文件描述符
    * @param  pu8Buf [OUT] 保存读取数据的缓冲区
    * @param  u32Len [IN]  需要读取的长度
    * @return >=0：成功, 返回成功读取的字节数
    * @return -1：失败
    * @note 缓冲区应由用户申请内存，并注意读取的长度不超过缓冲区大小
    */
    NK_Int32 (*read)(NK_Int32 s32Fd, NK_UInt8 *pu8Buf, NK_UInt32 u32Len);

    /**
    * @brief 向文件中，写入指定大小的文件数据
    * @param  s32Fd  [IN] 文件描述符
    * @param  pu8Buf [IN] 存放需要写入文件的数据
    * @param  u32Len [IN] 需要写入的长度
    * @return >=0：成功, 返回成功写入的字节数
    * @return -1：失败
    */
    NK_Int32 (*write)(NK_Int32 s32Fd, const NK_UInt8 *pu8Buf, NK_UInt32 u32Len);

    /**
     * reposition read/write file offset
     *
     * Refer to man (2) lseek
     *
     * \param [in] fd       file descriptor
     * \param [in] offset   offset according to the directive whence
     * \param [in] whence   SEEK_SET, SEEK_CUR, SEEK_END
     * \return >=0：成功, 返回成功偏移的字节数
     * return -1:失败
     */
    NK_Int32 (*lseek)(NK_Int32 s32Fd, NK_Int32 offset, NK_Int whence);

    /**
    * @brief 关闭一个已经打开的文件
    * @param  ps32Fd [IN] 文件描述符
    * @return NK_TRUE：成功
    * @return NK_FALSE:失败
    */
    NK_Int32 (*close)(NK_Int32 s32Fd);

    /**
    * @brief 删除指定的文件
    * @param  pcPath [IN] 文件路径
    * @return NK_TRUE：成功
    * @return NK_FALSE:失败
    */
    NK_Int32 (*remove)(NK_PChar pcPath);

    /**
     * @brief: 判断文件是否存在
     * @input: *path:文件路径
     * @return: NK_TRUE:文件存在; NK_FALSE:文件不存在
    **/
    NK_Int32 (*isExist)(const NK_Char *path);

    /**
     * @brief 从flash加载数据到ram,底层根据平台决定是从某个文件或者flash固定分区读取
     * @param type [in] 配置文件类型
     * @param pu8Buf [out] 用于缓存flash读到的数据，由接口外部分配内存
     * @param pu32Len [out] 用于缓存flash读到的数据长度，由接口外部分配内存
     * @param filename [in] 要读取的文件路径，若为NULL，则读取type的配置文件
     * @return NK_FALSE 失败
     * @return NK_TRUE：成功
    */
    NK_Int (*getDataFromFlash)(NKFSConfigType_E type, NK_UInt8 *pu8Buf, NK_UInt32 *pu32Len, NK_Char *filename);

    /**
     * @brief 保存数据到flash,底层根据平台决定是存到某个文件或者flash固定分区
     * @param type [in] 配置文件类型
     * @param pu8Buf [in] 要写入flash的数据
     * @param pu32Len [in] 要写入flash的数据长度
     * @param filename [in] 要写入的文件路径，若为NULL，则写入type的配置文件
     * @return NK_FALSE 失败
     * @return NK_TRUE：成功
    */
    NK_Int (*setData2Flash)(NKFSConfigType_E type, NK_UInt8 *pu8Buf, NK_UInt32 u32Len, NK_Char *filename);
    /**
     * @brief 获取文件大小
     * @param path [in] 目标文件
     * @return 失败 -1
     * @return 成功 文件大小
     */
    NK_Int (*getFileSize)(NK_Char *path);
    /**
     * @brief 创建文件夹
     * @param pcPath [in] 创建的目标文件夹
     * @param  mode[in] 该参数为无效参数。此处为靠近 C 标准函数库而设计，输入 0 或正数即可。
     * @return NK_FALSE 失败
     * @return NK_TRUE：成功
     */
    NK_Int32 (*mkdir)(const NK_Char *pcPath, NK_Int32 mode);
    /**
     * @brief 删除文件夹
     * @param pcPath [in] 目标文件夹
     * @return NK_FALSE 失败
     * @return NK_TRUE：成功
     */
    NK_Int32 (*rmdir)(const NK_Char *pcPath);

    /**
     * @brief 调用write接口后,为保证数据从缓冲区真正写到flash,需调用一次fsync保证数据写入flash.
     *        对于无fd的情况,对接时可改为用sync替换
     * @param s32Fd[in] 需要同步的文件描述符
     * @return NK_FALSE 失败
     * @return NK_TRUE：成功
     */
    NK_Int32 (*fsync)(NK_Int32 s32Fd);
    /**
     * @brief 根据配置文件类型获取文件大小
     * @param type [in] 配置文件类型
     * @return 失败 -1
     * @return 成功 文件大小
     */
    NK_Int (*getSizeFromFlash)(NKFSConfigType_E type);
};


NK_API struct APIS_FS_S *APIS_Fs(void);

#if defined(__cplusplus)
}
#endif
#endif