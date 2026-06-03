
#ifndef APIS_SOCKET_H_
#define APIS_SOCKET_H_
#include "APIS_Macro.h"
NK_CPP_EXTERN_BEGIN

typedef enum SOCKET_PROTOCOL_S_TAG {
    SOCKET_TCP,
    SOCKET_UDP,
    SOCKET_ICMP,
}SOCKET_PROTOCOL_E;

typedef enum SOCKET_ERR_TAG{
    SOCKET_ERR_TCP_UNKNOW               = 0,      // unknow err
    SOCKET_ERR_TCP_SOCKET_NOT_CREAT     = -600,  // TCP socket fd not created
    SOCKET_ERR_TCP_SOCKET_FAILED        = -601,  // TLS TCP socket connect fail
    SOCKET_ERR_TCP_UNKNOWN_HOST         = -602,  // TCP unknown host (DNS fail)
    SOCKET_ERR_TCP_CONNECT              = -603,  // TCP/UDP socket connect fail
    SOCKET_ERR_TCP_READ_TIMEOUT         = -604,  // TCP read timeout
    SOCKET_ERR_TCP_WRITE_TIMEOUT        = -605,  // TCP write timeout
    SOCKET_ERR_TCP_READ_FAIL            = -606,  // TCP read error
    SOCKET_ERR_TCP_WRITE_FAIL           = -607,  // TCP write error
    SOCKET_ERR_TCP_SEND_PEER_SHUTDOWN   = -608,  // TCP 发送数据时,服务端连接已关闭, 执行失败
    SOCKET_ERR_TCP_RECV_PEER_SHUTDOWN   = -609,  // TCP 接收数据时,服务端连接已关闭, 执行失败
    SOCKET_ERR_TCP_NOTHING_TO_READ      = -610,  // TCP socket nothing to read
    SOCKET_ERR_TCP_PARAM_ERR            = -611,  // TCP 参数异常,如空指针...
}SOCKET_ERR_E;

//ip、icmp包头,禁止编译器对齐
#pragma pack(push)
#pragma pack(1)

typedef struct ip_hdr_s_{
    /**
     * @brief version(4bits) + header length(4bits)
     * 版本字段：占4bit，表明IP协议实现的版本号，即IPV4 or IPV6
     * 报头长度：占4bit。是头部占32比特的数字，包括可选项。普通IP数据报(没有任何选项)，该字段的值是5，即160比特=20字节。此字段最大值为60字节
     */
    NK_UInt8  v_hl;

    /**
     * @brief 服务类型字段：占8bit.
     * 0~2 bits: 优先权子字段(Precedence，现已被忽略)
     * 3~6 bits: 分别代表延迟、吞吐量、可靠性和花费.当它们取值为1时分别代表要求最小时延、最大吞吐量、最高可靠性和最小费用,这4比特的服务类型中只能置其中1比特为1。
     *           可以全为0，若全为0则表示一般服务。服务类型字段声明了数据报被网络系统传输时可以被怎样处理
     * 7 bit:    保留未用
     */
    NK_UInt8  tos;

    /**
     * @brief 指明整个数据报的长度(以字节为单位)。最大长度为65535字节。IP头部+UDP头部+数据区长度
     */
    NK_UInt16 len;

    /* identification */
    /**
     * @brief 用来唯一地标识主机发送的每一份数据报。通常每发一份报文，它的值会加1
     * 
     */
    NK_UInt16 id;

    /**
     * @brief 标志位(3bits) + 段偏移(13bits)
     * 标志位字段：占3bit。标志一份数据报是否要求分段
     * 段偏移字段：占13bit。如果一份数据报要求分段的话，此字段指明该段偏移距原始数据报开始的位置
     */
    NK_UInt16 offset;

#define IP_RF       0x8000U         /* 保留的标志位 */
#define IP_DF       0x4000U         /* 不分片标志位 */
#define IP_MF       0x2000U         /* 更多分片标志 */
#define IP_OFFMASK  0x1fffU         /* 用于分段的掩码 */

    /**
     * @brief 用来设置数据报最多可以经过的路由器数。由发送数据的源主机设置，通常为32、64、128等。
     *        每经过一个路由器，其值减1，直到0时该数据报被丢弃
     */
    NK_UInt8  ip_ttl;

    /**
     * @brief 指明IP层所封装的上层协议类型，如ICMP(1)、IGMP(2) 、TCP(6)、UDP(17)等
     */
    NK_UInt8  proto;

    /**
     * @brief 头部校验和字段：占16bit。二进制反码求和，根据IP头部计算得到的校验和。
     *        (和ICMP、IGMP、TCP、UDP不同，IP不对头部后的数据进行校验)
     */
    NK_UInt16 cksum;

    NK_UInt32 ip_src;

    NK_UInt32 ip_dst;
}IP_HDR_S;

