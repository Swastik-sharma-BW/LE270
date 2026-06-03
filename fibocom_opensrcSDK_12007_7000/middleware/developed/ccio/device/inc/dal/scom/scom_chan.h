/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename: scom_chan.h
*
*  Description:
*
*  History: 2025/1/19 created by xuwang
*
*  Notes: slave channel based on simulated COM port
*
******************************************************************************/
#ifndef SCOM_CHAN_H
#define SCOM_CHAN_H

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include "scom_event.h"
#include "scom_inst.h"

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/
#define SCOM_CHAN_IDX_UNDEF      0xff
#define SCOM_CHAN_FD_UNDEF       0xFFFFFFFF

#define SCOM_CHAN_NAME_DEFAULT   "SCOMCLI"
#define SCOM_CHAN_NAME_MAXSIZE   32
#define SCOM_CHAN_BUF_MAXSIZE    4096


#define SCOM_CHAN_FD_SET(idx)    (idx)

/*----------------------------------------------------------------------------*
 *                   DATA TYPE DEFINITION                                     *
 *----------------------------------------------------------------------------*/

typedef  int32_t (*onRecvCallback)(uint32_t chanIdx, const uint8_t *data, uint32_t dataLen);

typedef struct ScomChannel
{
    uint8_t    isInit;
    uint8_t    idx;
    uint8_t    rsvd[2];
    uint8_t    name[SCOM_CHAN_NAME_MAXSIZE];
    uint32_t   chfd;
    uint32_t   bufLen;
    uint8_t   *buf;      /* base addr of buf */
    ScomInstDrv_t  *instDrv;
    onRecvCallback  recvCb;

    /* more privacy info */
    void           *extras;
}ScomChannel_t;


/*----------------------------------------------------------------------------*
 *                    GLOBAL FUNCTIONS DECLEARATION                           *
 *----------------------------------------------------------------------------*/
int32_t  scomChanOpen(uint32_t chanIdx, onRecvCallback recvCb, void *extras);
int32_t  scomChanClose(uint32_t chanIdx);
int32_t  scomChanSend(uint32_t chanIdx, const uint8_t *data, uint32_t dataLen);


#ifdef __cplusplus
}
#endif
#endif

