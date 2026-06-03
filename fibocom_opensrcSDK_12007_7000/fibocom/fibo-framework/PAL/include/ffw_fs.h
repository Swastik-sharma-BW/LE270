#ifndef LIB_INCLUDE_FFW_FS
#define LIB_INCLUDE_FFW_FS

/**
* @file ffw_fs.h
* @author sundaqing (sundaqing@fibocom.com)
* @brief 文件系统相关接口
* @version 0.1
* @date 2021-05-21
* 
* Copyright (c) 2021 Fibocom. All rights reserved
* 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ffw_plat_header.h>

typedef struct
{
    unsigned long bsize;   /// 块大小
    unsigned long frsize;  /// 碎片大小
    unsigned long blocks;  /// 块的总个数
    unsigned long bfree;   /// free blocks in fs
    unsigned long long bavail;  /// 可用的块的个数 use INT64
    unsigned long files;   /// total file nodes in file system
    unsigned long ffree;   /// free file nodes in fs
    unsigned long favail;  /// avail file nodes in fs
    unsigned long fsid;    /// file system id
    unsigned long flag;    /// mount flags
    unsigned long namemax; /// maximum length of filenames
    unsigned long size;    /// size file size
    unsigned long type;    /// type file type
} ffw_fs_stat_t;

typedef enum
{
    FLASH_PARTITION_C = 0,      /* Internal Flash */
    FLASH_PARTITION_D = 1,      /* External Flash */
} FlashPartitionT;

#ifndef O_RDONLY
#define O_RDONLY 0x00000001
#endif

#ifndef O_WRONLY
#define O_WRONLY 0x00000002
#endif

#ifndef O_RDWR
#define O_RDWR 0x00000004
#endif

#ifndef O_CREAT
#define O_CREAT 0x00000008
#endif

#ifndef O_TRUNC
#define O_TRUNC 0x00000010
#endif

#ifndef O_APPEND
#define O_APPEND 0x00000020
#endif

#ifndef O_EXCL
#define O_EXCL 0x00000040
#endif

/**
* @brief 打开文件
* 
* @param path 
* @param flags 
* @param mode 
* @return int32_t 
* @retval > 0: 成功
* @retval FFW_R_FAILED: 失败
 */
ffw_fs_handle_t ffw_fs_open(const char *path, int flags, int mode);

/**
* @brief 关闭文件
* 
* @param fd 文件描述符
* @return int32_t 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败
 */
int32_t ffw_fs_close(ffw_fs_handle_t fd);

/**
* @brief 读取文件
* 
* @param fd 
* @param buf 
* @param count 
* @return int32_t 
 */
int32_t ffw_fs_read(ffw_fs_handle_t fd, void *buf, int32_t count);

/**
* @brief 写文件
* 
* @param fd 
* @param buf 
* @param count 
* @return int32_t 
 */
int32_t ffw_fs_write(ffw_fs_handle_t fd, const void *buf, int32_t count);

/**
* @brief 获取文件大小
* 
* @param path 文件路径 
* @param[out] size 文件大小 
* @return int32_t 
 */
int32_t ffw_fs_size(const char *path, uint32_t *size);


/**
* @brief 根据打开文件句柄获取文件大小
* 
* @param file_size 
* @return int32_t 
 */
int32_t ffw_fs_fsize(ffw_fs_handle_t fd, uint32_t *file_size);
int32_t ffw_fs_ftell(ffw_fs_handle_t fd, uint32_t *size);

/**
 * @brief 判断路径是不是文件
 * 
 * @param path 
 * @return true 
 * @return false 
 */
bool ffw_fs_path_is_file(const char *path);

/**
 * @brief 判断文件是否是目录
 * 
 * @param path 
 * @return true 
 * @return false 
 */
bool ffw_fs_path_is_dir(const char *path);

/**
* @brief 设置文件长度
* 
* @param fd 
* @param length 
* @return int32_t 
 */
