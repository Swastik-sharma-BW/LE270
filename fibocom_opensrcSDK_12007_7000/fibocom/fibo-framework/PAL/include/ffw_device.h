#ifndef __FFW_DEVICE_H__
#define __FFW_DEVICE_H__

/**
* @file ffw_adc.h
* @brief Oprate chip peripherals, e.g. get voltage\temperature, poweroff 
* @author Lucien
* @version 0.1
* @date 2023-03-23
 */


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef int hal_adc_channel_t;

typedef enum {
    DL_AUTODLOADER=0,
    DL_RECOVERY,
    DL_FASTBOOT
}e_dl_mode;

typedef int (*Report_UsbDev_Event) (int msg);
extern Report_UsbDev_Event p_UsbReportHook;

/**
 * @brief get the raw data of the specified adc channel
 * 
 * @param cha_id: adc channel, refs schematic for detail
 * @param data: pointer of adc raw data
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_adc_get_data(uint16_t cha_id, uint32_t *data);


/**
 * @brief get the voltage of the specified adc channel
 * 
 * @param cha_id: adc channel 
 * @param data: pointer of voltage  unit: mV
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_adc_get_voltage(uint16_t cha_id, uint32_t *data);

/**
 * @brief Get the number of ADC channels
 * 
 * @param void
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_adc_get_chans(void);

/**
 * @brief get the voltage of Vbat
 * 
 * @param data: pointer of voltage  unit: mV
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_get_vbat( uint32_t *data);

/**
 * @brief get the cpu temperature
 * 
 * @param data: pointer of temperature  unit: 'C
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_get_cpu_temperature( int32_t *data);


/**
 * @brief get the pa temperature
 * 
 * @param data: pointer of temperature  unit: 'C
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_get_pa_temperature( int32_t *data);

/**
 * @brief poweroff device
 * 
 * @param NONE
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_poweroff_ms(void);

#ifdef CONFIG_FFW_EC200_AT_ENABLE
int ffw_qpowd_off_ms(void *engine);
#endif

int ffw_set_usbmode(int usbmode);
int ffw_get_usbmode(void);
int ffw_usbswitch(uint8_t en);
int ffw_dlmode_cfg(void *engine,e_dl_mode dl_mode);
int ffw_get_usbisinsert(void);
int ffw_usbdevice_init(char      *name, void (*usb_input_callback)());
int ffw_usbdevice_deinit(int usb_channel);
int ffw_usbdevice_send(int usb_channel, const void *data, size_t length);

typedef struct {
    uint8_t usb_channel;
    char usb_name[7];
    void (*ffw_usb_input_cb)();
    void *lock;
    uint8_t usb_true_channel;
}ffw_usb_data_channel_t;

bool ffw_device_get_32K_usp2_mclk_en();
void ffw_device_enable_32K_by_usp2_mclk(bool enable);
void ffw_device_enable_32K_by_usp1_mclk(bool enable);

/**
 * @brief log port set
 *
 * @param NONE
 * @return int
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_log_port_set(int type);
bool ffw_get_log_status(uint8_t *Apsetport, uint8_t *Apport, uint8_t *Apenable, uint8_t *Cpport, uint8_t *Cpenable);

/**
 * @brief update_mac
 *
 * @param NONE
 * @return NONE
 * @retval
 * @retval
 */
void ffw_pal_device_update_mac();

/**
 * @brief get mac
 *
 * @param NONE
 * @return char *
 * @retval
 * @retval
 */
char *ffw_pal_device_get_mac();

int ffw_usbdevice_state_cb(int (*usedev_Event) (int msg));

#endif /* __ADC__H__ */
