/**
* @file ffw_decode_bar.h
* @author sundaqing (sundaqing@fibocom.com)
* @brief 扫码库API定义
* @version 0.1
* @date 2022-04-21
* 
* Copyright (c) 2022 Fibocom. All rights reserved
* 
 */

#ifndef FD9885E5_81DA_4F56_9ABB_4E0C16B4A6BB
#define FD9885E5_81DA_4F56_9ABB_4E0C16B4A6BB

#include <stdint.h>

/**
* @brief 
* 
* @param verifystr 
* @param ... 
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_decodebar_init(char *verifystr, ...);

/**
* @brief 解码函数
* 
* @param width 图片宽度
* @param height 图片高度
* @param imgdata 图片数据
* @param[out] type 类型
* @param[out] dataout 解码数据缓冲区
* @param dataoutsize @c dataout 的大小
* @param dataout_len 解码数据的大小
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
 */
int ffw_decodebar_decode(int width, int height, char *imgdata, int *type, char *dataout, uint32_t dataoutsize, uint32_t *dataout_len);

/**
* @brief 获取解码sdk版本号
* 
* @param version 
* @param len 
* @return int 
* @retval FFW_R_SUCCESS 成功
* @retval FFW_R_FAILED 失败
*/
int ffw_decodebar_version(char *version, uint32_t len);

#endif /* FD9885E5_81DA_4F56_9ABB_4E0C16B4A6BB */
