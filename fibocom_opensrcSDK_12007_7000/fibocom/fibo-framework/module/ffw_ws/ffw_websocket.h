#ifndef LIB_INCLUDE_FFW_WEBSOCKET
#define LIB_INCLUDE_FFW_WEBSOCKET

/**
* @file ffw_websocket.h
* @author sundaqing (sundaqing@fibocom.com)
* @brief websocket协议客户端
* @version 0.1
* @date 2021-05-26
* 
* Copyright (c) 2021 Fibocom. All rights reserved
* 
 */

#include <stdint.h>

#include <ffw_transport_interface.h>
#include <ffw_os.h>
#include <ffw_url.h>
#include <ffw_buffer.h>

/// type of ffw_ws_header_s
typedef struct ffw_ws_header_s ffw_ws_header_t;

/// type of ffw_ws_conn_s
typedef struct ffw_ws_conn_s ffw_ws_conn_t;

/// type of websocket_packet_info_s
typedef struct websocket_packet_info_s websocket_packet_info_t;

/**
* @brief websocket 数据回调函数定义
* @param t 客户端实例
* @param data 数据
* @param len 数据大小
* @param arg   回调上下文
 */
typedef void (*ffw_ws_data_t)(ffw_ws_conn_t *t, void *data, uint32_t len, void *arg);

/**
* @brief websocket 断开回调函数定义
* @param t 客户端实例
* @param reason 断开原因
* @param arg   回调上下文
 */
typedef void (*ffw_ws_close_t)(ffw_ws_conn_t *t, int reason, void *arg);

/**
* @brief websocket 异步连接回调函数定义
* @param t 客户端实例
* @param result 1 连接成功 0 连接失败
* @param arg   回调上下文
 */
typedef void (*ffw_ws_connect_t)(ffw_ws_conn_t *t, int result, void *arg);

/**
* @brief websocket协议code定义
* 
 */
typedef enum ffw_ws_opcode_s
{
    /// 非最后一块数据
    FFW_WS_CONTINUATION = 0x0,

    /// 文本数据
    FFW_WS_TEXT_FRAME = 0x1,

    /// 二进制数据
    FFW_WS_BINARY_FRAME = 0x2,

    /// 关闭包
    FFW_WS_CLOSE = 8,

    /// PING 包
    FFW_WS_PING = 9,

    /// PING回应包
    FFW_WS_PONG = 0xa,
} ffw_ws_opcode_t;

/**
* @brief 解析消息报状态机状态定义
* 
 */
typedef enum ffw_ws_stat_s
{
    /// 解析消息头
    FFW_WS_HEADER = 0x0,

    /// 解析消息长度字节数部分
    FFW_WS_LENGTH_SIZE,

    /// 解析消息长度部分
    FFW_WS_LENGTH,

    /// 解析掩码部分
    FFW_WS_MASK,

    /// 解析净荷数据部分
    FFW_WS_DATA,

    /// 由于数据太大，忽略当前数据
    FFW_WS_DATA_IGNORE,

    /// 握手状态
    FFW_WS_HANDSHAKE
} ffw_ws_stat_t;

/**
* @brief 客户端状态定义
* 
 */
typedef enum ffw_ws_status_s
{
    /// 关闭状态
    FFW_WS_STATUS_CLOSE = 0,

    /// 正在连接状态
    FFW_WS_STATUS_CONNECTTING,

    /// 握手状态
    FFW_WS_STATUS_HANDSHAKE,

    /// 连接成功
    FFW_WS_STATUS_CONNECTTED,
} ffw_ws_status_t;

/**
* @brief 消息头定义
* 
 */
struct ffw_ws_header_s
{
    /// 消息头大小
    uint32_t header_size;

    /// 是否是最后一个包
    uint8_t fin;

    /// 掩码
    uint8_t mask;

    /// 操作码
    ffw_ws_opcode_t opcode;

    /// 长度部分
    uint32_t N0;

    /// 长度部分
    uint32_t N;

    /// 掩码钥匙
    uint8_t masking_key[4];
};

/**
* @brief 解析消息状态记录
* 
 */
struct websocket_packet_info_s
{
    /// header of packet
    ffw_ws_header_t header;

    ///净荷数据大小
    uint32_t data_size;

    ///表示已收到表示字节长度的字节数
    uint32_t len_byte;

    ///表示数据长度的字节数
    uint32_t len_size;

    ///接收数据的缓冲区
    ffw_buffer_t data;

    ///已收到的mask的字节数，总共为四个字节
    int32_t mask_byte;

    /// 当前状态
    ffw_ws_stat_t st;
};

/**
* @brief websocket客户端实例定义
* 
 */
struct ffw_ws_conn_s
{
    /// 是否初始化
    bool init;

    /// 信号量
    void *sem;

    /// 互斥锁
    void *lock;

