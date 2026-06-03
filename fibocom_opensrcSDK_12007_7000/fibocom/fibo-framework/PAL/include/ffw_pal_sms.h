/*******************************************************************************
 * Copyright (c) 1999-2021 Shenzhen fibocom  Co., Ltd
 * All rights reserved. 
 * File name: ffw_sms_pal.h
 * Author: chen  <jin.chen@fibocom.com>
 * Created: 2021-06-17
 * Description: sms pal interface 
******************************************************************************/        
#ifndef __FFW_PAL_SMS_H__
#define __FFW_PAL_SMS_H__
#include "oc_sms.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long ffw_uint32;
typedef unsigned short ffw_uint16;
typedef unsigned char ffw_uint8;
typedef signed long ffw_int32;

#define FFW_PAL_SMS_MUTEX_INIT_EX(ptr) \
    do { \
        if(NULL!=(ptr))  \
        pthread_mutex_init((ptr), NULL); \
    } while(0)

#define FFW_PAL_SMS_MUTEX_DEINIT_EX(ptr) \
    do { \
        if(NULL!=(ptr))  \
        pthread_mutex_destroy((ptr)); \
    } while(0)        

#define FFW_PAL_SMS_LOCK_EX(ptr) \
    do { \
         if(NULL!=(ptr))  \
         pthread_mutex_lock(ptr); \
    }while(0)

#define FFW_PAL_SMS_UNLOCK_EX(ptr)  \
    do { \
         pthread_mutex_unlock(ptr); \
    }while(0)

#define FFW_PAL_SMS_CENTER_ADDRESS_LEN_MAX             (32)
#define FFW_PAL_SMS_CENTER_ADDRESS_TYPE_LEN_MAX        (4)
#define FFW_PAL_SMS_EXTRA_DA_LEN_MAX                   (20 * 15)
#define FFW_PAL_SMS_EXTRA_TIME_LEN_MAX                 (40)
#define FFW_PAL_SMS_INDEX_ALL_MAX                      (-1)
#define FFW_PAL_SMS_CONT_LEN_MAX                       (640)
#define FFW_PAL_SMS_USER_DATA_MAX_LEN                  (255)

#define FFW_PAL_SMS_CSCA_ADDR_STR_MAX_LEN     ((CMI_SMS_MAX_LENGTH_OF_ADDRESS_VALUE + 1) * 4)
#define FFW_PAL_SMS_CSCA_TYPE_VAL_MIN          0
#define FFW_PAL_SMS_CSCA_TYPE_VAL_MAX          255

typedef enum
{
    FFW_PAL_SMS_ERROR_MIN = -1,
    FFW_PAL_SMS_ERROR_COMMON = FFW_PAL_SMS_ERROR_MIN,

    FFW_PAL_SMS_ERROR_NONE = 0,
    FFW_PAL_SMS_ERROR_INVALID_PARAM,
    FFW_PAL_SMS_ERROR_NOT_SUPPORT,
    FFW_PAL_SMS_ERROR_EXEC_ERROR,
    FFW_PAL_SMS_ERROR_DECODE_ERROR,
    FFW_PAL_SMS_ERROR_INVALID_STORAGE_TYPE,
    FFW_PAL_SMS_ERROR_INVALID_MSG_TYPE,
    FFW_PAL_SMS_ERROR_ENCODE_ERROR,
    FFW_PAL_SMS_ERROR_INPUT_OVERFLOW,
    FFW_PAL_SMS_ERROR_INVALID_PDU_PARAM,
    FFW_PAL_SMS_ERROR_SCA_ADDR_UNKNOWN,
    FFW_PAL_SMS_ERROR_INVALID_TXT_PARAM,
    FFW_PAL_SMS_ERROR_SERVICE_NOT_READY,
    FFW_PAL_SMS_ERROR_SEND_FAILED,
    FFW_PAL_SMS_ERROR_READ_FAILED,
    FFW_PAL_SMS_ERROR_OP_NOT_SUPPORTED,
    FFW_PAL_SMS_ERROR_SET_DOMAIN_PREF_FAILED,
    FFW_PAL_SMS_ERROR_GET_DOMAIN_PREF_FAILED,
    FFW_PAL_SMS_ERROR_UNKNOWN_ERROR,
    FFW_PAL_SMS_ERROR_SNMA_NOT_EXP,
    FFW_PAL_SMS_ERROR_STORE_FULL,

    FFW_PAL_SMS_ERROR_MAX
}ffw_pal_sms_error_code_e;

