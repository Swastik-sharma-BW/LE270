/**
 * @Description  : 标准库接口
 * @Version      : 1.0
 * @Author       : ZhongXiaoHui
 * @Date         : 2021-10-09 15:40:34
 * @copyright    : Copyright (C) JUAN. All rights reserved.
 **/
#ifndef APIS_STDLIB_H_
#define APIS_STDLIB_H_

#include "APIS_Macro.h"
NK_CPP_EXTERN_BEGIN

/**
 * @brief
 *  The function returns an integer less than, equal to, or greater than zero if the first n bytes of s1 is found, \n
 *  respectively, to be less than, to match, or be greater than the first n bytes of s2.
 */
NK_API NK_Int
NK_memcmp(const NK_PByte s1, const NK_PByte s2, NK_Size n);


/**
 * @brief
 *  The function scans the initial n bytes of the memory area pointed to by s for the first instance of chr. \n
 *  Both chr and the bytes of the memory area pointed to by s are interpreted as unsigned char.
 */
NK_API NK_PByte
NK_memchr(const NK_PByte s, NK_Char chr, NK_Size n);

/**
 * @brief
 *  The function is like the @ref NK_memchr() function, \n
 *  except that it searches backward from the end of the n bytes pointed to by s instead of forward from the beginning.
 */
NK_API NK_PByte
NK_memrchr(const NK_PByte s, NK_Char chr, NK_Size n);


/*
 * Find the first occurrence of the byte string needle in byte string haystack.
 */
NK_API NK_PByte
NK_memmem(const NK_PByte haystack, NK_Size haystacklen, const NK_PByte needle, NK_Size needlelen);

/**
 *  The function calculates the length of the string s, excluding the terminating null byte ('\0').
 */
NK_API NK_Size
NK_strlen(const NK_PChar s);

/**
 * @brief
 *  The function finds the first occurrence of the substring needle in the string haystack.  The terminating null bytes ('\0') are not compared.
 */
NK_API NK_PChar
NK_strstr(const NK_PChar haystack, NK_PChar needle);

/**
 * copies the string pointed to by src, including the terminating null byte ('\0'), to
 * the buffer pointed to by dest.  The strings may not overlap, and the destination string dest must be large
 * enough to receive the copy.  Beware of buffer overruns!  (See BUGS.)
 *
 */
NK_API NK_PChar
NK_strcpy(NK_PChar dest, const NK_PChar src);


/**
 * The strncpy() function is similar, except that at most n bytes of src are copied.  Warning: If there is no
 * null byte among the first n bytes of src, the string placed in dest will not be null-terminated.
 *
 */
NK_API NK_PChar
NK_strncpy(NK_PChar dest, const NK_PChar src, NK_Size n);


/**
 * The NK_memcpy() function copies n bytes from memory area src to memory area dest.  The memory areas must not
 * overlap.  Use NK_memmove() if the memory areas do overlap.
 *
 */
NK_API NK_PByte
NK_memcpy(NK_PByte dest, const NK_PByte src, NK_Size n);



/**
 * The  memmove()  function  copies  n bytes from memory area src to memory area dest.  The memory areas may
 * overlap: copying takes place as though the bytes in src are first copied into a temporary array that does
 * not overlap src or dest, and the bytes are then copied from the temporary array to dest.
 */
NK_API NK_PByte
NK_memmove(NK_PByte dest, const NK_PByte src, NK_Size n);

NK_API NK_Int
NK_atoi(const NK_PChar str);

NK_API NK_Int
NK_snprintf(NK_PChar str, NK_Size size, const NK_Char *format, ...);

/**
* @brief
*  获取进系统时间戳（单位：ms）
*/
NK_Size64
NK_Clock(NK_Void);

/**
 * @brief 将字符转为大写字母
 * @param c 参数c要么是EOF，要么是一个可以用unsigned char类型表示的值
 * @return NK_Int 如果c是小写字母，如果在当前语言环境中存在大写表示，则返回其大写值;
 */
NK_Int
NK_toupper(NK_Int c);

/**
 * @brief 将字符转为小写字母
 * @param c 参数c要么是EOF，要么是一个可以用unsigned char类型表示的值
 * @return NK_Int 如果c是大写字母，如果在当前语言环境中存在小写表示，则返回其小写值;
 */
NK_Int
NK_tolower(NK_Int c);

/** @brief 可重入的字符串截取函数
 *        The saveptr argument is a pointer to a char * variable that is used internally by strtok_r()
 *        in order to maintain context between successive calls that parse the same string.
 *        On the first call to strtok_r(), str should point to the string to be parsed, and the value of  saveptr  is  ignored.
 *        In subsequent calls, str should be NULL, and saveptr should be unchanged since the previous call.
 * @param str 待截取的字符串
 * @param delim 分隔符
 * @param saveptr
 * @return NK_PChar 成功返回截取后的字符串,失败返回NULL
 */
NK_PChar
NK_strtok_r(NK_PChar str, const NK_PChar delim, NK_PChar *saveptr);

NK_CPP_EXTERN_END
#endif
