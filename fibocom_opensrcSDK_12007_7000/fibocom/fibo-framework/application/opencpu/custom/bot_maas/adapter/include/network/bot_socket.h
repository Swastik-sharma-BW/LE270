/**
 * @file bot_socket.h
 * @brief bot network component header file
 * @details This file defines the socket interface of the bot network component, which must be adapted
 *
 * @copyright Copyright (C) 2015-2022 Ant Group Holding Limited
 */
#ifndef __BOT_SOCKET_H__
#define __BOT_SOCKET_H__

#if defined(__cplusplus)
extern "C" {
#endif

/** @addtogroup network
 *  network hal API.
 *
 *  @{
 */

#include "bot_platform.h"
#include "bot_sdk_config.h"


/** @enum bot_net_proto_type_e
    @brief the protocol type to connect
*/
typedef enum {
    BOT_NET_TCP = 0,
    BOT_NET_UDP,
    BOT_NET_INV
} bot_net_proto_type_e;

/**
 * @brief tests to see if a file descriptor is part of the set
 *
 * @param[in] fd   the test file descriptor
 * @param[in] set  the set of file descriptor
 * @retval 0 the fd is not in the set
 * @retval others the fd is in the set
 */
int BOT_FD_ISSET(int fd, fd_set *set);

/**
 * @brief set a file descriptor for a set
 *
 * @param[in] fd  the set file descriptor
 * @param[in] set the set of file descriptor
 *
 * @return void
 */
void BOT_FD_SET(int fd, fd_set *set);

/**
 * @brief Clear a set
 *
 * @param[in] set the set which will be cleared
 *
 * @return void
 */
void BOT_FD_ZERO(fd_set *set);

/**
 * @brief get socket errno.
 *
 * @return  socket errno
 */
int bot_socket_errno(void);

/**
 * @brief network address and service translation.
 *
 * @param[in] nodename identify an Internet host
 * @param[in] servname identify a service
 * @param[in] hints the point to an addrinfo structure that specifies criteria for
 *  selecting the socket address structures returned in the list pointed to by res
 * @param[in] res store the returned points
 *
 * @return get addrinfo result
 * @retval 0 success
 * @retval others fail
 */
int bot_getaddrinfo(const char *nodename,
                    const char *servname,
                    const struct addrinfo *hints,
                    struct addrinfo **res);

/**
 * @brief frees the memory that was allocated for the dynamically allocated linked list res
 *
 * @param[in] ai specifies criteria for selecting the socket address structures
 *
 * @return  void
 */
void bot_freeaddrinfo(struct addrinfo *ai);

/**
 * @brief creates an endpoint for communication and returns a descriptor.
 *
 * @param[in] domain    a communication domain
 * @param[in] type      communication semantics
 * @param[in] protocol  a particular protocol to be used with the socket
 *
 * @return a file descriptor for the new socket is returned
 * @retval -1 fail
 * @retval others success
 */
int bot_socket_open(int domain, int type, int protocol);

/**
 * @brief write data to socket.
 *
 * @param[in] sockfd  the file descriptor of the sending socket
 * @param[in] data    the message to send
 * @param[in] size    the message length
 *
 * @return these calls return the number of bytes sent
 * @retval -1 fail
 * @retval others success
 */
int bot_socket_write(int sockfd, const void *data, size_t size);

/**
 * @brief read data from a socket.
 *
 * @param[in] sockfd  the file descriptor of the receiving socket
 * @param[in] data    the buffer to store the received data
 * @param[in] len     the expected length of received data
 *
 * @return the number of bytes received
 * @retval -1 fail
 * @retval others success
 */
int bot_socket_read(int sockfd, void *data, size_t len);

/**
 * @brief  connect the socket referred to by the file descriptor sockfd to the address specified by addr.
 *
 * @param[in] sockfd   the file descriptor of the connecting socket
 * @param[in] addr     the  address  to  which are sent
 * @param[in] addrlen  the size of addr
 *
 * @return socket connect result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @brief  Monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operatio.
 *
 * @param[in] maxfdp1   this argument should be set to the highest-numbered file descriptor in any of the three sets, plus 1
 * @param[in] readset   the set of which will be watched to see if characters become available for reading
 * @param[in] writeset  the set of which will be watched to see if characters become available for write
 * @param[in] exceptset the set of will be watched for exceptional conditions
 * @param[in] timeout   the time should block waiting for a file descriptor to become ready
 *
 * @return socket select result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);

/**
 * @brief  Close the socket referred to by sockfd.
 *
 * @param[in] sockfd  the file descriptor of the closing socket
 *
 * @return socket close result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_close(int sockfd);

/**
 * @brief  Shutdown the socket referred to by sockfd.
 *
 * @param[in] sockfd  the file descriptor of the shutdown socket
 * @param[in] how     the method to shutdown
 *
 * @return socket shutdown result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_shutdown(int socket, int how);

#if PLATFORM_X86
/**
 * @brief Set block flag for file descriptors
 *
 * @param[in] sockfd: file descriptor
 * @param[in] block: block flag
 *
 * @return set block result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_setblock(int sockfd, int block);

/**
 * @brief Set socket options
 *
 * @param[in] sockfd: file descriptor
 * @param[in] level: setting level
 * @param[in] optname: the name of the option
 * @param[in] optval: the value of option
 * @param[in] optlen: the length of the option value
 *
 * @return set option result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_setopt(int sockfd, int level, int optname, const void *optval, int optlen);

/**
 * @brief Bind to network
 *
 * @param[in] sockfd: file descriptor
 * @param[in] addr: address information of server
 * @param[in] port: port information of server
 *
 * @return network bind result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_bind(int sockfd, uint32_t addr, uint16_t port);

/**
 * @brief Listen to network
 *
 * @param[in] sockfd: file descriptor
 * @param[in] n: max count of backlog connection
 *
 * @return network listen result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_listen(int sockfd, int n);

/**
 * @brief Accept network connect
 *
 * @param[in] sockfd: file descriptor
 * @param[out] addr: the accept ip addr
 * @param[out] port: the accept port number
 *
 * @return network connect result
 * @retval 0 success
 * @retval others fail
 */
int bot_socket_accept(int sockfd, uint32_t *addr, uint16_t *port);
#endif
/**
 * @brief  connect the socket with host:port in the given protocol.
 *
 * @param[in] fd     the file descriptor of the connecting socket
 * @param[in] host   Host to connect to
 * @param[in] port   Port to connect to
 * @param[in] proto  Protocol: BOT_NET_TCP or BOT_NET_UDP
 *
 * @return socket app connect result
 * @retval 0 success
 * @retval others fail
 */

int bot_socket_app_connect(int *fd, const char *host, const char *port, bot_net_proto_type_e proto);

/** @} */

#if defined(__cplusplus)
}
#endif
#endif /* __BOT_SOCKET_H__ */