typedef enum
{
    FFW_PAL_SMS_STATUS_NOT_READY = 0,
    FFW_PAL_SMS_STATUS_READY,
}ffw_pal_sms_service_status_flag_e;

typedef enum
{
    FFW_PAL_SMS_STORAGE_TYPE_INVALID = -1,
    FFW_PAL_SMS_STORAGE_TYPE_UIM = 0,
    FFW_PAL_SMS_STORAGE_TYPE_NV,
    FFW_PAL_SMS_STORAGE_TYPE_AP,
    FFW_PAL_SMS_STORAGE_TYPE_MAX,
}ffw_pal_sms_storage_type_e;

typedef enum
{
    FFW_PAL_SMS_MSG_FORMAT_INVALID = -1,
    FFW_PAL_SMS_MSG_FORMAT_CDMA,
    FFW_PAL_SMS_MSG_FORMAT_GW_PP,
    FFW_PAL_SMS_MSG_FORMAT_GW_BC,
    FFW_PAL_SMS_MSG_FORMAT_MWI,
    FFW_PAL_SMS_MSG_FORMAT_MAX
}ffw_pal_sms_msg_format_e;

typedef enum
{
    FFW_PAL_SMS_MSG_STATUS_INVALID = -1,
    FFW_PAL_SMS_MSG_STATUS_UNREAD = 0,
    FFW_PAL_SMS_MSG_STATUS_READ,
    FFW_PAL_SMS_MSG_STATUS_UNSENT,
    FFW_PAL_SMS_MSG_STATUS_SENT,
} ffw_pal_sms_msg_status_flag_e;

typedef enum
{
    FFW_PAL_SMS_MSG_ACK_FLAG_INVALID = -1,
    FFW_PAL_SMS_MSG_ACK_NONEED = 0,   //no need to ack the received sms message
    FFW_PAL_SMS_MSG_ACK_NEEDED,                //need to ack the received sms message
} ffw_pal_sms_msg_ack_flag_e;


typedef enum
{
    FFW_PAL_SMS_SEND_NO_SAVE = 0,
    FFW_PAL_SMS_SEND_AND_SAVE,
} ffw_pal_sms_send_flag_e;  

typedef enum
{
    FFW_PAL_SMS_DEL_BY_INDEX = 0,
    FFW_PAL_SMS_DEL_MT_READ,
    FFW_PAL_SMS_DEL_MT_NOT_READ,
    FFW_PAL_SMS_DEL_MO_SENT,
    FFW_PAL_SMS_DEL_MO_NOT_SENT,
    FFW_PAL_SMS_DEL_ALL,
} ffw_pal_sms_delete_flag_e;

typedef enum
{
    FFW_PAL_SMS_LIST_MT_READ = 0,
    FFW_PAL_SMS_LIST_MT_NOT_READ,
    FFW_PAL_SMS_LIST_MO_SENT,
    FFW_PAL_SMS_LIST_MO_NOT_SENT,
    FFW_PAL_SMS_LIST_ALL
}ffw_pal_sms_list_flag_e;

typedef enum
{
    FFW_PAL_SMS_FLAG_FULL_INVALID = -1,  //default 
    FFW_PAL_SMS_FLAG_FULL_TRUE   = 0, // SMS storage full
    FFW_PAL_SMS_FLAG_FULL_FALSE    = 1,   //SMS storage not full
}ffw_pal_sms_full_flag_e;

typedef enum
{
    FFW_PAL_SMS_MSG_SERVICE_INVALID = -1,
    FFW_PAL_SMS_MSG_SERVICE_GSM_PHASE2 = 0,
    FFW_PAL_SMS_MSG_SERVICE_GSM_PHASE2_PLUS = 1,
    FFW_PAL_SMS_MSG_SERVICE_MAX,
} ffw_pal_sms_message_service_type_e;

