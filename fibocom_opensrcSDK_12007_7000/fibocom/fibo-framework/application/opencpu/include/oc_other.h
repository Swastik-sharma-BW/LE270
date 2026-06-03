#ifndef _OC_OTHER_H_
#define _OC_OTHER_H_

#include <stdbool.h>
#include <stdint.h>

#include "oc_gprs.h"

typedef uint32_t dm_client_handle_type;
typedef uint32_t sim_client_handle_type;

typedef enum
{
    BOOTCAUSE_BY_SOFT,
    BOOTCAUSE_BY_PWRKEY,
    BOOTCAUSE_BY_PINRST,
    BOOTCAUSE_BY_WDG,
} POWER_ON_CAUSE;

typedef enum
{
    SIM_ABSENT = 0x00,
    SIM_NORMAL = 0x01,
} FIBO_SIM_STATUS;

typedef enum
{
    FIBO_SIM_PIN_STATE_READY = 0x00,                 /* <code>: READY, MT is not pending for any password, UICC is ready */
    FIBO_SIM_PIN_STATE_PIN_REQUIRED = 0x01,          /* <code>: SIM PIN, MT is waiting for SIM PIN to be given */
    FIBO_SIM_PIN_STATE_UNBLOCK_PIN_REQUIRED = 0x02,  /* <code>: SIM PUK, MT is waiting for SIM PUK to be given */
    FIBO_SIM_PIN_STATE_PIN2_REQUIRED = 0x03,         /* <code>: SIM PIN2, MT is waiting for SIM PIN2 to be given */
    FIBO_SIM_PIN_STATE_UNBLOCK_PIN2_REQUIRED = 0x04, /* <code>: SIM PUK2, MT is waiting for SIM PUK2 to be given */
    FIBO_SIM_PIN_STATE_UNBLOCK_PIN_BLOCKED = 0x0D    /* <code>: extend state,unblock pin block */
} FIBO_SIM_PIN_STATUS;

typedef enum
{
    FIBO_SIM_STATE_READY = 0x00,
    FIBO_SIM_STATE_PIN_REQUIRED = 0x01,
    FIBO_SIM_STATE_PUK_REQUIRED = 0x02,
    FIBO_SIM_STATE_PIN2_REQUIRED = 0x03,
    FIBO_SIM_STATE_PUK2_REQUIRED = 0x04,
    FIBO_SIM_STATE_HOT_PLUG_IN = 0x05,
    FIBO_SIM_STATE_HOT_PLUG_OUT = 0x06,
    FIBO_SIM_STATE_UNBLOCK_PIN_BLOCKED = 0x0D,  /* <code>: extend state,unblock pin block */
    FIBO_SIM_STATE_NO_SIM = 0xFF
} FIBO_SIM_STATUS_REPORT;

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
} local_time_t;

typedef struct
{
    UINT8   band;
    UINT16  carrier_freq;
    INT16   tx_power;
    UINT16  tx_mode;//0:send only once,1:Periodic send
    UINT16  sig_type;
    UINT16  mcs_index;
    UINT16  rb_start_idx;
    UINT16  rb_num;
} CFW_NW_NST_TX_PARA_CONFIG;

typedef struct
{
    uint8_t sec;   ///< Second
    uint8_t min;   ///< Minute
    uint8_t hour;  ///< Hour
    uint8_t day;   ///< Day
    uint8_t month; ///< Month
    uint16_t year;  ///< Year
    uint8_t wDay;  ///< Week Day
} hal_rtc_time_t;

/**
  \brief      get sim pin status
  \param[out] state   sim pin status,Enum FIBO_SIM_PIN_STATUS
  \param[out] pinremaintries  pin remaining retry counts
  \param[out] pukremaintries  puk remaining retry counts
  \param[in]  simId  sim id,value 0 or 1
  \return     0 - success, !0 - fail
*/
int fibo_sim_pin_status(int *state, unsigned *pinremaintries,unsigned *pukremaintries,uint8_t simid);
INT8 *fibo_get_verno_ys(void);

INT32 fibo_local_time_get(local_time_t *time);
INT32 fibo_local_time_set(local_time_t *time);
INT32 fibo_local_timezone_set(int8_t timezone);
INT32 fibo_local_timezone_get(void);
int fibo_utctime_get(INT32 *utc_time);
INT32 fibo_getRTC(hal_rtc_time_t *time);
INT32 fibo_localtime_r(int64_t ts, struct tm *tm);

//get IMEI and sn
INT32 fibo_get_imei(UINT8 *imei, uint8_t nSimID);
INT32 fibo_get_sn(char* deviceSN, uint8_t nSimID);

