#ifndef __FFW_CAN_H__
#define __FFW_CAN_H__

typedef enum {
  FFW_CAN_FILTER_ID_EXACT_ADD_TYPE,          ///< Add    exact id filter
  FFW_CAN_FILTER_ID_EXACT_REMOVE_TYPE,       ///< Remove exact id filter
  FFW_CAN_FILTER_ID_MASKABLE_ADD_TYPE,       ///< Add    maskable id filter
  FFW_CAN_FILTER_ID_MASKABLE_REMOVE_TYPE     ///< Remove maskable id filter
} ffw_can_filter_e;

typedef enum {
/*< Initialization mode. Used to setup communication parameters for the reception objects and global filtering, while peripheral is not active on the bus. */
  FFW_CAN_MODE_INITIALIZATION_TYPE,
/*< Normal operation mode. Used when peripheral is in active mode to receive, transmit, and acknowledge messages on the bus. Depending on the current unit state, it can generate error or overload messages.*/
  FFW_CAN_MODE_NORMAL_TYPE,
} ffw_can_mode_e;


typedef struct {
    uint32_t id; //can device id
    uint8_t data[64]; //FD mode 64 byte, standard mode 8byte
    uint8_t dlc; // data length
    uint8_t rtr; //Remote transmission request frame 1:remote frame
    uint8_t bis_ext_or_stand; //bIsExtOrStand 1:ARM_CAN_EXTENDED_ID 0:ARM_CAN_STANDARD_ID
    uint8_t Reserved;
} ffw_can_frame_t;
/*
 * @brief can 设备配置定义
 *
 */
typedef struct
{
    uint8_t can_port;
    ffw_can_mode_e  can_mode;
    ffw_can_filter_e filter_operation;
    uint8_t bit_rate_mode;
    uint32_t can_id;  //filter_can_id
    uint32_t mask_id; //filter_mask_id
    uint32_t bit_rate;

    uint8_t bis_ext_or_stand; //bis_ext_or_stand 1:ARM_CAN_EXTENDED_ID 0:ARM_CAN_STANDARD_ID
    uint8_t set_bit_segments; //0:use default bit_segments 1:use cust set bit_segments value

    //when set_bit_segments = 1 use following parameters
    uint8_t bit_prop_seg;   // Propagation Segment codes
    uint8_t bit_phase_seg1; //Phase Buffer Segment 1
    uint8_t bit_phase_seg2; //Phase Buffer Segment 2
    uint8_t bit_phase_sjw;  //(Re)Synchronization Jump Width Segment
    void *Reserved;
/*
    when filter_operation = ARM_CAN_FILTER_ID_MASKABLE_ADD, filter_mask_id
    0, the corresponding ID bit will be accepted, regardlesss of the value
    1, the corresponding ID bit will be compared with the value of the ID filter bit
*/
}ffw_can_config_t;
/**
* @brief Callback for recv can remote device data
*
 */
typedef void (*ffw_can_recv_callback_t)(ffw_can_frame_t *pFrame);

int ffw_can_init(uint8_t can_port, ffw_can_config_t *can_config, ffw_can_recv_callback_t recv_cb, void *arg);

int ffw_can_deinit(uint8_t can_port);

int ffw_can_send_data(uint8_t can_port, ffw_can_frame_t *pFrame);

#endif