typedef enum
{
    FFW_PAL_SMS_ACK_INVALID = -1,
    FFW_PAL_SMS_ACK_RP_ACK,
    FFW_PAL_SMS_ACK_RP_ERROR,
    FFW_PAL_SMS_STATUS_REPORT_RP_ACK,
    FFW_PAL_SMS_STATUS_REPORT_RP_ERROR
}ffw_pal_sms_ack_flag_e;

typedef enum
{
    FFW_PAL_SMS_IS_SEND_ACK_INVALID = -1,  //default 
    FFW_PAL_SMS_IS_NOT_SEND_ACK = 0, 
    FFW_PAL_SMS_IS_SEND_ACK=1,   
}ffw_pal_sms_is_send_ack_e;

typedef enum
{
    FFW_PAL_SMS_SNMI_MT_INVALID = -1,
    FFW_PAL_SMS_SNMI_MT_NO_IND,
    FFW_PAL_SMS_SNMI_MT_IND_MEM_IDX,
    FFW_PAL_SMS_SNMI_MT_IND_CONTENT,
    FFW_PAL_SMS_SNMI_MT_ROUTE_CLASS3,
    FFW_PAL_SMS_SNMI_MT_MAX
} ffw_pal_sms_snmi_mt_type_e;

typedef enum
{
    FFW_PAL_SMS_DOMAIN_PREF_INVALID = -1,
    FFW_PAL_SMS_DOMAIN_PREF_PS_ONLY,
    FFW_PAL_SMS_DOMAIN_PREF_CS_ONLY,
    FFW_PAL_SMS_DOMAIN_PREF_PS_PREF,
    FFW_PAL_SMS_PREF_CS_PREF,
    FFW_PAL_SMS_DOMAIN_PREF_MAX
} ffw_pal_sms_domain_pref_type_e;

typedef enum
{
    FFW_PAL_SMS_SNMI_DS_INVALID = -1,
    FFW_PAL_SMS_SNMI_DS_NO_IND,
    FFW_PAL_SMS_SNMI_DS_IND_CONTENT,
    FFW_PAL_SMS_SNMI_DS_IND_MEM_IDX,
    FFW_PAL_SMS_SNMI_DS_MAX
} ffw_pal_sms_snmi_ds_type_e;

typedef enum 
{
    FFW_PAL_SMS_EVENT_INVALID = -1,
    FFW_PAL_SMS_EVENT_UNKNOWN,
    FFW_PAL_SMS_EVENT_SERVICE_STATUS_CHANGE,
    FFW_PAL_SMS_EVENT_SMMFULL,
    FFW_PAL_SMS_EVENT_NEW_MT_MSG,
    FFW_PAL_SMS_EVENT_NEW_ROUTE_MSG,
    FFW_PAL_SMS_EVENT_NEW_AP_MSG,
    FFW_PAL_SMS_M2M_EVENT_WMS,
} ffw_pal_sms_event_e;

typedef enum 
{
    FFW_PAL_SMS_ACK_ERROR = 0,
    FFW_PAL_SMS_ACK_OK
} ffw_pal_sms_ack_type_e;

typedef struct
{
    uint8_t total_segments;     /**<     The number of long  short message*/
    uint8_t seg_number;         /**<     Current number.*/
    uint8_t reference_number;   /**< reference_number.*/
}ffw_sms_pal_udh_t;


typedef struct
{
    ffw_pal_sms_msg_format_e   format;
    ffw_pal_sms_msg_status_flag_e status;
    ffw_int32 pdu_data_len;
    ffw_uint8 pdu_data[FFW_PAL_SMS_CONT_LEN_MAX];
    char     extra_da[FFW_PAL_SMS_EXTRA_DA_LEN_MAX];          //reserved for AP storerage
    fibo_sms_timestamp     extra_time;   //reserved for AP storerage
    uint8_t data_head_valid;
    ffw_sms_pal_udh_t data_head;
}ffw_pal_sms_msg_t;

typedef struct
{
    ffw_int32 used_cnt;
    ffw_int32 max_cnt;
}ffw_pal_sms_loc_store_info_t;

typedef struct
{
    ffw_int32                        storage_index;
    ffw_pal_sms_msg_status_flag_e    msg_flag;
} ffw_pal_sms_msg_info_t;

