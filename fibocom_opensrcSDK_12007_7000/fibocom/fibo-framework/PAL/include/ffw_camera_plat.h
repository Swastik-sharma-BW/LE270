/**
* @file ffw_camera_plat.h
* @author sundaqing (sundaqing@fibocom.com)
* @brief 摄像头API定义
* @version 0.1
* @date 2022-04-21
* 
* Copyright (c) 2022 Fibocom. All rights reserved
* 
 */

#ifndef AA8A6120_457D_4347_BAD8_0AE53F67227A
#define AA8A6120_457D_4347_BAD8_0AE53F67227A

#include <stdint.h>

typedef struct ffw_camera_info_s
{
    char *name;
    uint32_t img_width;
    uint32_t img_height;
    uint32_t pixcels;
} ffw_camera_info_t;

/**
* @brief 打开摄像头
* 
* @return int 
 */
int ffw_camera_init(void);

/**
* @brief 关闭摄像头
* 
* @return int 
 */
int ffw_camera_deinit(void);

/**
* @brief 获取摄像头信息
* 
* @param Sensorinfo 
* @return int 
 */
int ffw_get_sensorinfo(ffw_camera_info_t *Sensorinfo);

/**
* @brief 开始预览
* 
* @return int 
 */
int ffw_camera_startpreview(void);

/**
* @brief 停止预览
* 
* @return int 
 */
int ffw_camera_stoppreview(void);

/**
* @brief 获取预览数据
* 
* @param preview_data 
* @return int 
 */
int ffw_camera_get_previewdata(uint8_t **preview_data);

#endif /* AA8A6120_457D_4347_BAD8_0AE53F67227A */
