#ifndef __OC_CAN_H__
#define __OC_CAN_H__

//#include "ffw_can.h"
#include "fibo_opencpu_comm.h"

typedef enum {
  FIBO_CAN_FILTER_ID_EXACT_ADD_TYPE,          ///< Add    exact id filter
  FIBO_CAN_FILTER_ID_EXACT_REMOVE_TYPE,       ///< Remove exact id filter
  FIBO_CAN_FILTER_ID_MASKABLE_ADD_TYPE,       ///< Add    maskable id filter
  FIBO_CAN_FILTER_ID_MASKABLE_REMOVE_TYPE     ///< Remove maskable id filter
} f_can_filter_e;

typedef enum {
/*< Initialization mode. Used to setup communication parameters for the reception objects and global filtering, while peripheral is not active on the bus. */
  FIBO_CAN_MODE_INITIALIZATION_TYPE,         
/*< Normal operation mode. Used when peripheral is in active mode to receive, transmit, and acknowledge messages on the bus. Depending on the current unit state, it can generate error or overload messages.*/
  FIBO_CAN_MODE_NORMAL_TYPE,    
} f_can_mode_e;

/*
 * @brief can 设备配置定义
 *
 */
typedef struct
{
    uint8 can_port; // can num，default 0
    f_can_mode_e  can_mode; //default 1
    f_can_filter_e filter_operation; // can recv filter type
    uint8 bit_rate_mode; // 0: Nominal bit rate   1：Flexible data rate（CAN FD not supported）
    uint32 can_id;  //filter_can_id
    uint32 mask_id; //filter_mask_id
    uint32 bit_rate;  // CAN baud rate

    uint8 bis_ext_or_stand; //filter_can_id type( 1:ARM_CAN_EXTENDED_ID, 0:ARM_CAN_STANDARD_ID )
    uint8 set_bit_segments; //0:use default bit_segments 1:use cust set bit_segments value

    //when set_bit_segments = 1 use following parameters
    uint8 bit_prop_seg;     // Propagation Segment codes
    uint8 bit_phase_seg1;   //Phase Buffer Segment 1
    uint8 bit_phase_seg2;   //Phase Buffer Segment 2
    uint8 bit_phase_sjw;    //(Re)Synchronization Jump Width Segment
    void *Reserved;
}f_can_config_t;


typedef struct {
    uint32 id; //can id
    uint8 data[64]; //FD mode 64 byte, standard mode 8byte
    uint8 dlc; // data length
    uint8 rtr; //Remote transmission request frame 1:remote frame
    uint8 bis_ext_or_stand; //bis_ext_or_stand 1:ARM_CAN_EXTENDED_ID 0:ARM_CAN_STANDARD_ID
    uint8 Reserved;
} f_can_frame_t;

/**
* @brief Callback for recv can remote device data
*
 */
typedef void (*can_recv_callback_t)(f_can_frame_t *pFrame);

int32 fibo_can_init(uint8 can_port, f_can_config_t *can_config, can_recv_callback_t recv_cb, void *arg);

int32 fibo_can_deinit(uint8 can_port);

int32 fibo_can_send(uint8 can_port, f_can_frame_t *pFrame);


#endif