typedef struct
{
    ffw_uint32                     ind_id;
    void*                          ind_data;
    ffw_uint32                     data_size;
} ffw_pal_sms_ind_data_type_t;

typedef struct
{
    ffw_uint32     wms_event;
    ffw_uint32     wms_transaction_id;
    ffw_uint8       wms_as_id;
} ffw_pal_sms_wms_event_t;

//the new mt event data (corresponding to FFW_PAL_SMS_EVENT_NEW_MT_MSG event)
typedef struct
{
    ffw_pal_sms_storage_type_e      storage_type;
    ffw_int32                                        storage_index;
} ffw_pal_sms_new_mt_msg_event_t;

//the new route message event data (corresponding to E_FIBO_SMS_EVENT_NEW_ROUTE_MSG event)
typedef struct
{
    ffw_pal_sms_msg_format_e     format;
    ffw_pal_sms_msg_ack_flag_e   ack_flag;
    ffw_uint32                             transaction_id;
    ffw_uint32                             pdu_data_len;
    ffw_uint8                               pdu_data[0];
} ffw_pal_sms_route_msg_event_t;

typedef struct 
{
  ffw_pal_sms_ack_type_e ack_type;
  ffw_uint8 first_octet;
  ffw_uint32 transaction_id;
  ffw_uint8 tp_cause_valid;
  ffw_uint8 tp_cause;
  ffw_uint8 pid_valid;
  ffw_uint8 pid;
  ffw_uint8 dcs_valid;
  ffw_uint8 dcs;
  ffw_uint8 usr_data_len_valid;
  ffw_uint16 usr_data_len;
  ffw_uint8 usr_data_valid;
  ffw_uint8 usr_data[FFW_PAL_SMS_USER_DATA_MAX_LEN];
} ffw_pal_sms_pdu_ack_t;  /* Message */

typedef enum
{
    ffw_sms_msg_format_GSM = 0,
    ffw_sms_msg_format_HEX = 1,
    ffw_sms_msg_format_UCS2 = 2,
    ffw_sms_msg_format_IRA = 4,
}ffw_pal_sms_format;

typedef enum
{
    ffw_sms_msg_class_0 = 0, //class 0
    ffw_sms_msg_class_1 = 1, //class 1
    ffw_sms_msg_class_2 = 2, //class 2
    ffw_sms_msg_class_3 = 3, //class 3
    ffw_sms_msg_class_4 = 4, //not support
}ffw_pal_sms_msg_class;

typedef struct
{
    ffw_pal_sms_format format;
    uint8_t msg_class_valid;
    ffw_pal_sms_msg_class msg_class;
    uint32_t length;
}ffw_pal_sms_cfg;

typedef enum
{
    FFW_PAL_SMS_MSG_CFG_INVALID = -1,
    FFW_PAL_SMS_MSG_CFG_STORAGE_TYPE = 0,
} ffw_pal_sms_cfg_e;


//the sms event callback function prototype
ffw_int32 ffw_pal_sms_event_cb(ffw_int32 event, void *msg_data, ffw_int32 msg_data_len, ffw_uint32 index);
typedef ffw_int32 (*ffw_pal_sms_wms_event_cb)(ffw_uint32 wms_event, ffw_uint32 wms_transaction_id, ffw_uint8 wms_as_id);
typedef void (*fibo_sms_rxmsg_handler_func_t)(
    uint32_t    ind_flag,
    void        *ind_msg_buf,
    uint32_t    ind_msg_len
);

/*******************************************************************************
* Function:  ffw_pal_sms_init
* Parameters: None
* Returns:  0 : success;  else: error.
* Description: initialize the pal interface             
*******************************************************************************/
ffw_int32 ffw_pal_sms_init(void);

/*******************************************************************************
* Function:  ffw_pal_sms_deinit
* Parameters: None
* Returns:  0 : success;  else: error.
* Description:  unregister the pal interface   
*******************************************************************************/
ffw_int32 ffw_pal_sms_deinit(void);

