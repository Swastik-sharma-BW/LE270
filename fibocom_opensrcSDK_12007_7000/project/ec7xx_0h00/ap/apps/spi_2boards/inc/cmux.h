/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-15    xiangxistu      the first version
 * 2024-02       EC              modify
 */

#ifndef __CMUX_H__
#define __CMUX_H__

#include <stdint.h>
#include "cmsis_os2.h"
#include DEBUG_LOG_HEADER_FILE

#ifdef __cplusplus
extern "C" {
#endif

/* CMUX using long frame mode by default */
#define CMUX_RECV_READ_MAX 2048

#ifndef CMUX_BUFFER_SIZE
#define CMUX_BUFFER_SIZE   (CMUX_RECV_READ_MAX * 2)
#endif

#define CMUX_SW_VERSION           "1.1.0"
#define CMUX_SW_VERSION_NUM       0x10100
//EC add begin

#define CMUX_PORT_NUMBER 4

#define CMUX_CTRL_PORT 0
#define CMUX_AT_PORT 1
#define CMUX_PPP_PORT 2
#define CMUX_OPAQ_PORT 3

typedef int8_t err_t;

typedef enum
{
   CMUX_OK,
   CMUX_ERROR,
   CMUX_NOMEM,
   CMUX_TIMEOUT,
   CMUX_INVAL
}CMUX_ECODE;

typedef enum
{
   CMUX_CLOSE,
   CMUX_OPEN,
   CMUX_OPENFAIL
}CMUX_STATUS;

typedef enum
{
   CMUX_AT_MODE,
   CMUX_CMUX_MODE,
   CMUX_UNKNOWN_MODE
}CMUX_MODE;

struct slist_node
{
    struct slist_node * next;
};

typedef struct slist_node  slist_t;

//EC add end
struct cmux_buffer
{
    uint8_t data[CMUX_BUFFER_SIZE];                    /* cmux data buffer */
    uint8_t *read_point;
    uint8_t *write_point;
    uint8_t *end_point;
    int flag_found;                                       /* the flag whether you find cmux frame */
};

struct cmux_frame
{
    uint8_t channel;                                   /* the frame channel */
    uint8_t control;                                   /* the type of frame */
    int data_length;                                      /* frame length */
    uint8_t *data;                                     /* the point for cmux data */
};

struct frameList
{
    struct cmux_frame *frame;                             /* the point for cmux frame */

    slist_t frame_list;                                /* slist for different virtual serial */
};

struct cmux_vcoms
{
    slist_t flist;                                     /* head of frame_list */

    uint16_t frame_index;                              /* the length of flist */

    uint8_t link_port;                                 /* link port id */

    uint8_t frame_using_status;                         /* This is designed for long frame when we read data; the flag will be "1" when long frame haven't reading done */

    struct cmux_frame *frame;

    size_t length;

    uint8_t *data;
    
    osTimerId_t openTimer;

    uint8_t openRetryTimes;

    uint8_t openStat;
};


struct cmux
{
    const struct cmux_ops *ops;                           /* cmux device ops interface */
    struct cmux_buffer *buffer;                           /* cmux buffer */
    osThreadId_t recv_tid;                                /* receive thread id */
    uint8_t recv_task_run;
    uint8_t vcom_num;                                  /* the cmux port number */
    struct cmux_vcoms *vcoms;                             /* array */

    slist_t list;                                      /* cmux list */
    uint8_t cmux_mode;                                 /* CMUX_MODE */
    void *user_data;                                      /* reserve */
};

struct cmux_ops
{
    err_t  (*start)     (struct cmux *obj);
    err_t  (*stop)      (struct cmux *obj);
    err_t  (*control)   (struct cmux *obj, int cmd, void *arg);
};



/* those function is opening for external file */
err_t cmux_init(struct cmux *object, uint8_t vcom_num, void *user_data);
err_t cmux_start(struct cmux *object);
err_t cmux_stop(struct cmux *object);
err_t cmux_attach(struct cmux *object, int port);
err_t cmux_detach(int port);
err_t cmux_open(struct cmux *object,int link_port);
size_t cmux_vcom_read(int link_port, void *buffer, size_t size);
size_t cmux_vcom_write(int link_port, const void *buffer, size_t size);

//void cmux_at_cmd_cfg(uint8_t mode, uint8_t subset, uint32_t port_speed, uint32_t N1, uint32_t T1, uint32_t N2,
//        uint32_t T2, uint32_t T3, uint32_t k);

/* cmux_utils */
uint8_t cmux_frame_check(const uint8_t *input, int length);

#ifdef  __cplusplus
    }
#endif

#endif  /* __CMUX_H__ */
