/*
*********************************************************************************************************
*                                                FlexST
*                                          The Real-Time soft timer
*
*                               (c) Copyright 2018-2019, tony(gulonglong)
*                                          All Rights Reserved
**   FlexST is provided in source form for FREE evaluation, for educational use or for peaceful research.
*
* File    : os soft timer.h
* By      : tony(gulonglong)
* Version : V1.2
*
*********************************************************************************************************
*/
#ifndef __OS_SOFT_TIMER_H__
#define __OS_SOFT_TIMER_H__

#include <stdint.h>

/********************* os time count for user *****************************/

#define SOFT_TIMER_HZ               10

#define SOFT_RT_TIMER_COUNT_MAX     5

/********************* os timer mode **************************************/

#define SOFT_TIMER_ONCE_MODE        0
#define SOFT_TIMER_PERIOIDC_MODE    1

/********************* os timer err code **********************************/
#define OS_TIME_OK                  0
#define OS_TIME_MODE_ERR          (-2)
#define OS_TIME_OBJECT_ERR        (-3)
#define OS_TIME_VALUE_ERR         (-4)
#define OS_TIME_NOT_RUN_ERR       (-5)

/*********************  STCB struct **************************************/

typedef void (*os_time_callback)(const void *);

typedef void* softTimerHandle_t;


/************************** soft timer functions ************************/

int soft_timer_init(void);

//soft timer user api
softTimerHandle_t soft_timer_register(uint8_t mode, os_time_callback os_time_cb, void *args);

int soft_timer_start(softTimerHandle_t m_timer, uint32_t delayms);

int soft_timer_stop(softTimerHandle_t m_timer);

int soft_timer_updata(softTimerHandle_t m_timer, uint32_t delayms);

int soft_timer_unregister(softTimerHandle_t m_timer);

#endif
