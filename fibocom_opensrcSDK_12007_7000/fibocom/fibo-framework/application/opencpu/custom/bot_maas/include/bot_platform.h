/**
 * @file bot_platform.h
 * @brief Platform-specific macro definitions
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */

#ifndef __BOT_PLATFORM_H
#define __BOT_PLATFORM_H

#include "fibo_opencpu.h"

typedef enum
{
    APP_PDP_AUTHPROTO_NONE = 0,
    APP_PDP_AUTHPROTO_PAP,
    APP_PDP_AUTHPROTO_CHAP
}app_pdp_authproto_t;

typedef enum
{
    APP_PDP_IPTYPE_IPV4 = 1,
    APP_PDP_IPTYPE_IPV6,
    APP_PDP_IPTYPE_IPV4V6
}app_pdp_iptype_t;


#define BOT_NET_ONLY_QUERY          1        ///< only query network status, do not pdp activation
#define TEST_TASK_NOT_SUPPORT_DELETE 1       ///< not support delete task (task exits by return directly)
#define TEST_QUEUE_ITEM_SIZE_MAX    (1024)
#define TEST_QUEUE_LEN_NORNAL       (36)    ///< LE370 plaform max queue len is 36

#define BOT_FS_STREAM_ENABLED       0       ///< support fs stream operation, like linux, windows.etc
#define BOT_FS_USER_DIR "/bot"              ///< bot storage folder

#define BOT_O_RDONLY        O_RDONLY        ///< open file as read-only
#define BOT_O_WRONLY        O_WRONLY        ///< open the file for writing only
#define BOT_O_RDWR          O_RDWR          ///< Open the file for reading and writing. The above three flags are mutually exclusive, that is, they cannot be used at the same time, but they can be combined with the following flags using the OR (|) operator.
#define BOT_O_APPEND        O_APPEND        ///< When reading and writing a file, it will move from the end of the file, that is, the written data will be added to the back of the file in an additional way.
#define BOT_O_CREAT         O_CREAT         ///< If the file to be opened does not exist, it will be created automatically.
#define BOT_O_TRUNC         O_TRUNC         ///< If the file exists and is opened in a writable mode, this flag will clear the file length to 0, and the data originally stored in the file will also disappear.

/* micro defination about file system, used for bot_fseek()'s/bot_seek()'s whence */
#define BOT_SEEK_SET        SEEK_SET        ///< beginning of file
#define BOT_SEEK_CUR        SEEK_CUR        ///< current position
#define BOT_SEEK_END        SEEK_END        ///< end of file

/* bot error code defination */
#define BOT_E_EINTR         EINTR           ///< 4, Interrupted system call
#define BOT_E_EAGAIN        EAGAIN          ///< 11, Try again
#define BOT_E_EPIPE         EPIPE           ///< 32, Broken pipe
#define BOT_E_EWOULDBLOCK   EWOULDBLOCK     ///< like, EAGAIN, Operation would block
#define BOT_E_ECONNRESET    ECONNRESET      ///< 104, FConnection reset by peer

/* socket macros */
#define BOT_AF_UNSPEC       AF_UNSPEC         ///< AF_UNSPEC
#define BOT_SOCK_STREAM     SOCK_STREAM       ///< Stream sockets
#define BOT_SOCK_DGRAM      SOCK_DGRAM        ///< Datagram sockets
#define BOT_IPPROTO_TCP     IPPROTO_TCP       ///< TCP protocol
#define BOT_IPPROTO_UDP     IPPROTO_UDP       ///< UDP protocol

#endif /* __BOT_PLATFORM_H */
