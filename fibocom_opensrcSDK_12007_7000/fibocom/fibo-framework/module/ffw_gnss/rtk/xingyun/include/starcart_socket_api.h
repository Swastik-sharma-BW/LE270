/*
 * @Description: 
 * @Author: Chen.xd
 * @Date: 2020-12-23 16:12:16
 */
#ifndef _STARCART_SOCKET_H_
#define _STARCART_SOCKET_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "starcart_sys.h"

    typedef int starcart_socket_t;

    typedef struct
    {
        char *addr;
        short port;
    } starcart_host_info_t;

    typedef struct
    {
        int (*connect_cb)(starcart_int32_t sock_fd);
        int (*send_cb)(starcart_int32_t sock_fd, starcart_int32_t len);
        int (*recv_cb)(starcart_int32_t sock_fd, const void *buf, starcart_int32_t len);
        int (*status_cb)(starcart_int32_t sock_fd, starcart_int32_t status);
    } starcart_socket_cb_t;

    /**
 * @return:
 *   0 if success
 *  -1 if fail
 */
    int starcart_socket_init(void);

    /**
 * @param[in]  nwk_id: network identifier.
 *                     0, By default, then depends on user to choose.
 *                     1, China telecom cellular system
 *                     2, China mobile cellular system
 *                     3, China unicom cellular system
 *                     4, Wi-Fi
 *                     5, Wired
 *                     6, Others
 *
 * @return:
 *   -1 if fail
 *   socket number if okay
 */
    starcart_socket_t starcart_socket_create(starcart_uint32_t nwk_id);

    /**
 * @brief  starcart_socket_connect is a non-blocking interface, if connect success, 
 *         MUST call connect_cb to notify upper layer, or if connect failed, call status_cb
 *         to tell upper layer the reason.
 * @param[in]  sock_fd: socket number.
 * @param[in]  host: the server info @see starcart_host_info_t.
 * @param[in]  cbs: the callbacks for async @see starcart_socket_cb_t.
 *
 * @return:
 *   0 if connect in progress
 *  -1 if fail
 */
    int starcart_socket_connect(starcart_socket_t sock_fd, starcart_host_info_t *host, starcart_socket_cb_t *cbs);

    /**
 * @param[in]  sock_fd: socket number.
 * @param[in]  buf: the data buffer which will be sent.
 * @param[in]  len: the data buffer length.
 *
 * @return:
 *   >0 if success
 *   -1 if fail for any reason
 *   -2 if resource temporarily unavailable
 */
    int starcart_socket_send(starcart_socket_t sock_fd, const void *buf, int len);

    /**
 * @param[in]  sock_fd: socket number.
 * @param[out] buf: the data buffer which will be filled by received data.
 * @param[in]  len: the maxium length of data buffer.
 *
 * @return:
 *  >0 if success
 *  =0 if reset by peer
 *  -1 if fail for any reason
 *  -2 if resource temporarily unavailable
 */
    int starcart_socket_recv(starcart_socket_t sock_fd, void *buf, int len);

    /**
 * @param[in]  sock_fd: socket number.
 *
 * @return:
 *   0 if success
 *  -1 if fail
 */
    int starcart_socket_close(starcart_socket_t sock_fd);

    /**
 * @return:
 *   0 if success
 *  -1 if fail
 */
    int starcart_socket_cleanup(void);

#ifdef __cplusplus
}
#endif
#endif
