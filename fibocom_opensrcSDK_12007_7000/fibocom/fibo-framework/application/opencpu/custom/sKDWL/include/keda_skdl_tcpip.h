#ifndef _KEDA_SKDL_TCPIP_H_
#define _KEDA_SKDL_TCPIP_H_

#define SC_FD_SETSIZE      128

#define SC_SOL_SOCKET      0xfff    /* options for socket level */
#define SC_SO_ERROR        0x1007


#define SC_ECONNRESET      104
#define SC_ECONNABORTED    103  /* Software caused connection abort */

#define SC_AF_INET         2
#define SC_AF_INET6        10
#define SC_INADDR_ANY      ((INT32)0x00000000)

/* Socket protocol types (TCP/UDP/RAW) */
#define SC_SOCK_STREAM     1
#define SC_SOCK_DGRAM      2
#define SC_SOCK_RAW        3

#define SC_INVALID_SOCKET  (0xFFFFFFFFL)
#define SC_SOCKET_ERROR    (0xFFFFFFFFL)

#define SC_SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SC_SO_REUSEADDR    0x0004 /* Allow local address reuse */
#define SC_SO_KEEPALIVE    0x0008 /* keep connections alive */

#define SC_TCP_NODELAY         0x01    /* don't delay send to coalesce packets */
#define SC_TCP_KEEPALIVE       0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#define SC_TCP_KEEPIDLE        0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define SC_TCP_KEEPINTVL       0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define SC_TCP_KEEPCNT         0x05    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */
#define SC_TCP_TIMEROUT        0x06    /* set pcb->TCP_TIMEROUT   - Use number of probes sent for get/setsockopt */
#define SC_TCP_MSS_VALUE       0x07    /* set pcb->mss   - Use number of probes only for get/setsockopt */
#define SC_TCP_RTO_VALUE       0x08    /* set pcb->rto  - Use number of probes only for get/setsockopt */
#define SC_TCP_TXB_UNACK       0x09    /* set (pcb->snd_lbb - pcb->lastack) - how much byte remaind in buffer for unacked */
#define SC_TCP_TXB_REST        0x0A    /* set (pcb->snd_buf) - how much byte remaind in tx buffer can writen by app */
#define SC_TCP_TXB_ACKED       0x0B    /* set (pcb->acked_sum) - the number of accumulation of acked */
#define SC_TCP_CLOSE_TIMEROUT  0x0C    /* set (pcb->close_timeout) - the timerout set of tcp close all */
#define SC_TCP_RST_TRIGGER     0x0D    /* set reset man trigger - call tcp_reset_man_trigger() */
#define SC_TCP_INFO            0x0E    /* get tcp_info */

/*
 * Option flags per-socket.
 */
#define SC_SO_DEBUG        0x0001 /* Unimplemented: turn on debugging info recording */
#define SC_SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SC_SO_REUSEADDR    0x0004 /* Allow local address reuse */
#define SC_SO_KEEPALIVE    0x0008 /* keep connections alive */
#define SC_SO_DONTROUTE    0x0010 /* Unimplemented: just use interface addresses */
#define SC_SO_BROADCAST    0x0020 /* permit to send and to receive broadcast messages (see IP_SOF_BROADCAST option) */
#define SC_SO_USELOOPBACK  0x0040 /* Unimplemented: bypass hardware when possible */
#define SC_SO_LINGER       0x0080 /* linger on close if data present */
#define SC_SO_DONTLINGER   ((int)(~SC_SO_LINGER))
#define SC_SO_OOBINLINE    0x0100 /* Unimplemented: leave received OOB data in line */
#define SC_SO_REUSEPORT    0x0200 /* Unimplemented: allow local address & port reuse */
#define SC_SO_IPSEC        0x0400 /* denote ipsec using */

#define SC_SO_SNDBUF       0x1001  /* Unimplemented: send buffer size */
#define SC_SO_RCVBUF       0x1002  /* receive buffer size */
#define SC_SO_SNDLOWAT     0x1003  /* Unimplemented: send low-water mark */
#define SC_SO_RCVLOWAT     0x1004  /* Unimplemented: receive low-water mark */
#define SC_SO_SNDTIMEO     0x1005  /* Unimplemented: send timeout */
#define SC_SO_RCVTIMEO     0x1006  /* receive timeout */
#define SC_SO_ERROR        0x1007  /* get error status and clear */
#define SC_SO_TYPE         0x1008  /* get socket type */
#define SC_SO_CONTIMEO     0x1009  /* Unimplemented: connect timeout */
#define SC_SO_NO_CHECK     0x100a  /* don't create UDP checksum */
#define SC_SO_BINDTODEVICE 0x100b /* bind to device */
#define SC_SO_BIO          0x100c  /* set socket into blocking mode */
#define SC_SO_NBIO         0x100d  /* set socket into non-blocking mode */
#define SC_SO_NONBLOCK     0x100e  /* set/get socket blocking mode via optval param*/