/*******************************************************************************
* Function:  ffw_pal_sms_center_address_get
* Parameters: 
        center_address:               [IN/OUT], sms center address
        center_address_type:      [IN/OUT], type of sms center address 
* Returns:  0 : success;  else: error.
* Description:  query the sms center address
*******************************************************************************/
ffw_int32 ffw_pal_sms_center_address_get
(
    ffw_uint8 sim_id,
    char center_address[FFW_PAL_SMS_CENTER_ADDRESS_LEN_MAX], 
    char center_address_type[FFW_PAL_SMS_CENTER_ADDRESS_TYPE_LEN_MAX]
);

/*******************************************************************************
* Function:  ffw_pal_sms_center_address_set
* Parameters: 
        center_address:               [IN], sms center address
        center_address_type:      [IN],  type of sms center address 
* Returns:  0 : success;  else: error.
* Description:  set the sms center address
*******************************************************************************/
ffw_int32 ffw_pal_sms_center_address_set
(
    ffw_uint8 sim_id,
    char center_address[FFW_PAL_SMS_CENTER_ADDRESS_LEN_MAX], 
    char center_address_type[FFW_PAL_SMS_CENTER_ADDRESS_TYPE_LEN_MAX]
);

/*******************************************************************************
* Function:  ffw_pal_sms_receive_msg_storage_set
* Parameters: 
        storage_type:               [IN], sms message storage
* Returns:  0 : success;  else: error.
* Description:  set the storage of the received sms message
*******************************************************************************/
ffw_int32 ffw_pal_sms_receive_msg_storage_set
(
    ffw_pal_sms_storage_type_e storage_type
);

/*******************************************************************************
* Function:  ffw_pal_sms_receive_msg_storage_get
* Parameters: 
        storage_type_ptr:               [OUT], the storage of received sms messages
		sms_store_info_ptr:				[OUT], the storage infomation of received sms storage
* Returns:  0 : success;  else: error.
* Description:  get the storage infomation of the received sms message
*******************************************************************************/
ffw_int32 ffw_pal_sms_receive_msg_storage_get
(
    ffw_pal_sms_storage_type_e * storage_type_ptr,
    ffw_pal_sms_loc_store_info_t * sms_store_info_ptr
);

/*******************************************************************************
* Function:  ffw_pal_sms_msg_send_text
* Parameters: 
        sim_id:             [IN], sim_id
        phone_num:          [IN], dst
        data:               [IN], sms information
        cfg:                [IN], text sms config
* Returns:  0 : success;  else: error.
* Description:  send the sms message
*******************************************************************************/
ffw_int32 ffw_pal_sms_msg_send_text
(
    ffw_uint8 sim_id,
    char *phone_num,
    char *data,
    ffw_pal_sms_cfg *cfg
);
/*******************************************************************************
* Function:  ffw_pal_sms_msg_send_pdu
* Parameters: 
        sim_id:             [IN], sim_id
        pdu:                [IN], sms pdu message to be sent
        pdu_len:            [IN], the length of sms pdu message
        send_flag:          [IN], indicating save or not save the sent message
        msg_id_ptr:         [OUT], sent message id
        storage_index_ptr:  [OUT], the index of storage which the sent message was stored,valid only when send_flag is set to FFW_PAL_SMS_SEND_AND_SAVE
* Returns:  0 : success;  else: error.
* Description:  send the sms message
*******************************************************************************/
ffw_int32 ffw_pal_sms_msg_send_pdu
(
    ffw_uint8 sim_id,
    unsigned char * pdu, 
    ffw_int32 pdu_len, 
    ffw_pal_sms_send_flag_e send_flag,
    ffw_uint32 * msg_id_ptr,
    ffw_int32 * storage_index_ptr
);

