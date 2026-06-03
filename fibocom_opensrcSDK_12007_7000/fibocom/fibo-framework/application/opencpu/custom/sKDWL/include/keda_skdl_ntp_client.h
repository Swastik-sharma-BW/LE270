/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/
#ifndef _KEDA_API_NTP_CLIENT_H_
#define _KEDA_API_NTP_CLIENT_H_

typedef enum {
    SC_NTP_OP_SET,
    SC_NTP_OP_GET,
    SC_NTP_OP_EXC,
}SCntpOperationType;

typedef struct SCsysTime_s {
  int tm_sec;	//seconds [0,59]
  int tm_min;	//minutes [0,59]
  int tm_hour;  //hour [0,23]
  int tm_mday;  //day of month [1,31]
  int tm_mon;   //month of year [1,12]
  int tm_year; // since 1970
  int tm_wday; // sunday = 0
}SCsysTime_t;

typedef enum {
    SC_NTP_OK = 0,
    SC_NTP_ERROR,
    SC_NTP_ERROR_INVALID_PARAM,
    SC_NTP_ERROR_TIME_CALCULATED,
    SC_NTP_ERROR_NETWORK_FAIL,
    SC_NTP_ERROR_TIME_ZONE,
    SC_NTP_ERROR_TIME_OUT,
    SC_NTP_END
}SCntpResultType;

typedef SCntpResultType SCntpReturnCode;

/*****************************************************************************
* FUNCTION
*  sAPI_NtpUpdate
*
* DESCRIPTION
*  sAPI_NtpUpdate is used to updata system time with http server . 
* PARAMETERS
*   commad_type:
*       SC_NTP_OP_SET, set host addr
*       SC_NTP_OP_GET, get current host addr, long enough host_addr para is needed
*       SC_NTP_OP_EXC, update sys time by ntp protocol
*   host_addr: NTP server addr
*   time_zone: the local time zone, (-47 to 48)default is 32
*   magQ_urc: the msg queue for urc
* RETURNS
*  SCntpReturnCode  
*
* NOTE
*  Ignoring part of parameters by set NULL or 0.
*****************************************************************************/

SCntpReturnCode sKDWL_NtpUpdate(SCntpOperationType commad_type, char* host_addr, int time_zone, sMsgQRef magQ_urc);

/*****************************************************************************
* FUNCTION
*  sAPI_GetSysLocalTime
*
* DESCRIPTION
*  sAPI_GetSysLocalTime is used to get system local time
* PARAMETERS
*   *currUtcTime
* RETURNS
*  NULL
*
* NOTE
*  you can call this function as following
*  tm_rtc currUtcTime;
*  sAPI_GetSysLocalTime(&currUtcTime);
*  currUtcTime is that we need,you can use currUtcTime.tm_year,currUtcTime.tm_mon....
*****************************************************************************/

void sKDWL_GetSysLocalTime(SCsysTime_t *currUtcTime);

/*****************************************************************************
* FUNCTION
*  sAPI_SetSysLocalTime
*
* DESCRIPTION
*  sAPI_GetSysLocalTime is used to set system local time
* PARAMETERS
*   *timeStr
* RETURNS
*  NULL
*
* NOTE
*  String format is “yy/MM/dd,hh:mm:ss"
*  you can call this function as following
*  char *timeStr="14/01/01,02:14:36";
*  sAPI_GetSysLocalTime(timeStr);
*****************************************************************************/

void sKDWL_SetSysLocalTime(char* timeStr);
#endif