/** 255.255.255.255 */
#define SC_IPADDR_NONE         ((UINT32)0xffffffffUL)

#define SC_IPPROTO_IP      0
#define SC_IPPROTO_TCP     6
#define SC_IPPROTO_UDP     17

/* Flags we can use with sAPI_TcpipSend and sAPI_TcpipRecv. */
#define SC_MSG_PEEK       0x01    /* Peeks at an incoming message */
#define SC_MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
#define SC_MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
#define SC_MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
#define SC_MSG_MORE       0x10    /* Sender will send more */

#define SC_TCPIP_SUCCESS      0
#define SC_TCPIP_FAIL        -1

/*
 * howto arguments for shutdown(2), specified by Posix.1g.
 */
enum
{
  SC_SHUT_RD	=0,		/* shut down the reading side */
  SC_SHUT_WR,			/* shut down the writing side */
  SC_SHUT_RDWR			/* shut down both sides */
};		

typedef struct
{
    INT32 tv_sec;        /* seconds */
    INT32 tv_usec;       /* and microseconds */
} SCtimeval;
#if 0
enum SCnetType
{
    TCPIP_PDP_IPV4 = 1,
    TCPIP_PDP_IPV6,
    TCPIP_PDP_IPV4V6,
    TCPIP_PDP_INVALID
};

struct SCipInfo
{
    enum SCnetType type;
    UINT32 ip4;
    UINT32 ip6[4];
};
#endif
typedef struct
{
    UINT32 s_addr;
} SCinAddr;

typedef struct
{
    UINT8 sin_len;
    UINT8 sin_family;
    UINT16 sin_port;
    SCinAddr sin_addr;
#define SC_SIN_ZERO_LEN 8
    INT8 sin_zero[SC_SIN_ZERO_LEN];
} SCsockAddrIn;



typedef struct
{
    UINT8 fd_bits [(SC_FD_SETSIZE * 2 + 7) / 8];
} SCfdSet;


typedef struct
{
    INT8  *h_name;      /* Official name of the host. */
    INT8 **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                           terminated by a null pointer. */
    INT32    h_addrtype;  /* Address type. */
    INT32    h_length;    /* The length, in bytes, of the address. */
    INT8 **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
#define h_addr h_addr_list[0] /* for backward compatibility */
} SChostent;

typedef void   *sMsgQRef;


typedef struct
{
    UINT8 sa_len;
    UINT8 sa_family;
#if LWIP_IPV6
    UINT8 sa_data[22];
#else /* LWIP_IPV6 */
    UINT8 sa_data[14];
#endif /* LWIP_IPV6 */
} SCsockAddr;

struct SCaddrinfo {
    int               ai_flags;      /* Input flags. */
    int               ai_family;     /* Address family of socket. */
    int               ai_socktype;   /* Socket type. */
    int               ai_protocol;   /* Protocol of socket. */
    UINT32         ai_addrlen;    /* Length of socket address. */
    SCsockAddr *ai_addr;       /* Socket address of socket. */
    char             *ai_canonname;  /* Canonical name of service location. */
    struct SCaddrinfo  *ai_next;       /* Pointer to next in list. */
};

#define SC_IOCPARM_MASK    0x7fU           /* parameters must be < 128 bytes */
#define SC_IOC_VOID        0x20000000UL    /* no parameters */
#define SC_IOC_OUT         0x40000000UL    /* copy out parameters */
#define SC_IOC_IN          0x80000000UL    /* copy in parameters */
#define SC_IOC_INOUT       (SC_IOC_IN|SC_IOC_OUT)
/* 0x20000000 distinguishes new &
   old ioctl's */
#define SC_IO(x,y)        (SC_IOC_VOID|((x)<<8)|(y))

#define SC_IOR(x,y,t)     (SC_IOC_OUT|(((UINT32)sizeof(t)&SC_IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define SC_IOW(x,y,t)     (SC_IOC_IN|(((UINT32)sizeof(t)&SC_IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define SC_FIONBIO     SC_IOW('f', 126, UINT32) /* set/clear non-blocking i/o */