int32_t ffw_fs_ftruncate(ffw_fs_handle_t fd, int32_t length);

/**
* @brief 设置文件长度
* 
* @param path 
* @param length 
* @return int32_t 
 */
int32_t ffw_fs_truncate(const char *path, int32_t length);

/**
* @brief 删除文件
* 
* @param pathname 
* @return int32_t 
 */
int32_t ffw_fs_unlink(const char *pathname);

/**
* @brief 同步文件数据
* 
* @param fd file descriptor
* @return int32_t 
 */
int32_t ffw_fs_fsync(ffw_fs_handle_t fd);

/**
* @brief 获取当前文件的状态
* 
* @param path
* @param st
* @return int32_t
 */
int32_t ffw_fs_stat(const char *path, ffw_fs_stat_t *st);

/**
* @brief reposition read/write file offset
* 
* @param fd file descriptor
* @param offset offset according to the directive whence
* @param whence SEEK_SET, SEEK_CUR, SEEK_END
* @return int32_t 
*      - offset location as measured in bytes from the beginning of
*        the file on success.
*      - -1 on error. Refer to file system implementation for errno.
*/
int32_t ffw_fs_seek(ffw_fs_handle_t fd, int32_t offset, int whence);

int32_t ffw_fs_rewind(ffw_fs_handle_t fd);


/**
* @brief 创建目录
* 
* @param name 目录名称
* @param mode 目录权限
* @return int32_t 
 */
int32_t ffw_fs_mkdir(const char *name, int mode);

/**
* @brief 删除目录
* 
* @param name 目录名称
* @return int32_t 
 */
int32_t ffw_fs_rmdir(const char *name);

/**
* @brief 打开目录
* 
* @param name 
* @return void* 
 */
void *ffw_fs_opendir(const char *name);

/**
* @brief 读取目录
* 
* @param dirp 
* @return void* 
 */
void *ffw_fs_readdir(void *dirp);

int32_t ffw_fs_telldir(void *dirp);

int32_t ffw_fs_seekdir(void *dirp, long loc);

/**
* @brief 关闭目录
* 
* @param dirp 
* @return int32_t 
 */
int32_t ffw_fs_closedir(void *dirp);

/**
* @brief 判断当前文件是否是文件
* 
* @param dirent_entry @c ffw_fs_readdir 的返回值
* @return true 是文件
* @return false 是目录
 */
bool ffw_fs_direntry_file(void *dirent_entry);

/**
* @brief 获取当前文件的名称
* 
* @param dirent_entry @c ffw_fs_readdir 的返回值
* @return const char* 文件名称
 */
const char *ffw_fs_direntry_name(void *dirent_entry);

/**
 * @brief 
 * 
 * @param oldpath 
 * @param newpath 
 * @return int32_t 
 */
int32_t ffw_fs_rename(const char *oldpath, const char *newpath);

/**
* @brief 获取文件系统的信息
* 
* @param path 
* @param fsstat 
* @return int32_t 
 */
int32_t ffw_fs_sysstat(const char *path, ffw_fs_stat_t *fsstat);

/**
* @brief 
* 
* @return int32_t 
 */
int32_t ffw_fs_freesize(void);

/**
* @brief 
* 
* @return int32_t 
 */
int32_t ffw_fs_totalsize(void);

/**
* @brief 定制分区大小
* 
* @return int32_t 
 */
int32_t ffw_fs_freesize_cus(void);

/**
* @brief 
* 
* @return int32_t 
 */
int32_t ffw_fs_totalsize_cus(void);


/**
* @brief 获取当前目录下所有子项目信息或文件信息
* @param dir 目录/文件名
* @param cb 回调函数
* @param args 参数
* @return int32_t 
 */
int32_t ffw_get_file_list(char *dir, void (*cb)(const char *filepath, size_t size, void *arg), void *args);


#endif /* LIB_INCLUDE_FFW_FS */
