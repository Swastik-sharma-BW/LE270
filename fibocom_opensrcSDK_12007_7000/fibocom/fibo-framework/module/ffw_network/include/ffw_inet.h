#ifndef CADB51DD_6C15_4D0C_98B1_9A7A25490CA0
#define CADB51DD_6C15_4D0C_98B1_9A7A25490CA0

#include "ffw_result.h"
#include "ffw_utils.h"
#include "ffw_sockaddr.h"
#include "ffw_socket.h"

/**
* @brief ip地址和端口转换为可以连接的地址
* 
* @param ipaddr ip地址
* @param port 端口
* @param[out] addr_out 转换结果 
* @param[out] addrlen 地址的长度
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败
 */
int ffw_inet_pton(const char *ipaddr, uint16_t port, struct ffw_sockaddr *addr_out, int32_t *addrlen);


/**
* @brief 可以连接的地址转换为ip地址和端口
* 
* @param addr 地址
* @param[out] dst ip地址缓冲区
* @param len @c dst 缓冲区的长度
* @param[out] port 端口
* @return const char* 失败返回NULL, 成功返回 @c dst 
 */
const char *ffw_inet_ntop(struct ffw_sockaddr *addr, char *dst, int len, uint16_t *port);

/**
* @brief 判断IP地址是否合法
* 
* @param ipaddr 
* @return true 合法 IPV4或者IPV6
* @return false 非法地址
 */
bool ffw_inet_ip_valid(const char *ipaddr);

/**
* @brief 字符串形式的IP转为网络形式的IP
* 
* @param af FFW_AF_INET ipv4 FFW_AF_INET6 ipv6 FFW_AF_UNSPEC: ipv4 or ipv6
* @param src ip地址
* @param dst 转换结果
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败
*/
int ffw_ip_pton(int af, const char *src, void *dst);

/**
* @brief 网络形式的IP转换为字符串形式的地址
* 
* @param af FFW_AF_INET ipv4 FFW_AF_INET6 ipv6
* @param src 网络形式的字符串
* @param dst 转换结果缓冲区
* @param size 缓冲区大小
* @return int 
* @retval FFW_R_SUCCESS: 成功
* @retval FFW_R_FAILED: 失败
 */
int ffw_ip_ntop(int af, const void *src, char *dst, int size);

#endif /* CADB51DD_6C15_4D0C_98B1_9A7A25490CA0 */