#define SC_SOCKET_OFFSET  (0) /*note: must set to 0*/

#define SC_MEMP_NUM_NETCONN 128

#define SC_FDSETSAFESET(n, code)  do {\
    if ((((int)(n) - SC_SOCKET_OFFSET) < (SC_MEMP_NUM_NETCONN * 2)) \
       && (((int)(n) - SC_SOCKET_OFFSET) >= 0)) {code;}\
    } while(0)

#define SC_FDSETSAFEGET(n, code) (((((int)(n) - SC_SOCKET_OFFSET) < (SC_MEMP_NUM_NETCONN * 2)) \
       && (((int)(n) - SC_SOCKET_OFFSET) >= 0)) ? (code) : 0)

#define SC_FD_SET(n, p)  SC_FDSETSAFESET(n, (p)->fd_bits[((n) - SC_SOCKET_OFFSET)/8] |=  (1 << (((n) - SC_SOCKET_OFFSET) & 7)))
#define SC_FD_CLR(n, p)  SC_FDSETSAFESET(n, (p)->fd_bits[((n) - SC_SOCKET_OFFSET)/8] &= ~(1 << (((n) - SC_SOCKET_OFFSET) & 7)))
#define SC_FD_ISSET(n,p) SC_FDSETSAFEGET(n, (p)->fd_bits[((n) - SC_SOCKET_OFFSET)/8] &   (1 << (((n) - SC_SOCKET_OFFSET) & 7)))
#define SC_FD_ZERO(p)    memset((void*)(p),0,sizeof(*(p)))


enum
{
    SC_SOCKET_NONE,
    SC_SOCKET_WRITE,
    SC_SOCKET_LISTEN,
    SC_SOCKET_CONNECT,
    SC_SOCKET_CLOSE
};

/** Use to inform the callback function about changes */
enum 
{
  SC_NETCONN_EVT_RCVPLUS,
  SC_NETCONN_EVT_RCVMINUS,
  SC_NETCONN_EVT_SENDPLUS,
  SC_NETCONN_EVT_SENDMINUS,
  SC_NETCONN_EVT_CONNECTED,
  SC_NETCONN_EVT_ACCEPTPLUS,
  SC_NETCONN_EVT_ERROR_CLSD,
  SC_NETCONN_EVT_ERROR_RST,
  SC_NETCONN_EVT_ERROR_ABRT,
  SC_NETCONN_EVT_CLOSE_WAIT,
  SC_NETCONN_EVT_SENDACKED,
  SC_NETCONN_EVT_CLOSE_NORMAL,
  SC_NETCONN_EVT_ERROR
};
/*****************************************************************************
* FUNCTION
*  sKDWL_TcpipSocket
*
* DESCRIPTION
*  Allocates a socket.
*  The parameters specify what type of socket is requested. Since this socket API implementation is concerned only with network sockets,
*  these are the only socket type that is supported. Also, TCP (SOCK STREAM) sockets can be used.
*
* PARAMETERS
*   domain	  Specifies the protocol family of the created socket.
*   type      socket protocol types.
*   protocol  Type of protocol.
*
* RETURNS
*  Upon  successful  completion, sKDWL_TcpipSocket() shall return a non-negative integer, the socket file descriptor.
*  -1 - Failure
*
* NOTE
*
*****************************************************************************/
INT32 sKDWL_TcpipSocket(INT32 domain, INT32 type, INT32 protocol);


/*****************************************************************************
* FUNCTION
*  sKDWL_TcpipConnect
*
* DESCRIPTION
*   Attempts to make a connection on a socket.
* PARAMETERS
*    sockfd	Specifies the index of the socket.
*    addr	Pointer to sockaddr structure that identify connection.
*    addrlen	Size name structure. 
* RETURNS
*    0 - OK
*    -1 - Failure
*
* NOTE
*
*****************************************************************************/
INT32 sKDWL_TcpipConnect(INT32 sockfd, const SCsockAddr *addr, UINT32 addrlen);

/*****************************************************************************
* FUNCTION
*  sAPI_TcpipSend
*
* DESCRIPTION
*   Sends bytes.
*   This function is used in TCP connection for sending data. Before a call to sAPI_TcpipSend() the receiver of the data must have been set up using sAPI_TcpipConnect().
* PARAMETERS
*    s	Specifies the index of the socket.
*    data	Data buffer to send.
*    size	Size of the data.
*    flags	User can use this flags:
*        0x00 - No Flags
*        SC_MSG_PEEK - Peeks at an incoming message
*        SC_MSG_DONTWAIT - Nonblocking i/o for this operation only
*        SC_MSG_MORE - Sender will send more
* RETURNS
*    0 - OK
*    -1 - Failure
*
* NOTE
*
*****************************************************************************/
INT32 sKDWL_TcpipSend(INT32 sockfd, const void *buf, INT32 len, INT32 flags);