//lpg
INT32 fibo_lpg_switch(UINT8 ucLpgSwitch);

//mcal
INT32 fibo_set_mcal(CFW_NW_NST_TX_PARA_CONFIG *nst_para_config, CFW_SIM_ID nSimID);
INT32 fibo_set_mcal_dev(UINT8 ucDevValue,CFW_SIM_ID nSimID);

//sim detect switch
INT8 fibo_sim_detect_switch(uint8_t nSlotStatus);

//Double sim single standby
int fibo_set_sim_slot(uint8_t simid);
uint8_t fibo_get_sim_slot();
int fibo_set_customize_app_register(void *CustomizeHandleFun);

//sim id
INT32 fibo_get_iccid(uint8_t *ccid,uint8_t simId);
INT32 fibo_get_ccid(uint8_t *ccid);

typedef enum
{
    E_FIBO_SIM_APP_TYPE_UNKNOWN   = 0,    /**<  Unknown application type  */
    E_FIBO_SIM_APP_TYPE_3GPP      = 1,    /**< Identify the SIM/USIM application on the card.  */
    E_FIBO_SIM_APP_TYPE_3GPP2     = 2,    /**< Identify the RUIM/CSIM application on the card.  */
    E_FIBO_SIM_APP_TYPE_ISIM      = 3,    /**< Identify the ISIM application on the card.  */
}e_fibo_sim_app_t;

typedef enum
{
    E_FIBO_SIM_SLOT_ID_INVALID    = 0,
    E_FIBO_SIM_SLOT_ID_1          = 1,    /**< Identify card in slot 1.  */
    E_FIBO_SIM_SLOT_ID_2          = 2,    /**< Identify card in slot 2.  */
    E_FIBO_SIM_SLOT_ID_MAX
}e_fibo_sim_slot_id_t;

typedef struct{
    e_fibo_sim_slot_id_t     e_slot_id;  /**< Indicates the slot to be used. */
    e_fibo_sim_app_t         e_app;      /**< Indicates the type of the application. */
}fibo_sim_app_id_info_t; 

int32_t fibo_get_mcc_mnc(
    sim_client_handle_type  h_sim,
    fibo_sim_app_id_info_t  *pt_info,   ///< [IN] The SIM identifier info.
    char                    *mcc, ///< [OUT] mcc
    char                    *mnc///< [OUT] mnc
);

/**
  \brief    get sim status
  \param[out] pucSimStatus   sim status,Enum FIBO_SIM_STATUS
  \param[in] simId  sim id,value 0 or 1
  \return    0 - success, !0 - fail
*/
INT32 fibo_get_sim_status_v2(uint8_t *pucSimStatus,uint8_t simId);
INT32 fibo_get_sim_status(uint8_t *pucSimStatus);

INT32 fibo_get_imsi_asyn(void (*cb)(UINT8 *imsi),uint8_t simId);
//siganl quality
#ifdef CONFIG_CUST_NBSANXING_FEATURE
INT32 fibo_get_csq(INT32 *rssi, INT32 *ber);
#else
INT32 fibo_get_csq(INT32 *rssi, INT32 *ber,uint8_t simId);
#endif
//imsi info
INT32 fibo_get_imsi_by_simid(UINT8 *imsi,uint8_t simId);

//get and set cfun state
INT32 fibo_cfun_set(uint8_t mode,uint8_t sim);
UINT8 fibo_cfun_get(uint8_t sim);

//app version
INT32 fibo_app_version_set(char *ver);
INT32 fibo_app_version_get(char *outstr, int32_t outlen);
INT8 *fibo_get_sw_verno(void);

//Log
int fibo_textTrace(char *fmt, ...);

//boot
UINT8 fibo_getbootcause(void);
// Vbat
INT32 fibo_charge_vbat_vol_get(void);

//cgmm
int32_t fibo_dm_get_cgmm(dm_client_handle_type h_dm, char *p_cgmm_info, int32_t cgmm_info_size);

//get eth/wifi etc. MAC
int32 fibo_dm_get_dev_mac(const char *dev_name, char *mac_addr, size_t addr_size);
#ifdef CONFIG_CUST_XIAONIU_FEATURE
int fibo_get_4g_temperature(void);
#endif
INT32 fibo_setRTC_timezone(int8_t timezone);
INT32 fibo_setRTC(hal_rtc_time_t *time);
#ifdef CONFIG_CUST_NBSANXING_FEATURE
INT32 fibo_get_imsi(UINT8 *imsi);
#endif
#endif
