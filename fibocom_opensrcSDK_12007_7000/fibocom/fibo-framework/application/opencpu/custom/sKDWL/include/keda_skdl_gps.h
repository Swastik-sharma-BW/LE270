#ifndef __KEDA_SKDL_GPS_H__
#define __KEDA_SKDL_GPS_H__


/****************************************************************************
    Define enum  
*****************************************************************************/
typedef enum
{
    SC_GNSS_RETURN_CODE_OK                =  0,
    SC_GNSS_RETURN_CODE_ERROR             = -1,

    SC_GNSS_CODE_SOCKRT_ERROR             = 101,
    SC_GNSS_CODE_GET_SERVER_ERROR         = 102,
    SC_GNSS_CODE_CONNECT_ERROR            = 103,
    SC_GNSS_CODE_WRITE_SOCKET_ERROR       = 104,
    SC_GNSS_CODE_READ_SOCKET_ERROR        = 105,
    SC_GNSS_CODE_GET_DATA_ERROR           = 106,
    SC_GNSS_CODE_SEND_DATA_ERROR          = 107,
    SC_GNSS_CODE_DOWNLOAD_AGNSS_DATA_TO_NVM_ERROR           = 108,

    SC_GNSS_CODE_MAX                      =0xFF
}SC_Gnss_Return_Code;

typedef enum
{
    SC_GNSS_POWER_OFF = 0,
    SC_GNSS_POWER_ON =  1
}SC_Gnss_Power_Status;

typedef enum
{
    SC_GNSS_AP_FLASH_OFF = 0,
    SC_GNSS_AP_FLASH_ON =  1
}SC_Gnss_Ap_Flash_Status;

typedef enum
{
    SC_GNSS_STOP_OUTPUT_NMEA_DATA  = 0,
    SC_GNSS_START_OUTPUT_NMEA_DATA =  1
}SC_Gnss_Output_Control;

typedef enum
{
    SC_GNSS_NMEA_DATA_GET_BY_PORT = 0,   //get data by NMEA port
    SC_GNSS_NMEA_DATA_GET_BY_URC  =  1   //get data by URC report
}SC_Gnss_Nmea_Data_Get;

typedef enum
{
    SC_GNSS_START_HOT        = 0,  /*0:hot 1:warm 2:cold*/
    SC_GNSS_START_WARM       = 1,
    SC_GNSS_START_COLD       = 2,
    SC_GNSS_START_MAX
}SC_Gnss_Start_Mode;


typedef enum
{
#ifdef JACANA_GPS_SUPPORT
    SC_GNSS_NMEA_UPDATE_RATE_1HZ        = 1,
#else //SIMCOM_UNICORECOMM_GNSS_SUPPORT
    SC_GNSS_NMEA_UPDATE_RATE_1HZ        = 1,
    SC_GNSS_NMEA_UPDATE_RATE_2HZ        = 2,
    SC_GNSS_NMEA_UPDATE_RATE_5HZ        = 5,
#endif
    SC_GNSS_NMEA_RATE_MAX
}SC_Gnss_Nmea_Rate;

typedef void   *sMsgQRef;

typedef enum {

  SC_LBS_SUCCESS,
  SC_LBS_FAIL, 
  SC_LBS_INVALID_PARAMETER,
  SC_LBS_SIMCARD_NOT_READY,
  SC_LBS_RESULT_NETWORK_ERROR,
  SC_LBS_GET_LOC_FAIL,
  SC_LBS_ERROR_END
}SC_lbs_err_e;
  
typedef  SC_lbs_err_e SC_LBS_RETURNCODE;

/****************************************************************************
    Define struct 
*****************************************************************************/



/*****************************************************************************
 * FUNCTION
 *  sKDWL_GnssPowerStatusSet
 *
 * DESCRIPTION
 *  GNSS power control
 *
 * PARAMETERS
 *  [power] power on/off
 *
 * RETURNS
 *  0: set done        -1: set fail
 *
 * NOTE
 *
 *****************************************************************************/
SC_Gnss_Return_Code sKDWL_GnssPowerStatusSet(SC_Gnss_Power_Status power);
/*****************************************************************************
 * FUNCTION
 *  sKDWL_GnssPowerStatusGet
 *
 * DESCRIPTION
 *  Get the status of GNSS power
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  SC_GNSS_POWER_OFF: power off        SC_GNSS_POWER_ON: power on
 *
 * NOTE
 *
 *****************************************************************************/
SC_Gnss_Power_Status sKDWL_GnssPowerStatusGet(void);

/*****************************************************************************
 * FUNCTION
 *  sKDWL_GnssNmeaDataGet
 *
 * DESCRIPTION
 *  Get NMEA data by NMEA port or URC report.
 *
 * PARAMETERS
 *  ctl: control whether the data is output.
 *  mode: Get NMEA data by NMEA port or URC report.
 *
 * RETURNS
 *  0: set ok       -1: set fail
 *
 * NOTE
 *
 *****************************************************************************/
SC_Gnss_Return_Code sKDWL_GnssNmeaDataGet(SC_Gnss_Output_Control ctl,SC_Gnss_Nmea_Data_Get mode);
/*****************************************************************************
 * FUNCTION
 *  sKDWL_GnssStartMode
 *
 * DESCRIPTION
 *  GNSS start mode
 *
 * PARAMETERS
 *  [mode] hot start/cold start
 *
 * RETURNS
 *  0: set ok       -1: set fail
 *
 * NOTE
 *
 *****************************************************************************/
SC_Gnss_Return_Code sKDWL_GnssStartMode(SC_Gnss_Start_Mode mode);

/*****************************************************************************
 * FUNCTION
 *  sKDWL_GnssInfoGet
 *
 * DESCRIPTION
 *  get gnss information
 *  
 * PARAMETERS
 *  [period] The rang is 0-255, unit is second. after set <period> will report 
 *the GNSS information every the seconds.
 *
 * RETURNS
 *  0: set done        -1: set fail  
 *
 * NOTE
 *  
 *****************************************************************************/
SC_Gnss_Return_Code sKDWL_GnssInfoGet(UINT8 period);

SC_Gnss_Return_Code sKDWL_GnssLocationGet(char *data_out, int data_len);

SC_LBS_RETURNCODE sKDWL_LocGet(int channel, sMsgQRef magQ_urc,int type);

#endif