/*****************************************************************************
* FUNCTION
*  sAPI_TcpipSendto
*
* DESCRIPTION
*   Sends bytes.
*   This function is used in TCP connection for sending data. Before a call to sAPI_TcpipSend() the receiver of the data must have been set up using sAPI_TcpipConnect().
* PARAMETERS
*    s	Specifies the index of the socket.
*    data	Data buffer to send.
*    size	Size of the data.
*    flags	User can use this flags:
*        0x00 - No Flags
*        SC_MSG_PEEK - Peeks at an incoming message
*        SC_MSG_DONTWAIT - Nonblocking i/o for this operation only
*        SC_MSG_MORE - Sender will send more
*    addr	the addr struture with the remote network parameter like port and IP address.
*    addrlen Size name structure.
* RETURNS
*    0 - OK
*    -1 - Failure
*
* NOTE
*
*****************************************************************************/
INT32 sKDWL_TcpipSendto(INT32 sockfd, const void *buf, INT32 len, INT32 flags,
                       const SCsockAddr *dest_addr, UINT32 addrlen);

/*****************************************************************************
* FUNCTION
*  sAPI_TcpipRecvfrom
*
* DESCRIPTION
*   Received bytes.
*   used to receive messages from a socket, and may be used to receive data on a
*   socket whether or not it is connection-oriented.
* PARAMETERS
*    sockfd	Specifies the index of the socket.
*    buf	Memory buffer to store received data.
*    len	Length to data to receiving.
*    flags	User shall use one of followings:
*        0x00 - No Flags
*        SC_MSG_PEEK - Peeks at an incoming message
*        SC_MSG_DONTWAIT - Nonblocking i/o for this operation only
*        SC_MSG_MORE - Wait for more than one message
*    addr	fill the addr struture with the remote network parameter like port and IP address.
*    addrlen Size name structure.
* RETURNS
*    0 - OK
*    -1 - Failure
*
* NOTE
*
*****************************************************************************/
INT32 sKDWL_TcpipRecvfrom(INT32 sockfd, void *buf, INT32 len, INT32 flags,
                         SCsockAddr *src_addr, UINT32 *addrlen);

/*****************************************************************************
* FUNCTION
*  sAPI_TcpipClose
*
* DESCRIPTION
*   Closes a socket.
* PARAMETERS
*    fd	Specifies the index of the socket.
* RETURNS
*    0 - OK
*    -1 - Failure
*
* NOTE
*
*****************************************************************************/
INT32 sKDWL_TcpipClose(INT32 fd);


/*****************************************************************************
* FUNCTION
*  sAPI_TcpipIoctlsocket
*
* DESCRIPTION
*   Returns an entry containing addresses of address family AF_INET for the host 
*   with name name. Due to sKDWL_TcpipGethostbyname limitations, only one address is returned.
* PARAMETERS
*    name	The hostname to resolve
* RETURNS
*    An entry containing addresses of address family AF_INET for the host with name name
*    NULL - Failure
*
* NOTE
*
*****************************************************************************/
 SChostent *sKDWL_TcpipGethostbyname(INT8 *name);

 
/*****************************************************************************
* FUNCTION
*  sKDWL_TcpipHtons
*
* DESCRIPTION
*   Convert an UINT16 from host to network byte order.
* PARAMETERS
*    n	UINT16 in host byte order
* RETURNS
*    in network byte order
*
* NOTE
*
*****************************************************************************/    
UINT16 sKDWL_TcpipHtons(UINT16 hostshort);

/*****************************************************************************
* FUNCTION
*  sAPI_TcpipGetSocketErrno
*
* DESCRIPTION
*    Get the error code of the specified sockfd
* PARAMETERS
*    sockfd Specifies the index of the socket.
Returns
*    -1 - Failure
*    error code
* NOTE
*
*****************************************************************************/   
INT32 sKDWL_TcpipGetSocketErrno(int sockfd);

INT32 sKDWL_TcpipPdpActive(int cid, int channel);

INT32 sKDWL_TcpipPdpDeactive(int cid, int channel);

#endif