    /// 掩码钥匙
    uint8_t masking_key[4];

    /// 客户端状态
    ffw_ws_status_t status;

    /// 连接回调函数
    ffw_ws_connect_t conn_cb;

    /// 回调上下文
    void *conn_arg;

    /// 数据回调函数
    ffw_ws_data_t data_cb;

    /// 连接断开回调
    ffw_ws_close_t close_cb;

    /// 回调上下文
    void *arg;

    /// 网络连接句柄
    ffw_transport_t transport_handler;

    /// 发送PING的时间间隔
    uint32_t ping_interval;

    /// PING回应的超时时间，单位为秒
    uint32_t ping_timout;

    /// 上一次发送时间戳
    int64_t last_send;

    /// 上一次接收时间戳
    int64_t last_recv;

    /// 是否需要忽略当前包
    bool ignore_pkt;

    /// 解析包的状态
    websocket_packet_info_t pkt_info;

    /// 握手超时时间
    int32_t handshake_timeout;

    /// TCP连接超时时间
    uint32_t connect_timeout;

    /// 是否需要握手
    bool should_handshake;

    /// 握手需要的信息
    char *origin;

    /// 握手需要的信息
    char *key;

    /// 解析后的URL
    ffw_url_t url;

    /// 使用的网卡
    ffw_handle_t netif;
};

/**
* @brief 初始化一个websocket客户端
* 
* @param wsc 客户端实例
* @param ping_interval PING发送间隔，单位为秒
* @param ping_timout PING回应的超时时间，单位为秒
* @param data_cb 数据回到函数
* @param close_cb 断开回调函数
* @param arg 回调上线文
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_ws_init(ffw_ws_conn_t *wsc, uint32_t ping_interval, uint32_t ping_timout, ffw_ws_data_t data_cb, ffw_ws_close_t close_cb, void *arg);

/**
* @brief 同步连接websocket服务器
* 
* @param wsc 客户端实例
* @param url url
* @param should_handshake 是否需要握手
* @param origin 初始化路径
* @param key 握手需要的KEY
* @param timeout 超时时间，单位毫秒
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_ws_connect(ffw_ws_conn_t *wsc, const char *url, bool should_handshake, const char *origin, const char *key, int32_t timeout);

/**
* @brief 异步连接websocket服务器
* 
* @param wsc 客户端实例
* @param url url
* @param should_handshake  是否需要握手
* @param origin 初始化路径
* @param key 握手需要的KEY
* @param timeout 超时时间，单位毫秒
* @param conn_cb 连接回调函数
* @param arg 回调上线文
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_ws_connect_async(ffw_ws_conn_t *wsc, const char *url, bool should_handshake, const char *origin, const char *key, int32_t timeout, ffw_ws_connect_t conn_cb, void *arg);

/**
* @brief 发送二进制数据
* 
* @param wsc  客户端实例
* @param data 数据
* @param size 数据大小
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_ws_send_binary(ffw_ws_conn_t *wsc, const void *data, uint32_t size);

/**
* @brief 发送文本数据
* 
* @param wsc 客户端实例
* @param text 数据
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_ws_send_text(ffw_ws_conn_t *wsc, const char *text);

/**
 * @brief 发送数据头部，然后使用 @c ffw_ws_send_data 发送净荷数据
 * 
 * @param wsc 客户端实例
 * @param type 类型
 * @param size 数据大小
 * @return int 
 * @retval FFW_R_SUCCESS 成功
*  @retval FFW_R_FAILED 失败
 */
int ffw_ws_start_send(ffw_ws_conn_t *wsc, ffw_ws_opcode_t type, uint32_t size);

/**
 * @brief 发送数据，可以调用多次。必须先调用 @c ffw_ws_start_send
 * 
 * @param wsc 客户端实例
 * @param data 数据
 * @param size 数据大小
 * @return int 
 * @retval FFW_R_SUCCESS 成功
 *  @retval FFW_R_FAILED 失败
 */
int ffw_ws_send_data(ffw_ws_conn_t *wsc, const void *data, uint32_t size);

/**
* @brief 发送PING
* 
* @param wsc 客户端实例
* @return int 
* @retval FFW_R_SUCCESS 成功
*  @retval FFW_R_FAILED 失败
 */
int ffw_ws_ping(ffw_ws_conn_t *wsc);

/**
* @brief 发送PONG
* 
* @param wsc 客户端实例
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_ws_pong(ffw_ws_conn_t *wsc);

/**
* @brief 关闭客户端
* 
* @param wsc 客户端实例
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_ws_close(ffw_ws_conn_t *wsc);

/**
* @brief 销毁实例
* 
* @param wsc 实例
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_ws_deinit(ffw_ws_conn_t *wsc);

#endif /* LIB_INCLUDE_FFW_WEBSOCKET */