/*******************************************************************************
* Function:  ffw_pal_sms_msg_read
* Parameters: 
        storage_type:       [IN], sms message storage to be read sms message from
        index:                   [IN], the location in the storage of the message to be read, 
                                     valid index start from 0,  and -1 means reading all messages
        sms_msg_ptr:      [OUT], the sms message read out from storage
        sms_msg_num:    [IN], the item num of sms_msg_ptr
* Returns:  the number of sms message read from storage
* Description:  read sms message from the given storage
*******************************************************************************/
ffw_int32 ffw_pal_sms_msg_read
(
    ffw_uint8 sim_id,
    ffw_pal_sms_storage_type_e storage_type, 
    ffw_uint32 index, 
    ffw_pal_sms_msg_t  * sms_msg_ptr,
    ffw_int32  sms_msg_num
);
/*******************************************************************************
* Function:  ffw_pal_sms_msg_write
* Parameters: 
        storage_type:       [IN], sms message storage to write sms message to
        pdu_ptr:                   [IN], the sms message 
        pdu_len:                [IN], the length of the sms message
        status_flag:             [IN], message status
        storage_index_ptr:  [OUT], the index of storage which the message was stored
* Returns:  the number of sms message read from storage
* Description:  read sms message from the given storage
*******************************************************************************/
ffw_int32 ffw_pal_sms_msg_write
(
    ffw_pal_sms_storage_type_e storage_type, 
    ffw_uint8 * pdu_ptr, 
    ffw_int32 pdu_len, 
    ffw_pal_sms_msg_status_flag_e status_flag,
    ffw_int32 * storage_index_ptr
);

/*******************************************************************************
* Function:  ffw_pal_sms_msg_delete
* Parameters: 
        storage_type:       [IN], sms message storage to delete sms message
        index:                   [IN], the location in the storage of the message to be deleted, 
                                     valid index start from 0
        delete_flag:          [IN], the delete flag indicate how to delete the sms messages
* Returns:  0 : success;  else: error.
* Description:  delete the sms messages from the given storage
*******************************************************************************/
ffw_int32 ffw_pal_sms_msg_delete
(
    ffw_uint8 sim_id,
    ffw_pal_sms_storage_type_e storage_type,
    ffw_int32 index,
    ffw_pal_sms_delete_flag_e delete_flag
);
/*******************************************************************************
* Function:  ffw_pal_sms_msg_status_modify
* Parameters: 
        storage_type:       [IN], sms message storage to modify the stats of sms message
        index:                   [IN], the location in the storage of the message to be modified, 
                                     valid index start from 0
        modify_flag:          [IN], the status flag indicate how to modify the sms messages
* Returns:  0 : success;  else: error.
* Description:  modify the status of  the sms messages from the given storage
*******************************************************************************/
ffw_int32 ffw_pal_sms_msg_status_modify
(
    ffw_pal_sms_storage_type_e storage_type, 
    ffw_int32 index, 
    ffw_pal_sms_msg_status_flag_e modify_flag
);

/*******************************************************************************
* Function:  ffw_pal_sms_valid_index_get
* Parameters: 
        storage_type:       [IN], sms message storage to list the sms messages
        list_flag:                   [IN], the list flag indicate how to list the sms messages
        sms_msg_info_ptr:   [OUT],  store the information of the listed sms messages
        sms_msg_info_num:    [IN], the item num of sms_msg_info_ptr
* Returns:  the number of the sms messages being lised
* Description:  list the sms messages from the given storage
*******************************************************************************/
ffw_int32 ffw_pal_sms_valid_index_get
(
    ffw_pal_sms_storage_type_e storage_type, 
    ffw_pal_sms_list_flag_e list_flag, 
    ffw_pal_sms_msg_info_t * sms_msg_info_ptr,
    ffw_int32 sms_msg_info_num
);

/*******************************************************************************
* Function:  ffw_pal_sms_storage_info_get
* Parameters: 
        storage_type:              [IN],  the sms message storage
	sms_store_info_ptr:	   [OUT], the storage infomation of the sms message storage
* Returns:  0 : success;  else: error.
* Description:  get the storage infomation of the sms message storage
*******************************************************************************/
ffw_int32 ffw_pal_sms_storage_info_get
(
    ffw_pal_sms_storage_type_e storage_type, 
    ffw_pal_sms_loc_store_info_t * sms_store_info_ptr
);

/*******************************************************************************
* Function:  ffw_pal_sms_service_status_get
* Parameters: 
	service_status_ptr:	   [OUT], the status of the sms module
* Returns:  0 : success;  else: error.
* Description:  get the status of the sms module
*******************************************************************************/
ffw_int32 ffw_pal_sms_service_status_get
(
    ffw_pal_sms_service_status_flag_e * service_status_ptr
);

