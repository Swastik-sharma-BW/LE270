/**
 * @file bot_fs.h
 * @brief bot fs component header file
 * @details The interface in this file is used for system parameter storage,
 * and at least one of bot_fs.h and bot_hal_flash.h must be adapted
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */
#ifndef __BOT_FS_H__
#define __BOT_FS_H__

#if defined(__cplusplus)
extern "C" {
#endif

/** @addtogroup fs
 *  fs hal API.
 *
 *  @{
 */

#include "bot_typedef.h"
#include "bot_platform.h"
#include "bot_platform_user_config.h"

typedef int bot_fd_t;
typedef int bot_fs_flag_t;
typedef unsigned int bot_mode_t;
typedef bot_fd_t bot_file_t;
#define BOT_FILE_INIT_VALUE  -1
#define BOT_MKDIR_MODE_NOT_USE  0xffffffff

#define BOT_PATH_LEN_MAX 256

/**
 * @brief The base path of parameter storage
 *
 * @param[in] void
 *
 * @return offset path
 */
const char *bot_basepath_get(void);

/**
 * @brief bot_fs_init() initializes vfs system.
 *
 * @param[in] void
 *
 * @return fs init result
 * @retval 0 success
 * @retval others fail
 */
int bot_fs_init(void);

/**
 * @brief bot_exitdir() check if dir exists or not.
 *
 * @param[in] path  The name of directory to be queried.
 *
 * @return dir exists or not
 * @retval 0 exists
 * @retval others not exists
 */
int bot_exitdir(const char *path);

/**
 * @brief bot_mkdir() attempts to create a directory named @path
 *
 * @param[in] path  The name of directory to be created.
 * @param[in] mode  reserved.
 *
 * @return folder creation result
 * @retval 0 success
 * @retval others fail
 */
int bot_mkdir(const char *path, bot_mode_t mode);

/**
 * @brief bot_rmdir() attempts to delete a directory named @path
 *
 * @param[in] path  The name of directory to be deleted.
 *
 * @return folder remove result
 * @retval 0 success
 * @retval others fail
 */
int bot_rmdir(const char *path);

/**
 * @brief bot_open() opens the file or device by its @path.
 *
 * @param[in] path   the path of the file or device to open.
 * @param[in] flags  the flags of open operation.
 * BOT_O_RDONLY      open the file as read-only
 * BOT_O_WRONLY      open the file for writing only
 * BOT_O_RDWR        open the file in a read-write mode. The above three flags are mutually exclusive, that is,
 *                   they cannot be used at the same time, but they can be combined with the following flags using the OR (|) operator.
 * BOT_O_APPEND      will move from the end of the file when reading and writing files, that is,
 *                   the written data will be added to the back of the file in an additional way.
 * BOT_O_CREAT       if the file to be opened does not exist, it will be created automatically.
 * BOT_O_TRUNC       if the file exists and is opened in a writable mode, this flag will clear the file length to 0,
 *                   and the data originally stored in the file will also disappear.
 * BOT_O_EXCL        if O_CREAT is also set, this command will check whether the file exists.
 *                   If the file does not exist, the file will be created, otherwise it will cause an error to open the file.
 *                   In addition, if O_CREAT and O_EXCL are set at the same time, and the file to be opened is a symbolic link ,
 *                   it will fail to open the file.
 * BOT_O_SYNC        open the file synchronously.
 * BOT_O_NOCTTY      if the file to be opened is a terminal device, the terminal will not be regarded as a process control terminal.
 * BOT_O_NONBLOCK    open the file in an unblockable way,
 *                   that is, it will return to the process immediately regardless of whether there is data to read or wait.
 * BOT_O_NDELAY      same as O_NONBLOCK.
 * BOT_O_NOFOLLOW    if the file pointed by the parameter path is a symbolic link, it will fail to open the file.
 * BOT_O_DIRECTORY   if the file pointed to by the parameter path is not a directory, it will fail to open the file.
 *                   Note: This is a special flag after Linux2. 2 to avoid some system security problems.
 *
 * @return file descriptor
 * @retval ≥0 success
 * @retval <0 fail
 */
int bot_open(const char *path, int flags);

/**
 * @brief bot_close() closes the file or device associated with file
 *        pointer @fd.
 *
 * @param[in] fd  the file handler of the file or device.
 *
 * @return file close result
 * @retval 0 success
 * @retval others fail
 */
int bot_close(int fd);

/**
 * @brief bot_read() attempts to read up to (@size * @count) bytes from file
 *        pointer @fd into the buffer starting at @buff.
 *
 * @param[in]  fd      the file handler of the file or device.
 * @param[out] buf    the buffer to read bytes into.
 * @param[in]  count   the number of bytes to read.
 *
 * @return  the number of bytes is returned (0 indicates end
 *          of file) and the file position is advanced by this number
 * @retval ≥0 success
 * @retval <0 fail
 */
int bot_read(int fd, void *buf, size_t count);

/**
 * @brief bot_write() writes up to @nbytes bytes from the buffer starting
 *        at @buff to the file referred to by the file handler @fd.
 *
 * @param[in] fd      the file handler of the file or device.
 * @param[in] buf     the buffer to write bytes from.
 * @param[in] count   the number of bytes to write.
 *
 * @return  the number of bytes written is returned.
 * @retval ≥0 success
 * @retval <0 fail
 */
int bot_write(int fd, const void *buf, size_t count);

/**
 * @brief bot_lseek() repositions the file offset of the open file
 *        description associated with the file handler @fd to the
 *        argument @offset according to the directive @whence as follows:
 *
 *        BOT_SEEK_SET: The file offset is set to @offset bytes.
 *        BOT_SEEK_CUR: The file offset is set to its current location
 *                  plus @offset bytes.
 *        BOT_SEEK_END: The file offset is set to the size of the file
 *                  plus @offset bytes.
 *
 * @param[in] fd      the file handler of the file.
 * @param[in] offset  The offset relative to @whence directive.
 * @param[in] whence  The start position where to seek.
 *
 * @return  returns  the  resulting  offset  location  as measured in bytes from the beginning of the file
 * @retval ≥0 success
 * @retval <0 fail
 */
int bot_lseek(int fd, int offset, int whence);

/**
 * @brief bot_unlink() deletes a name @path from the filesystem.
 *
 * @param[in]  path       The path of the file to be quried.
 *
 * @return file deletion result
 * @retval 0 success
 * @retval <0 fail
 */
int bot_unlink(const char *path);

#if defined(__cplusplus)
}
#endif
#endif /* __BOT_FS_H__ */