typedef struct icmp_hdr_s_{
    NK_UInt8  icmp_type;
    NK_UInt8  icmp_code;
    NK_UInt16 icmp_cksum;
    NK_UInt16 icmp_id;
    NK_UInt16 icmp_seq;
}ICMP_HDR_S;

#pragma pack(pop)


#define NK_ICMP_ECHOREPLY          (0)
#define NK_ICMP_DEST_UNREACH       (3)
#define NK_ICMP_SOURCE_QUENCH      (4)
#define NK_ICMP_REDIRECT           (5)
#define NK_ICMP_ECHO               (8)
#define NK_ICMP_TIME_EXCEEDED      (11)
#define NK_ICMP_PARAMETERPROB      (12)
#define NK_ICMP_TIMESTAMP          (13)
#define NK_ICMP_TIMESTAMPREPLY     (14)
#define NK_ICMP_INFO_REQUEST       (15)
#define NK_ICMP_INFO_REPLY         (16)
#define NK_ICMP_ADDRESS            (17)
#define NK_ICMP_ADDRESSREPLY       (18)
#define NK_NR_ICMP_TYPES           (18)

/**
 * @brief
 *  Ping 操作返回信息。
 */
typedef struct Nk_PingOnceResponseMsg {

    NK_UInt8 ttl;

    /// 发送数据包长度。
    NK_Size sendPacketLen;

    /// 接收数据包长度。
    NK_Size recvPacketLen;

    /// 发送数据包地址。
    NK_Char srcIP[16];

    /// 发送数据包对端地址。
    NK_Char dstIP[16];

} NK_PingOnceResponseMsg;

/**
 * @description: 根据指定协议创建socket句柄
 * @param protocol 指定是UDP或TCP
 * @return 成功返回socket句柄，需确认创建成功是否有=0的情况，会影响到p2p destroy的逻辑
 * @return 失败返回NK_FALSE
 */
NK_Int APIS_Socket_Creat(SOCKET_PROTOCOL_E protocol);

/**
 * @description: 连接远端目标，需阻塞连接，直到成功或失败。
 * @param sock JALTE_CreateSocket返回的有效值
 * @param ip 远端IP,也可能是域名,接口内部实现时，要先解析一次
 * @param port 远端port
 * @param timeout_ms 连接超时时长，毫秒
 * @return NK_TRUE：成功
 * @return NK_FALSE:失败
 */
NK_Int APIS_Socket_Connect(NK_Int sock, NK_PChar ip, NK_Int port, NK_Int timeout_ms);

/**
 * @description: 断开远端连接
 * @param sock 有效的socket句柄
 * @return 0：成功
 * @return -1:失败
 */
NK_Int APIS_Socket_Close(NK_Int sock);

/**
 * @description: 通过socket发送数据，需阻塞发送，直到全部发送成功。超时后中断发送，并返回错误
 * @param sock 有效的socket句柄
 * @param buf 待发送的数据
 * @param len 待发送的数据长度
 * @param timeout_ms 发送超时
 * @return 成功返回已发送的字节数
 * @return 失败返回<=0, @ref SOCKET_ERR_E
 */
 NK_Int APIS_Socket_Send(NK_Int sock, NK_PChar buf, NK_Int len, NK_Int timeout_ms);

/**
 * @description: 通过socket接收数据，需阻塞接收，直接接收到指令长度的数据。超时后中断接并返回错误
 * @param sock 有效的socket句柄
 * @param buf 存放接收数据的缓冲区
 * @param len 缓冲区长度
 * @param timeout_ms 接收超时
 * @return 成功返回已接收的字节数。
 * @return 失败返回<=0, @ref SOCKET_ERR_E
 */
NK_Int APIS_Socket_Recv(NK_Int sock, NK_PChar buf, NK_Int len, NK_Int timeout_ms);

/**
 * @description: 获取最新的socket错误码
 * @param sock 有效的socket句柄
 * @return 成功返回socket错误码
 */
NK_Int APIS_Socket_GetErrno(NK_Int sock);

