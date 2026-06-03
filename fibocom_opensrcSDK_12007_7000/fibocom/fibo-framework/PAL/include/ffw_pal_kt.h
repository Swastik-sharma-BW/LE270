/*******************************************************************************
 * Copyright (c) 1999-2021 Shenzhen fibocom  Co., Ltd
 * All rights reserved. 
 * File name: ffw_sms_pal.h
 * Author: chen  <jin.chen@fibocom.com>
 * Created: 2021-06-17
 * Description: sms pal interface 
******************************************************************************/
#ifndef __FFW_PAL_KT_H__
#define __FFW_PAL_KT_H__
#ifdef CONFIG_CUST_KR_FEATURE

#define FFW_PAL_MSG_MAX_ADDR_LEN   80




typedef struct 
{
    uint8_t   year;
    uint8_t   month;
    uint8_t   day;
    uint8_t   hour;
    uint8_t   minute;
    uint8_t   second;
    uint8_t   tz;         /* time zone */
    uint8_t   tzSign;     /* '+'/'-' */
}ffw_pal_kt_sms_timestamp_Info;


typedef struct 
{
    uint8_t  type;
    uint8_t  msgClass;
    uint8_t  alphabet;
    uint8_t  indication;
    uint8_t  dcs;
}ffw_pal_kt_sms_dcs_Info;

typedef struct {

    uint8_t                       firstOctet;                             /* first octet */
    uint8_t                       protocolId;                             /* TP-PID */
    uint8_t                       srcAddrType;                            /* type of mumber for originating address */
    uint16_t                      smsLength;                              /* length of body */
    uint8_t                       srcAddr[FFW_PAL_MSG_MAX_ADDR_LEN + 1];     /* originating address string */
    ffw_pal_kt_sms_timestamp_Info    timeStamp;                              /* TP-SCTS */
    uint8_t                       offset_dcs;
    ffw_pal_kt_sms_dcs_Info          dcsInfo;                             /* TP-DCS */   
    uint8_t                       offset_usrdata;
    uint8_t                       tpUserDataLen;

}ffw_pal_kt_sms_deliver_pdu_t;


typedef struct 
{
    uint16_t   pduLength;
    uint16_t   reserved0;
    uint8_t    pduData[180];
}ffw_pal_kt_sms_pdu_t;

typedef struct
{
    uint8_t sec;   ///< Second
    uint8_t min;   ///< Minute
    uint8_t hour;  ///< Hour
    uint8_t day;   ///< Day
    uint8_t month; ///< Month
    uint16_t year;  ///< Year
    uint8_t wDay;  ///< Week Day
    int tz; ///< time zone
} ffw_pal_kt_local_time_t;

typedef struct
{
    uint8_t sec;   ///< Second
    uint8_t min;   ///< Minute
    uint8_t hour;  ///< Hour
    uint8_t day;   ///< Day
    uint8_t month; ///< Month
    uint16_t year;  ///< Year
    uint8_t wDay;  ///< Week Day
} ffw_pal_kt_hal_rtc_time_t;



int ffw_pal_KT_local_time_get(ffw_pal_kt_local_time_t *time);
int ffw_pal_KT_time_local_to_gmt(ffw_pal_kt_local_time_t *time);

void ffw_pal_KT_del_oldest_sms();
int ffw_pal_KT_dec_deliver_pdu(ffw_pal_kt_sms_deliver_pdu_t *deliver_pdu,ffw_pal_kt_sms_pdu_t *pPduData);
int ffw_pal_KT_rebuild_deliver_pdu(ffw_pal_kt_sms_deliver_pdu_t *deliver_pdu,ffw_pal_kt_sms_pdu_t *pPduData);


int ffw_pal_KT_modify_apn(int8_t set_delete, uint8_t pdptype, char *apn, uint8_t nAuthProt, char *username, char *pwd, uint8_t nSim);

int ffw_pal_KT_getRTC(ffw_pal_kt_hal_rtc_time_t *time);

void ffw_pal_KT_dev_reset();

#endif
#endif
