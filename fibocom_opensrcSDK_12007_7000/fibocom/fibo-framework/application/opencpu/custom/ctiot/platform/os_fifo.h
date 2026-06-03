/**********************************************************************************************************
*                                                THREAD-OS
*                                          The Real-Time Kernel
*                                  THREAD-OS Configuration File for V1.x
*
*                               (c) Copyright 2018-2019, tony(gulonglong)
*                                          All Rights Reserved
**   THREAD-OS is provided in source form for FREE evaluation, for educational use or for peaceful research.
*
* File    : os_fifo.h
* By      : tony(gulonglong)
* Version : V1.0
*
* This file is a ringbuf module. node composition as follows:
*
*---->fifo{ [len1][------data1--------] [len2][---data2----] [lenx][----lenx------]}
*
*********************************************************************************************************
*/
#ifndef __OS_FIFO_H__
#define __OS_FIFO_H__


#define FIFO_OK                0
#define FIFO_PARAMS_ERR        -1
#define FIFO_FULL_ERR          -2
#define FIFO_EMPTY_ERR         -3
#define FIFO_BUF_SMALL_ERR     -4
#define FIFO_ZREOR_ERR         -5



typedef struct os_fifo_t *os_fifo_handle_t;


/*********************************** use fifo ******************************/


os_fifo_handle_t os_fifo_init(size_t bufSize);
int os_fifo_get(os_fifo_handle_t mFifoRam, void *buf, size_t len);
int os_fifo_copy(os_fifo_handle_t mFifoRam, void *buf, size_t len);
int os_fifo_put(os_fifo_handle_t mFifoRam, const void *buf, size_t len);
int os_fifo_flush(os_fifo_handle_t mFifoRam);
int os_fifo_delete_topnode(os_fifo_handle_t mFifoRam);


#endif