/**
 * @description: 根据域名获取ip
 * @param host_name [in] 域名
 * @param ip [out] ip
 * @return NK_TRUE：成功
 * @return NK_FALSE:失败
*/
NK_Int APIS_Socket_Getaddrinfo(const char* host_name,NK_PChar ip );

/**
 * @brief 强制退出dns解析接口
 * @return NK_TRUE：成功
 * @return NK_FALSE:失败
 */
NK_Int APIS_Socket_GetaddrinfoFoceExit();

/**
 * @brief Return True, if you can read any data from socket or is incoming connection on TCP based socket.
 * Status is tested for time Timeout (in milliseconds).
 * If value in Timeout is 0, status is only tested and continue.
 * If value in Timeout is -1, run is breaked and waiting for read data maybe forever.
 * NK_This function is need only on special cases, when you need use RecvBuffer function directly!
 * read functioms what have timeout as calling parameter, calling NK_This function internally.
 * @param sock [in] 有效的socket句柄
 * @param timeout_sec [in] 超时时长，秒
 * @param pSelectRet [out] 返回select返回值, >0 有事件发生, =0 超时, <0 错误
 * @return NK_TRUE 数据可读
 * @return NK_FALSE 数据不可读
 */
NK_Int APIS_Socket_CanRead(NK_Int sock, NK_Int timeout_sec, NK_Int *pSelectRet);

/**
 * @brief: Return True, if you can to socket write any data (not full sending buffer).
 * Status is tested for time Timeout (in milliseconds).
 * If value in Timeout is 0, status is only tested and continue.
 * If value in Timeout is -1, run is breaked and waiting for write data maybe forever.
 * @param sock [in] 有效的socket句柄
 * @param timeout_sec [in] 超时时长，秒
 * @param pSelectRet [out] 返回select返回值, >0 有事件发生, =0 超时, <0 错误
 * @return NK_TRUE 数据可写
 * @return NK_FALSE 数据不可写
*/
NK_Int APIS_Socket_CanWrite(NK_Int sock, NK_Int timeout_sec, NK_Int *pSelectRet);

/**
 * @description: udp 发送数据
 * @param sock 有效的socket句柄
 * @param buf 待发送的数据
 * @param len 待发送的数据长度
 * @param dstIp 目标IP
 * @param dstPort 目标端口
 * @return 成功返回已发送的字节数
 * @return 失败返回-1
 */
NK_Int APIS_Socket_SendTo(NK_Int sock, NK_PChar buf, NK_Int len, NK_PChar dstIp, NK_Int dstPort);

/**
 * @description: udp 接收数据
 * @param sock 有效的socket句柄
 * @param buf 待发送的数据
 * @param len 待发送的数据长度
 * @param dstIp 目标IP
 * @param *pRetPort 返回目标port
 * @return 成功返回已收到的字节数
 * @return 失败返回-1
 */
NK_Int APIS_Socket_RecvFrom(NK_Int sock, NK_PChar buf, NK_Int len, NK_PChar srcIp, NK_Int *pRetPort);

/**
 * @description: socket bind
 * @param sock 有效的socket句柄
 * @param ip 绑定的ip
 * @param port 绑定的port
 * @return NK_TRUE：成功
 * @return NK_FALSE:失败
 */
NK_Int APIS_Socket_Bind(NK_Int sock, NK_PChar ip, NK_Int port);

/**
 * @brief converts the unsigned short integer hostshort from host byte order to network byte order.
 * @param hostshort
 * @return NK_UInt16
 */
NK_UInt16 APIS_Socket_htons(NK_UInt16 hostshort);

/**
 * @brief converts the unsigned integer hostlong from host byte order to network byte order
 * @param hostlong
 * @return NK_UInt32
 */
NK_UInt32 APIS_Socket_htonl(NK_UInt32 hostlong);

/**
 * @brief converts the unsigned short integer netshort from network byte order to host byte order
 * @param netshort
 * @return NK_UInt16
 */
NK_UInt16 APIS_Socket_ntohs(NK_UInt16 netshort);

/**
 * @brief converts the unsigned integer netlong from network byte order to host byte order
 * @param netlong
 * @return NK_UInt32
 */
NK_UInt32 APIS_Socket_ntohl(NK_UInt32 netlong);

/**
 * @brief 将一个点分十进制的IP转换成一个长整型数（u_long类型）
 * @param cp IP address in ascii representation (e.g. "127.0.0.1")
 * @return NK_UInt32 若字符串有效则将字符串转换为32位二进制网络字节序的IPV4地址，否则为INADDR_NONEE(255.255.255.255)
 */
