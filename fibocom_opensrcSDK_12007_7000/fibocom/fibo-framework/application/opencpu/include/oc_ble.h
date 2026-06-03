#ifndef FFW_API_OC_BLE
#define FFW_API_OC_BLE

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    FIBO_BLE_SET_NAME = 0,
    FIBO_BLE_READ_NAME
}fibo_ble_name_opt_e;

typedef struct
{
    uint8_t type;    //0:public addr   1:random addr
    char addr[18];
}fibo_ble_bond_device_s;

typedef struct 
{
    uint8_t type; /*0:gatt char,1:gatt desc*/
    char uuid[64]; 
    uint8_t attr;
    uint16_t handle;
}fibo_ble_gatt_char_t;

typedef struct
{
    char uuid[64];
    uint8_t attr;
    uint16_t handle;
}fibo_ble_gatt_service_t;

typedef INT32 (*ble_data_recv_callback) (int data_len,char *data);

//ble basic and service function
int32_t fibo_ble_power(void);
INT32 fibo_ble_onoff(uint8_t onoff);
INT32 fibo_ble_get_ver(char *ver);
INT32 fibo_ble_read_addr(uint8_t addr_type,char *addr);
INT32 fibo_ble_set_read_name(uint8_t set_read,uint8_t *name,uint8_t name_type);
INT32 fibo_ble_get_connect_state(int *state);
INT32 fibo_ble_set_dev_param(int param_count,int AdvMin,int AdvMax,int AdvType,int OwnAddrType,int DirectAddrType,int AdvChannMap,int AdvFilter,char *DirectAddr);
INT32 fibo_ble_set_dev_data(int data_len,char *data);
INT32 fibo_ble_enable_dev(int onoff);
INT32 fibo_ble_send_data(int data_len,char *data);
INT32 fibo_ble_set_read_mtu(uint8_t set_read, int *mtu);
INT32 fibo_ble_get_peer_mtu(int *mtu);
INT32 fibo_ble_set_scan_response_data(int data_len,char *data);
INT32 fibo_ble_recv_data(ble_data_recv_callback callback);
INT32 fibo_ble_data_request(int data_len,void *data, int timeout);
INT32 fibo_ble_gtsend_data(int index, int len, void* data);
INT32 fibo_ble_set_gtuuid(int index, void *uuid, UINT8 attr);
INT32 fibo_ble_response_to_read(int len, void* data);
INT32 fibo_ble_disconnect(int index);
INT32 fibo_ble_response_to_write(UINT8 result);
INT32 fibo_ble_gatt_service_discover(UINT8 conn_index);
INT32 fibo_ble_gatt_char_discover(UINT8 conn_index, INT32 handle);
INT32 fibo_ble_read_request(UINT8 conn_index, UINT16 handle);
INT32 fibo_ble_write_request(UINT8 conn_index, UINT16 handle, UINT8 *data, UINT16 len, bool need_response);
INT32 fibo_ble_gatt_subscribe(UINT8 conn_index, UINT16 handle, UINT8 type);
INT32 fibo_ble_set_auth(int32_t index, int32_t mode, const char *password);
INT32 fibo_ble_delete_bond_info(const char *mac, int32_t add_type);
INT32 fibo_ble_get_bond_list(fibo_ble_bond_device_s *list, int32_t *count, int32_t max_count);
INT32 fibo_ble_notify_indicate(int conn_index, int uuid_index, int mode, int len, void* data);
INT32 fibo_ble_set_scan_param(uint8_t scan_type, uint8_t filter_policy, uint16_t scan_window, uint16_t scan_interval, uint16_t duration);
INT32 fibo_ble_scan_switch(uint8_t mode);
INT32 fibo_ble_connect(uint8_t add_type, char *mac, int timeout, int min_interval, int max_interval, int latency);
INT32 fibo_ble_sleep_mode(int32_t mode);

#endif
