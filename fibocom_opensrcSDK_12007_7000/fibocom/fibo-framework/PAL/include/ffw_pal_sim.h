#ifndef _FFW_PAL_SIM_H_
#define _FFW_PAL_SIM_H_
/**
 * @file ffw_pal_sim.h
 * @brief ffw sim pal interface
 * @version 0.1
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned long ffw_uint32;
typedef unsigned short ffw_uint16;
typedef unsigned char ffw_uint8;
typedef char ffw_int8;
typedef signed long ffw_int32;
typedef bool ffw_bool;
typedef void ffw_void;
typedef void *ffw_pal_handlep_t;


#define FFW_PAL_SIM1_ID    0
#define FFW_PAL_SIM2_ID    1
#define FFW_PAL_SIM_MAX    2


#define FFW_PAL_APDU_LEN_MAX (1024)
#define FFW_PAL_APDU_RSP_LEN_MAX (4096) //EC platform max lenth of apdu res is 4K

#define FFW_PAL_SIM_LOCK_MAX (24)

/**
 * @brief reset sim
 *
 * @param[in] sim_id SIMID
 * @param[in] mode
 * @return ffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_reset(ffw_uint8 sim_id,ffw_bool mode);

typedef struct
{
    ffw_uint8 slot_id; // 0-sim1, 1-sim2
    ffw_uint8 slot_status; // 0-deactived, 1-actived
    ffw_uint8 sim_indicator; // 0--SIM not present, 1 -- esim no profile, 2-- SIM present, 3 -- ESIM profile
} ffw_pal_sim_slot_info_t;

/**
 * @brief get sim slot infomation
 *
 * @param[in,out] slot_info sim slot info
 * @return fffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_slot_info_get(ffw_pal_sim_slot_info_t *slot_info);


typedef enum{
    FFW_PAL_SIM_STATUS_REMOVED = 0,
    FFW_PAL_SIM_STATUS_SIM_BUSY = 1,
    FFW_PAL_SIM_STATUS_SIM_PIN = 2,
    FFW_PAL_SIM_STATUS_SIM_PUK = 3,
    FFW_PAL_SIM_STATUS_PH_NET_PIN =4,
    FFW_PAL_SIM_STATUS_ERROR = 5,
    FFW_PAL_SIM_STATUS_EMPT_EUICC = 6,
    FFW_PAL_SIM_STATUS_READY = 7,
    FFW_PAL_SIM_STATUS_INSERT = 8
}ffw_pal_sim_status_e;

typedef enum {
    FFW_SIM_HWCTRL_UNKNOWN = 0x00,    /**<  Unknown \n  */
    FFW_SIM_HWCTRL_SET_HOTSWAP = 0x01,    /**<  SET HOTSWAP \n  */
    FFW_SIM_HWCTRL_GET_HOTSWAP = 0x02,    /**<  GET HOTSWAP \n  */
    FFW_SIM_HWCTRL_SET_SIMPHASE = 0x03,   /**<  SET SIMPHASE \n  */
    FFW_SIM_HWCTRL_GET_SIMPHASE = 0x04,   /**<  SET SIMPHASE \n  */
}ffw_pal_sim_hwctrl_e;

typedef enum {
    FFW_PAL_SIM_SIMLOCK_INVALID = (ffw_int32)0xffffffff, /**<  invalid SIMLOCK option \n  */
    FFW_PAL_SIM_SIMLOCK_ONOFF = 0x00,    /**<  enable or disable simlock \n  */
    FFW_PAL_SIM_SIMLOCK_PLMN_CFG = 0x01,    /**<  set simlock plmns \n  */
    FFW_PAL_SIM_SIMLOCK_PLMN_ADD = 0x02,    /**<  add simlock plmn \n  */
    FFW_PAL_SIM_SIMLOCK_PLMN_DEL = 0x03,    /**<  del exist an lock plmn \n  */
    FFW_PAL_SIM_SIMLOCK_CK = 0x04,    /**<  set sim lock CK \n  */
    FFW_PAL_SIM_SIMLOCK_SIMCHECK_GET = 0x04,    /**<  sim lock check status \n  */
    FFW_PAL_SIM_SIMLOCK_OPERATION_MAX,    /**<  SIMLOCK operation max \n  */
}ffw_pal_sim_lock_operation_e;

typedef struct {
    char plmn[6];
}ffw_pal_sim_lock_plmn_t;

typedef struct {
    ffw_uint32                 simlock_plmn_cnt;
    ffw_pal_sim_lock_plmn_t     simlock_plmns[FFW_PAL_SIM_LOCK_MAX];
}ffw_pal_sim_lock_plmn_list_t;