NK_UInt32 APIS_Socket_inet_addr(const NK_Char *cp);

/**
 * @brief Set Time-to-live value. (if system supporting it!)
 * @param sock 有效的socket句柄
 * @param ttl ttl时间, 单位 ms
 * @return NK_Int 成功返回NK_TRUE, 失败返回NK_FALSE
 */
NK_Int APIS_Socket_SetTTL(NK_Int sock, NK_Int ttl);

/**
 * @brief ping包接口
 * @param dest [in] 目标地址,必须是纯ip地址,不能传域名
 * @param ttl [in] ping包生命周期, time to live
 * @param sequence [in] 序列号
 * @param timeout [in] 超时时间,单位ms
 * @param data [in] 待发送的数据
 * @param datalen [in] 待发送的数据长
 * @param Msg [out] 存放ping包应答,含双方ip、对端ttl、发送、接受数据包长
 * @return NK_SSize 失败返回-1, 成功返回ping包耗时,单位ms
 */
NK_SSize APIS_Socket_Ping(NK_PChar dest, NK_UInt8 ttl, NK_Int sequence, NK_UInt32 timeout, NK_PByte data, NK_Size datalen, NK_PingOnceResponseMsg *Msg);


#ifdef USE_SSL
#ifndef MAX_SIZE_OF_CLIENT_ID
#define MAX_SIZE_OF_CLIENT_ID  (80)
#endif
/**
 * @brief Define structure for TLS connection parameters
 *
 */
typedef struct {
    NK_UInt8 profile_idx;
    const NK_Char *ca_crt;
    NK_UInt16 ca_crt_len;

    //是否用对称加密方式
#ifdef AUTH_MODE_CERT
    /**
     * Device with certificate
     */
    const NK_Char *cert_file;  // public certificate file
    const NK_Char *key_file;   // pravite certificate file
#else
    /**
     * Device with PSK
     */
    const NK_Char *psk;         // PSK string
    char psk_id[MAX_SIZE_OF_CLIENT_ID + 1];  // PSK ID
#endif

    NK_Int32 psk_length;        // PSK length
    NK_UInt32 timeout_ms;       // SSL handshake timeout in millisecond

} SSLConnectParams;

typedef SSLConnectParams TLSConnectParams;

/**
 * @brief Setup TLS connection with server
 *
 * @param   pConnectParams reference to TLS connection parameters
 * @host    server address
 * @port    server port
 * @return  TLS connect handle when success, or 0 otherwise
 */
NK_PVoid APIS_Socket_Tls_Connect(TLSConnectParams *pConnectParams, const NK_Char *host, NK_Int port);

/**
 * @brief Disconnect with TLS server and release resources
 *
 * @param handle TLS connect handle
 */
NK_Void APIS_Socket_Tls_Disconnect(NK_PVoid handle);

/**
 * @brief Write data via TLS connection
 *
 * @param handle        TLS connect handle
 * @param data          source data to write
 * @param totalLen      length of data
 * @param timeout_ms    timeout value in millisecond
 * @param written_len   length of data written successfully
 * @return              QCLOUD_RET_SUCCESS for success, or err code for failure
 */
NK_Int APIS_Socket_Tls_Write(NK_PVoid handle, NK_PByte data, NK_Int32 totalLen, NK_UInt32 timeout_ms, NK_Int32 *written_len);

/**
 * @brief Read data via TLS connection
 *
 * @param handle        TLS connect handle
 * @param data          destination data buffer where to put data
 * @param totalLen      length of data
 * @param timeout_ms    timeout value in millisecond
 * @param read_len      length of data read successfully
 * @return              QCLOUD_RET_SUCCESS for success, or err code for failure
 */
NK_Int APIS_Socket_Tls_Read(NK_PVoid handle, NK_PByte data, NK_Int32 totalLen, NK_UInt32 timeout_ms, NK_Int32 *read_len);

NK_Int APIS_Socket_Encrypt_AES128(NK_Char *in, NK_Int in_len, NK_Char *out, NK_Int *out_len, NK_Char *key, NK_Char *iv);

NK_Int APIS_Socket_Decrypt_AES128(NK_Char *in, NK_Int in_len, NK_Char *out, NK_Int *out_len, NK_Char *key, NK_Char *iv);

NK_Int APIS_Socket_Encrypt_SHA256(NK_Char *in, NK_Int in_len, NK_Char *key, NK_Int key_len, NK_Char *out, NK_Int *out_len);

#endif



NK_CPP_EXTERN_END
#endif


