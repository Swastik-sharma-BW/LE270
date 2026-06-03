/**
 * @file ffw_sockaddr.h
 * @author your name (you@domain.com)
 * @brief 此文件定义了socket编程需要的是地址，struct sockaddr、struct sockaddr_storage、struct sockaddr_in、struct sockaddr_in6
 *        如果有些平台没有定义这些结构体，需要在ffw_plat_header.h包含此文件，如果平台本身已有，忽略此文件（如Linux）
 * @version 0.1
 * @date 2021-10-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef B6AE497C_E752_4ED0_BCD1_E6B1A148C618
#define B6AE497C_E752_4ED0_BCD1_E6B1A148C618

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* POSIX.1g specifies this type name for the `sa_family' member.  */
typedef unsigned short int ffw_sa_family_t;

/* This macro is used to declare the initial common members
   of the data types used for socket addresses, `struct ffw_sockaddr',
   `struct ffw_sockaddr_in', `struct sockaddr_un', etc.  */
#define __FFW_SOCKADDR_COMMON(sa_prefix) ffw_sa_family_t sa_prefix##family

/* Size of struct ffw_sockaddr_storage.  */
#define _FFW_SS_SIZE 128
#define __FFW_SOCKADDR_COMMON_SIZE (sizeof(unsigned short int))
#define __ffw_ss_aligntype unsigned long int
#define _FFW_SS_PADSIZE (_FFW_SS_SIZE - __FFW_SOCKADDR_COMMON_SIZE - sizeof(__ffw_ss_aligntype))

/* Structure describing a generic socket address.  */
struct ffw_sockaddr
{
    uint8_t            sin_len;
    uint8_t   sa_family; /* Common data: address family and length.  */
    char sa_data[14];       /* Address data.  */
};

///struct ffw_sockaddr_storage --> struct sci_sockaddrext
struct ffw_sockaddr_storage
{   
    uint8_t        s2_len;
    uint8_t        ss_family; /* Address family, etc.  */
    char __ss_padding[_FFW_SS_PADSIZE];
    __ffw_ss_aligntype __ss_align; /* Force desired alignment.  */
};

/* Internet address.  */
typedef uint32_t ffw_in_addr_t;
typedef uint16_t ffw_in_port_t;
struct ffw_in_addr
{
    ffw_in_addr_t s_addr;
};

///struct ffw_sockaddr_in --> struct sci_sockaddr
struct ffw_sockaddr_in
{
    uint8_t            sin_len;
    uint8_t     sin_family;
    uint16_t       sin_port;
    struct ffw_in_addr  sin_addr;
    char            sin_zero[8];
};


/* IPv6 address */
struct ffw_in6_addr
{
    union {
        uint8_t __u6_addr8[16];
        uint16_t __u6_addr16[8];
        uint32_t __u6_addr32[4];
    } __in6_u;
    // #define s6_addr __in6_u.__u6_addr8
};

///struct ffw_sockaddr_in --> struct sci_sockaddr6
struct ffw_sockaddr_in6
{
    uint8_t            sin6_len;      /* length of this structure    */
    uint8_t     sin6_family;   /* AF_INET6                    */
    uint16_t       sin6_port;     /* Transport layer port #      */
    uint32_t           sin6_flowinfo; /* IPv6 flow information       */
    struct ffw_in6_addr sin6_addr;     /* IPv6 address                */
    uint32_t           sin6_scope_id; /* Set of interfaces for scope */
};

#include <errno.h>

#endif /* B6AE497C_E752_4ED0_BCD1_E6B1A148C618 */