/**
 * @brief get sim status
 *
 * @param[in] sim_id SIM ID
 * @param[out] sim_status pointer of ffw_pal_sim_status_e
 * @return ffw_int32
 */
ffw_int32 ffw_pal_sim_status_get(ffw_uint8 sim_id, ffw_pal_sim_status_e *sim_status);

#define FFW_UIM_APPS_MAX  0x4

typedef enum {
    FFW_UIM_APP_TYPE_UNKOWN = 0x00, /**<  SIM card Unknown\n  */
    FFW_UIM_APP_TYPE_SIM = 0x01, /**<  SIM card \n  */
    FFW_UIM_APP_TYPE_USIM = 0x02, /**<  USIM application \n  */
    FFW_UIM_APP_TYPE_RUIM = 0x03, /**<  RUIM card \n  */
    FFW_UIM_APP_TYPE_CSIM = 0x04, /**<  CSIM application \n  */
    FFW_UIM_APP_TYPE_ISIM = 0x05, /**<  ISIM application  */
}ffw_sim_app_type_e;

typedef enum {
    FFW_UIM_APP_STATE_UNKNOWN = 0x00, /**<  Unknown \n  */
    FFW_UIM_APP_STATE_DETECTED = 0x01, /**<  Detected \n  */
    FFW_UIM_APP_STATE_PIN1_OR_UPIN_REQ = 0x02, /**<  PIN1 or UPIN is required \n  */
    FFW_UIM_APP_STATE_PUK1_OR_PUK_REQ = 0x03, /**<  PUK1 or PUK for UPIN is required \n  */
    FFW_UIM_APP_STATE_PERSON_CHECK_REQ = 0x04, /**<  Personalization state must be checked \n  */
    FFW_UIM_APP_STATE_PIN1_PERM_BLOCKED = 0x05, /**<  PIN1 is blocked \n  */
    FFW_UIM_APP_STATE_ILLEGAL = 0x06, /**<  Illegal \n  */
    FFW_UIM_APP_STATE_READY = 0x07, /**<  Ready  */
}ffw_sim_app_state_e;

typedef struct {

  uint8_t pin_retries;
  /**<   Indicates the number of retries remaining to verify this PIN.*/

  uint8_t puk_retries;
  /**<   Indicates the number of retries remaining to unblock this PIN.*/
}ffw_sim_pin_info_type_t;  /* Type */

#define FFW_UIM_AID_MAX_LEN 32

typedef struct {
    ffw_sim_app_type_e app_type;
    ffw_sim_app_state_e app_state;
    ffw_sim_pin_info_type_t pin1_info;
    ffw_sim_pin_info_type_t pin2_info;
    ffw_uint32  aid_value_len;  /**< Must be set to # of elements in aid_value */
    ffw_int8    aid_value[FFW_UIM_AID_MAX_LEN];
}ffw_pal_sim_app_info_t;

typedef struct {
    ffw_pal_sim_status_e sim_status;
    ffw_uint32           app_num;
    ffw_pal_sim_app_info_t sim_app[FFW_UIM_APPS_MAX];
}ffw_pal_sim_info_t;



/**
 * @brief get sim infomation
 *
 * @param[in] sim_id SIMID
 * @param[out]  sim slot info
 * @return ffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_info_get(ffw_uint8 sim_id, ffw_pal_sim_info_t *sim_info);

/**
 * @brief get sim infomation
 *
 * @param[in] sim_id SIMID
 * @param[in] aid sim application identifier
 * @param[in] aid_len length of aid
 * @param[out] channel_id id of SIM logical channel
 * @return ffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_channel_open(ffw_uint8 simId, ffw_uint8 *aid, ffw_uint32 aid_len, ffw_uint32 *channel_id);


/**
 * @brief get sim infomation
 *
 * @param[in] sim_id SIMID
 * @param[in] channel_id id of SIM logical channel
 * @return ffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_channel_close(ffw_uint8 simId, ffw_uint32 channel_id);


/**
 * @brief get sim infomation
 *
 * @param[in] sim_id SIMID
 * @param[in] channel_id id of SIM logical channel
 * @param[in] apdu apdu send to UIM
 * @param[in] apdu_len length of apdu
 * @param[in] resp_apdu apdu recv from uim
 * @param[in/out] resp_apdu_len max of resp_apdu as in and recv length of resp_apdu as out
 * @return ffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_apdu_send(ffw_uint8 simId, ffw_uint32 channel_id, ffw_uint8 *apdu, ffw_uint32 apdu_len,
                                ffw_uint8 *resp_apdu, ffw_uint32 *resp_apdu_len);


typedef ffw_void (*ffw_pal_sim_info_change_ind_cb)(ffw_uint8 sim_id, ffw_pal_sim_info_t *sim_info, ffw_void *user_data);
ffw_int32 ffw_pal_sim_info_change_indication_register(ffw_pal_sim_info_change_ind_cb cb, ffw_void *user_data);

/**
 * @brief ffw_pal_sim_hwctrl
 *
 * @return ffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_hwctrl(ffw_uint8 sim_id, ffw_pal_sim_hwctrl_e hw_ctrl, ffw_void *arg);


/**
 * @brief ffw_pal_sim_active_slot_get
 *
 * @param[in] id usim_slot
 * @return ffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_active_slot_get(ffw_uint8* id);

/**
 * @brief get sim atr
 *
 * @param[in] sim_id SIMID
 * @param[out] uim atr
 * @param[in/out] atr_len max of uim atr and actually atr len as out
 * @return ffw_int32
 * @retval 0 success
 * @retval otherwise failed
 */