/*******************************************************************************
* Function:  ffw_pal_sms_msg_service_config_set
* Parameters: 
	mode:	       [IN], cnmi command parameter mode
    msg_code:	   [IN], cnmi command parameter mt
    report_code:   [IN], cnmi command parameter ds
    service_val:   [IN], csms command parameter value
* Returns:  0 : success;  else: error.
* Description:  set the status of the sms module
*******************************************************************************/
ffw_int32 ffw_pal_sms_msg_service_config_set
(
    ffw_int32 mode,
    ffw_int32 msg_code,
    ffw_int32 report_code,
    ffw_int32 service_val
);

/*******************************************************************************
* Function:  ffw_pal_sms_domain_pref_get
* Parameters: 
	domain_pref_ptr:	       [OUT] Service for MO SMS Messages type
* Returns:  0 : success;  else: error.
* Description:  set the status of the sms module
*******************************************************************************/
ffw_int32 ffw_pal_sms_domain_pref_get(ffw_pal_sms_domain_pref_type_e * domain_pref_ptr);

/*******************************************************************************
* Function:  ffw_pal_sms_domain_pref_set
* Parameters: 
	domain_pref:	       [IN] Service for MO SMS Messages type
* Returns:  0 : success;  else: error.
* Description:  set the status of the sms module
*******************************************************************************/
ffw_int32 ffw_pal_sms_domain_pref_set(ffw_pal_sms_domain_pref_type_e domain_pref);

/*******************************************************************************
* Function:  ffw_pal_sms_ack_send
* Parameters: 
	act_flag:	       [IN] message ack type
	transaction_id:	   [IN] qmi req  transaction id
* Returns:  0 : success;  else: error.
* Description:  set the status of the sms module
*******************************************************************************/
ffw_int32 ffw_pal_sms_ack_send(ffw_pal_sms_ack_flag_e act_flag, ffw_uint32 transaction_id);

/*******************************************************************************
* Function:  ffw_pal_sms_event_register
* Parameters: 
    callback:           [IN],  callback function when the sms related event happens
    cb_param:        [IN],  parameter transfer to the callback function when being called
* Returns:  0 : success;  else: error.
* Description:  register the callback function called when sms related event happens
*******************************************************************************/
ffw_int32 ffw_pal_sms_event_register(fibo_sms_rxmsg_handler_func_t callback, void *cb_param);

/*******************************************************************************
* Function:  ffw_pal_sms_pdu_ack_send
* Parameters: 
    sms_pdu_ack_ptr:           [IN],  pdu message context
* Returns:  0 : success;  else: error.
* Description:  send sms pdu ack to network
*******************************************************************************/
ffw_int32 ffw_pal_sms_pdu_ack_send(ffw_pal_sms_pdu_ack_t * sms_pdu_ack_ptr);

/*******************************************************************************
* Function:  ffw_pal_sms_wms_event_register
* Parameters: 
    callback:           [IN],  callback function when the sms related event happens
    cb_param:        [IN],  parameter transfer to the callback function when being called
* Returns:  0 : success;  else: error.
* Description:  register the callback function called when sms related event happens
*******************************************************************************/
ffw_int32 ffw_pal_sms_wms_event_register(ffw_pal_sms_wms_event_cb callback, void *cb_param);

/*******************************************************************************
* Function:  ffw_pal_sms_msg_list_read
* Parameters: 
    resp_info:        [IN],  return the message list info
* Returns:  0 : success;  else: error.
* Description:  list message
*******************************************************************************/
int ffw_pal_sms_msg_list_read(uint8_t sim_id, fibo_sms_msg_list_req_info_t *req_info,fibo_sms_msg_list_info_t *resp_info);

bool ffw_is_sms_cb_register();
void ffw_pal_sms_read_param_set(uint8_t sim_id, int read_mode);
int ffw_pal_sms_read_param_get(uint8_t sim_id);


int ffw_pal_sms_cfg_set(uint8_t simid, ffw_pal_sms_cfg_e sms_cfg, void*cfg_data);
int ffw_pal_sms_cfg_get(uint8_t simid, ffw_pal_sms_cfg_e sms_cfg, void*cfg_data);

#ifdef __cplusplus
}
#endif


#endif

