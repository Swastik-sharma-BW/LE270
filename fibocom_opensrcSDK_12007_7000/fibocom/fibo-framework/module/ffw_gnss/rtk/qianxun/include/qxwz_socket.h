#include "qxwz_rtos_al.h"

#ifndef QXWZ_SOCKET
#define QXWZ_SOCKET

#define QXWZ_SOC_RECV_BUF_SIZE 1024 * 10
#define QXWZ_SOC_SEND_BUF_SIZE 1024

#define SOCKET_TYPE_BLOCK 1
#define SOCKET_TYPE_NOBLOCK 2

#define MAX_SOCKET_CHANNEL 2

typedef int qxwz_soc;

typedef enum
{
    ERROR_NONE = 0x00,
    CONNECT_ERROR = 0x01,
    SEND_ERROR = 0x02,
    RECV_ERROR = 0x03,
    CLOSE_ERROR = 0x04,
    CONN_TIMEOUT = 0x05,
    SEND_TIMEOUT = 0x06,
    CLOSE_TIMEOUT = 0x07,
    RECV_TIMEOUT = 0x08,
} soc_error_e;

typedef enum
{
    SOCKET_DISCONNECT = 0,
    SOCKET_CONNECTSTART,
    SOCKET_CONNECTING, //check connect
    SOCKET_CONNECTED,  //check read
    SOCKET_CONNECTED2, //check read
    SOCKET_CONNECTERR, //check read
} QxwzSocketFsmN;
typedef enum
{
    SOCKET_READSTOP,
    SOCKET_READSTART,
    SOCKET_READING,
    SOCKET_READED,
    SOCKET_READERR,
} QxwzSocketFsmR;

typedef struct
{
    char *hostName;
    unsigned short port;
} qxwz_soc_address; //

typedef struct _soc_cb_
{
    int (*cb_connect)(int soc);
    int (*cb_send)(int soc);
    int (*cb_recv)(int soc, const char *buf, int len);
    int (*cb_close)(int soc);
    int (*cb_status)(int soc, int status); /*status: <0 means error occurs in low-level tcp/ip stack*/
} soc_cb_t;

typedef struct
{
    unsigned char buf[QXWZ_SOC_RECV_BUF_SIZE];
    size_t length;
    size_t complete_frame_length;
} qxwz_rcv_data_t;

typedef struct
{
    unsigned char buf[QXWZ_SOC_SEND_BUF_SIZE];
    size_t length;
} qxwz_snd_data_t;
typedef enum
{
    SESSION_W_DONE_OKAY = 0,

    SESSION_W_DONE_FAILURE,
} qxwz_session_write_status_e;

typedef void (*qxwz_socket_callback)(char *buf, size_t len, int8_t code);
typedef int (*check_rcv_integrity)(const char *buffer, int length);
typedef void (*write_done_callback)(qxwz_session_write_status_e status);

typedef enum
{
    SESSION_STATE_CLOSED = 0,
    SESSION_STATE_CLOSING,

    SESSION_STATE_CONNECTING,
    SESSION_STATE_CONNECTED,
    SESSION_STATE_DISCONNECTED,

} qxwz_session_state_e;
typedef struct
{
    void *handle;
    int fd;
    int soc;
    char *host;
    short port;
    int type;        // block or no-block
    int conn_status; // connect status / 0 -successful, 1-- in progress, -1 -- failed
    int read_status; // read status / 0 -successful, 1-- in progress, -1 -- failed
    int milsec;
    char *name;
    qxwz_timer_t priv;
    QxwzSocketFsmN nfsm;
    QxwzSocketFsmR rfsm;
    soc_cb_t soc_cb;
    int (*tcp_broken_cb)(void *);
    int32_t connect_broken_cnt;
    qxwz_rcv_data_t rcv_data;
    qxwz_snd_data_t snd_data;
    qxwz_socket_callback session_cb; /*each session should have a first primary callback for rcv data*/
    write_done_callback w_done_cb;   /*after writing is completed, this cb is called*/
    check_rcv_integrity check_cb;    /*after receiving data, check whether it is a complete frame or not, or a error frame; */
    qxwz_bool_t is_longConnection;
    qxwz_bool_t is_dataReady;   /*sync data between receiving and handling*/
    qxwz_bool_t is_unwritable;  /*wonder if it can push data into session*/
    qxwz_bool_t is_dataTooLong; /*exhaust all data from rcv and then discard it if it is too long*/
    void *private;              /*for the private data for a special session*/
    unsigned char retry;        /*counting the retry times of getting hostname's ip*/
    qxwz_soc_address server_addr;
    qxwz_session_state_e state;
} QxwzSocketStruct;

/*
 * @brief: open a socket,  init QxwzSocketStruct 
 * @param[in] handle: ptr to QxwzSocketStruct
 *
 * @return: 
 *  =0 if success
 *  -1 if fail for any reason
 */
extern int qxwz_sock_open(QxwzSocketStruct *handle);

/*
 * @brief: close a socket
 * @param[in] handle: ptr to QxwzSocketStruct

 *
 * @return: none
 */
extern void qxwz_sock_close(QxwzSocketStruct *handle);

/*
 * @brief: read the data from socket , support block or no-block operation
 * @param[in] handle: ptr to QxwzSocketStruct
 * @param[in] vptr: the buffer will be read
 * @param[in] n: attempts to read up to n bytes from file descriptor
 *
 * @return:
 *  > 0 if success, the number of items read.
 *  = 0 if the server is closed.
 *  -1 if fail for any reason
 */
size_t qxwz_sock_read(QxwzSocketStruct *handle, void *vptr, size_t n);

/*
 * @brief: write the data to socket
 * @param[in] handle: ptr to QxwzSocketStruct
 * @param[in] vptr: the buffer will be written
 * @param[in] n:  n bytes will be written to the file descriptor
 *
 * @return:
 *  >=0 if success, the number of items written.
 *  -1 if fail for any reason
 */
extern size_t qxwz_sock_write(QxwzSocketStruct *handle, const void *vptr, size_t n);

/*
 * @brief: connect socket , support block or no-block operation
 * @param[in] handle: a socket pointer
 *
 * @return: none
 *  >= 0 if  success 
 *   1  if in progress
 *  -1 if fail for any reason
 */
extern int qxwz_sock_connect(QxwzSocketStruct *handle);

/*
 * @brief: polling socket , support block or no-block operation
 * @param[in] handle: a socket pointer
 * @param[in] n: number of QxwzSocketStruct
 * @param[in] connect_cb: socket connect callback function pointer
 * @param[in] recv_cb: socket receive callback function pointer
 *
 * @return: none
 */
extern void qxwz_sock_polling(QxwzSocketStruct *handle, int n, void (*connect_cb)(void *), void (*recv_cb)(void *));

#if defined(__EC100Y__)
void qxwz_set_info(struct ql_data_call_info *info1);
#endif

#endif