ffw_int32 ffw_pal_sim_atr_get(ffw_uint8 simId, ffw_uint8 *atr, ffw_uint32 *atr_len);



/*!
* @brief verify pin
*
* @param[in] pin_code pin code
* @return ffw_int32
* @retval 0 success
* @retval otherwise failed
*/
ffw_int32 ffw_pal_sim_pin_verify(ffw_uint8 simId, const char *pin_code);


/*!
* @brief puk check
*
* @param[in] puk_code puk code
* @param[in] new_pin new pin
* @return ffw_int32
* @retval 0 success
* @retval otherwise failed
*/
ffw_int32 ffw_pal_sim_pin_unlock(ffw_uint8 simId, const char *puk_code, const char *new_pin);

/*!
* @brief enable or disable pin
*
* @param[in] operation 1:enable pin 0:disbale pin
* @param[in] pin_code pin code
* @return ffw_int32
* @retval 0 success
* @retval otherwise failed
*/
ffw_int32 ffw_pal_sim_pin_control(ffw_uint8 simId, ffw_int32 operation ,const char *pin_code);


/*!
* @brief change pin code
*
* @param[in] old_pin_code old pin
* @param[in] new_pin_code new pin
* @return ffw_int32
* @retval 0 success
* @retval otherwise failed
*/
ffw_int32 ffw_pal_sim_pin_change(ffw_uint8 simId, const char *old_pin_code, const char *new_pin_code);


/*!
* @brief sim lock set
*
* @param[in] opertion opertion
* @param[in] config  config settings
* @param[in] size    size of config
* @return ffw_int32
* @retval 0 success
* @retval otherwise failed
*/
ffw_int32 ffw_pal_sim_lock_config_set(ffw_uint8 simId, ffw_pal_sim_lock_operation_e opertion, void *config, ffw_uint32 size);
/*!
* @brief sim lock get
*
* @param[in] opertion opertion
* @param[out] config  config settings
* @param[in] size    size of config
* @return ffw_int32
* @retval 0 success
* @retval otherwise failed
*/
ffw_int32 ffw_pal_sim_lock_config_get(ffw_uint8 simId, ffw_pal_sim_lock_operation_e opertion, void *config, ffw_uint32 size);


#define FFW_PAL_SIM_IMSI_LEN_MAX          16  /**  Maximum length of IMSI data. */
#define FFW_PAL_SIM_ICCID_LEN_MAX         20  /**  Maximum length of ICCID data. */
#define FFW_PAL_SIM_PHONE_NUMBER_MAX      82 /**  Maximum phone number length. */

typedef enum {
    FFW_PAL_SIM_ID_IMSI = 0,
    FFW_PAL_SIM_ID_ICCID = 1,
    FFW_PAL_SIM_ID_PHONENUMBER = 2
}ffw_pal_sim_id_type_e;

/*!
* @brief get sim id
*
* @param[in] sim_id SIMID
* @param[out] id string of id
* @param[in] len max length of id
* @return ffw_int32
* @retval 0 success
* @retval otherwise failed
*/
ffw_int32 ffw_pal_sim_id_get(ffw_uint8 simId, ffw_pal_sim_id_type_e id_type, void *id, int len);

typedef struct {
    ffw_uint8 max_sim;           //Max sim
    ffw_uint8 active_slot_mask;  //Current Active Slot current active slot，0x1 slot1，0x2 slot2
    ffw_uint8 max_active_sim;    //Max Active SIM , DSDS/DSDA is 2, DSSS is 1
    ffw_uint8 max_data_sim;      //Max Data SIM , DSDS/DSDA is 2, DSSS is 1
}ffw_pal_sim_capability_t;

ffw_int32 ffw_pal_sim_capability_get(ffw_pal_sim_capability_t *sim_capbility);

ffw_int32 ffw_pal_sim_init(void);
#endif



