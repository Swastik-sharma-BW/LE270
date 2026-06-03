
#ifndef _FIBO_OPENCPU_H_
#define _FIBO_OPENCPU_H_

#include <stdbool.h>
#include "fibo_opencpu_comm.h"
#include "ffw_plat_header.h"

#ifdef CONFIG_FFW_OPENCPU_ENABLE
#include "fibo_opencpu_api.h"
#endif

//#include "cfw.h"
#include "stdio.h"
//#include "osi_api.h"
//#include "osi_log.h"
//#include "cfw.h"
//#include "at_cfg.h"
//#include "at_cfw.h"
//#include "cfw_dispatch.h"
//#include "image_sensor.h"
#include "oc_adc.h"
//#include "drv_lcd.h"
//#include "fibo_common.h"
//#include "audio_types.h"
#include "oc_gprs.h"
#include "oc_lbs.h"
#include "oc_osi.h"
//#include "drv_spi.h"
#ifdef CONFIG_FIBOCOM_XIAONIU_FEATURE
#include "fibo_opencpu_cus.h"
#endif
#if 1//def CONFIG_FIBOCOM_OPENCPU
#include "oc_mqtt.h"
#include "oc_socket.h"
#include "oc_heartbeat.h"
#include "oc_mqtt_cloud.h"
#include "oc_http.h"
#include "oc_ftp.h"
#include "oc_ssl.h"
//#include "oc_charge.h"
//#include "oc_sdc.h"
#include "oc_ffs.h"
#include "oc_gb2312_unicode.h"
//#include "oc_mbedtls.h"
#include "oc_other.h"
#include "oc_cell.h"
#include "oc_ws.h"
#endif
#include "oc_uart.h"
//#include "expat.h"
#include "oc_gpio.h"
#ifdef CONFIG_FFW_FEATURE_ONE_WIRE
#include "oc_one_wire.h"
#endif
#include "oc_usb.h"
#include "oc_innerflash.h"
#include "oc_keypad.h"
//#include "oc_call.h"

#include "oc_i2c.h"
#include "oc_spi.h"
//#include "ffw_result.h"
#ifdef CONFIG_FFW_TTS_SUPPORT
#include "oc_tts.h"
#endif

//#include "drv_wifi.h"
#ifdef CONFIG_FFW_AUDIO_ENABLE
#include "oc_audio.h"
#endif

#ifdef CONFIG_FFW_LCD_ENABLE
#include "ffw_lcd.h"
#include "oc_lcd.h"
#endif
#include "ffw_key.h"

#include "oc_gnss.h"

#include "oc_sms.h"
#ifdef CONFIG_FFW_FEATURE_CAN
#include "oc_can.h"
#endif

#ifdef CONFIG_CUST_FUTE_FEATURE
#include "cJSON.h"
#endif

#ifdef CONFIG_CUST_NBSANXING_FEATURE
#include "sx_protocol_api.h"
#endif

#ifdef write
#undef write
#endif

#ifdef read
#undef read
#endif

#ifdef close
#undef close
#endif

/* FOTA ERROR CODE */
#define OTA_ERRCODE_MEMLEAK            (-1)
#define OTA_ERRCODE_FSREAD             (-2)
#define OTA_ERRCODE_FSWRITE            (-3)
#define OTA_ERRCODE_APP_CHECKFAIL      (-4)
#define OTA_ERRCODE_FM_CHECKFAIL       (-5)
#define OTA_ERRCODE_APPFOTA_CHECKFAIL  (-6)
#define OTA_ERRCODE_DL_FAIL            (-7)
#define OTA_ERRCODE_BLE_CHECKFAIL      (-8)
#define OTA_ERRCODE_BLE_FS_FULL        (-9)
#define OTA_ERRCODE_SPACE_NOT_ENOUGH   (-10)


/*ERROR CODE*/
#define GAPP_RET_OK (0)
#define GAPP_RET_PARAMS_ERROR (-1)
#define GAPP_RET_NOT_INIT (-2)
#define GAPP_RET_MEMORY_ERROR (-3)
#define GAPP_RET_OPTION_NOT_SUPPORT (-4)
#define GAPP_RET_TIMEOUT (-5)
#define GAPP_RET_UNKNOW_ERROR (-6)
#define GAPP_RET_FILE_ALREADY_OPEN_ERROR (-7)
#define GAPP_RET_NOT_ENOUGH_SPACE_ERROR (-8)
#define GAPP_RET_NOT_VALID_STATE_ERROR (-9)


#define FS_O_RDONLY  O_RDONLY
#define FS_O_WRONLY  O_WRONLY
#define FS_O_RDWR    O_RDWR
#define FS_O_APPEND  O_APPEND
#define FS_O_CREAT   O_CREAT
#define FS_O_TRUNC   O_TRUNC
#define FS_O_EXCL    O_EXCL
#define FS_O_SYNC    O_SYNC

#define FS_SEEK_SET  SEEK_SET
#define FS_SEEK_CUR  SEEK_CUR
#define FS_SEEK_END  SEEK_END

// TTS
#ifndef CTTS_STRING_ENCODE_TYPE_UTF8
#define CTTS_STRING_ENCODE_TYPE_UTF8 (0)
#endif
#ifndef CTTS_STRING_ENCODE_TYPE_GB2312
#define CTTS_STRING_ENCODE_TYPE_GB2312 (1)
#endif
#ifndef CTTS_STRING_ENCODE_TYPE_UNICODE
#define CTTS_STRING_ENCODE_TYPE_UNICODE (2)
#endif
#ifndef CTTS_TEXT_MAX_LEN
#define CTTS_TEXT_MAX_LEN (1024)
#endif
#define TTS_SPEED_MIN -32768 /* slowest voice speed */
#define TTS_SPEED_NORMAL 0   /* normal voice speed (default) */
#define TTS_SPEED_MAX +32767 /* fastest voice speed */

#define TTS_PITCH_MIN -32768 /* lowest voice tone */
#define TTS_PITCH_NORMAL 0   /* normal voice tone (default) */
#define TTS_PITCH_MAX +32767 /* highest voice tone */

typedef enum {
    GAPP_TTS_SUCCESS = 0,
    GAPP_TTS_ERROR = 1,
    GAPP_TTS_PARAMS_ERROR = -1,
    GAPP_TTS_BUSY = -2,
    GAPP_TTS_MEMORY_ERROR = -3
}tts_status_e;


#define MAX_CODE_TYPE_LEN (8)
#define MAX_CODE_TYPE     (12)


/*STRUCT*/
typedef struct {
    char codeType[MAX_CODE_TYPE_LEN];
    uint8_t codeValue;
}barCodeEx_t;

typedef enum
{
    AEE_TONE_RING_G5 = 1568,/*1568.0Hz*/
    AEE_TONE_RING_A5 = 880,/*880.0Hz*/
    AEE_TONE_RING_E5 = 1318,/*1318.5Hz*/
} AUDIO_TONE_EX_T;

#if 0
typedef struct{
    uint8_t curr_rat;
    cfw_nwCapLte_t lte_cellinfo;
}opencpu_cellinfo_t;
#endif
typedef struct Open_SCellInfo_T
{
    UINT16          mcc;
    UINT16          mnc;

    //DL earfcn (anchor earfcn), range 0 - 262143
    UINT32          earfcn;
    //the 28 bits Cell-Identity in SIB1, range 0 - 268435455
    UINT32          cellId;

    UINT16          tac;
    //Physical Cell ID, range 0 - 503
    UINT16          phyCellId;

    //value in dB, value range: -30 ~ 30(NB) or -20 ~ 40(CAT1bis)
    INT8            snr;

    //value in units of dBm, value range: -156 ~ -44
    INT16           rsrp;
    //value in units of dB, value range: -34 ~ 25
    INT16           rsrq;

    //Srxlev, refer to TS36.304 section 5.2.3.2
    INT16           srxlev;
    //FDD or TDD
    BOOL            isTdd;
    //The current selected band accoring to BandIndicator or mFBI, value range: 0 ~ 85
    UINT8           band;

    INT16          rssi;
    //BandWidth
    UINT8           ulBandWidth;
    //BandWidth
    UINT8           dlBandWidth;

    UINT16          mnclen;
    //RAT
    UINT8           rat;
}Open_SCellInfo;
typedef struct Open_NCellInfo_T
{
    //DL earfcn (anchor earfcn), range 0 - 262143
    UINT32          earfcn;

    //Physical Cell ID, range 0 - 503
    UINT16          phyCellId;
    //Srxlev, refer to TS36.304 section 5.2.3.2
    INT16           srxlev;
    //value in units of dBm, value range: -156 ~ -44
    INT16           rsrp;
    //value in units of dB, value range: -34 ~ 25
    INT16           rsrq;

    //BandWidth
    UINT8           dlBandWidth;

    //value in dB, value range: -30 ~ 30(NB) or -20 ~ 40(CAT1bis)
    INT8            snr;
    UINT16          tac;
    UINT16          mcc;
    UINT16          mnc;
    //the 28 bits Cell-Identity in SIB1, range 0 - 268435455
    UINT32          cellId;

    UINT16          mnclen;
    //whether plmn/tac/cellId valid/present
    BOOL            cellValid;
}Open_NCellInfo;  //24 bytes

#define OPENCPU_NCELLINFO_SIZE   6
typedef struct opencpu_cellinfo_T
{
    Open_SCellInfo    sCellInfo;  //24 bytes
    Open_NCellInfo    nCellList[OPENCPU_NCELLINFO_SIZE]; //24*6 bytes
    INT8              nCellNum;
}opencpu_cellinfo;

typedef void (*fota_download_cb_t)(UINT32 size, UINT32 total);


typedef struct {
    uint8_t reg;
    uint8_t value;
}camera_reg_t;

//Lcd
#if 0
typedef struct {
    uint8_t command;
    uint8_t length;
    uint8_t *CommandValue;
}lcd_reg_t;
#endif
#if 0
//sweep
typedef struct
{
    CAM_DEV_T CamDev;
    lcdSpec_t lcddev;
    osiThread_t *camTask;
    uint8_t Decodestat;
    bool Openstat;
    bool MemoryState;
    bool gCamPowerOnFlag;
    bool issnapshot;
    uint32_t height;
    uint32_t width;
    uint32_t sweepsize;
    uint32_t times;
} camastae_t;
#endif

typedef INT32 (*ble_data_recv_callback) (int data_len,char *data);


typedef struct
{
int rsrp; //参考信号接收功率， 取值范围 0-255
int rsrq; //参考信号接收质量， 取值范围 0-255
int snr; //信号与干扰加噪声比
int ecl; //信号覆盖等级,0 - 常规覆盖 1 - 扩展覆盖 2 - 极端覆盖
}T_CellInfo;

typedef enum{
Mode_NoSet = 0, //不使用省电模式
Mode_PSM = 1, //PSM 模式
Mode_DRX, //DRX 模式
Mode_eDRX, //eDRX 模式
}T_PowerMode;

typedef struct
{
long totalFLash; //FLASH 总大小
long totalRam; //RAM 总大小
long totalFree; //当前剩余内存
}T_MemInfo;




typedef struct{
    int spl_address;
    int spl_size;
    int boot_address;
    int boot_size;
    int flash_address;
    int flash_size;
    int appimg_flash_address;
    int appimg_flash_size;
    int fs_sys_address;
    int fs_sys_address_size;
    int fs_modem_address;
    int fs_modem_size;
    int fs_factory_address;
    int fs_factory_size;
}fibo_flash_partinfo_t;

typedef struct{
    int gSysnvUsbWorkMode;
}fibo_config_t;

/*support network config name*/
#define FIBO_NW_ROAMING_STATUS          "ROAMING_STATUS"
//Add signal quality urc of opencpu for MTC0849-69 begin 20250407 
#define FIBO_NW_LTE_SIGNAL_QUALITY_URC      "LTE_SIGNAL_QUALITY_URC"
#define FIBO_NW_CELL_INFO_CFG      "CELL_INFO_CFG"
//Add  the DRX cycle for MTC0991-1550
#define FIBO_NW_DRX_CYCLE_CFG       "DRX_CYCLE_CFG"
typedef enum
{
    SEND_CMD_MIN_NUM,
    FIBO_CMD_REFRESH_TABLE_B = 1,
    FIBO_CMD_GET_TABLE_B_CONTENT,
    FIBO_CMD_GET_MCC_MNC,
    FIBO_CMD_REFRESH_TABLE_C,
    FIBO_CMD_GET_OPERATOR_INFO = 5,
    FIBO_CMD_SET_SIGNAL_QUALITY_URC,
    SEND_CMD_MAX_NUM
} SEND_CMD_E;

typedef enum
{
    FIBO_SIGNAL_QUALITY_URC_INIT    = 0x00,   //dont urc
    FIBO_SIGNAL_QUALITY_URC_CONFIG1 = 0x01,   //only use rsrp_range urc
    FIBO_SIGNAL_QUALITY_URC_CONFIG2 = 0x02,   //only use sinr_range urc
    FIBO_SIGNAL_QUALITY_URC_CONFIG3 = 0x03,   //both use rsrp_range and sinr_range urc
    FIBO_SIGNAL_QUALITY_URC_DEFAULT = 0xFF,   //use default urc
}signal_quality_urc_config_type_t;

typedef enum
{
    FIBO_NITZ_NO_SYNC      = 0x00,   //ctzu = 0;
    FIBO_NITZ_SYNC_SUCCESS = 0x01,   //sync sucess.
    FIBO_NITZ_SYNC_FAIL    = 0x02,   //sync failed.
    FIBO_NITZ_MAX_NUM
}FIBO_NITZ_SYNC_MODE_E;

typedef struct nw_config_signal_quality_urc_s
{
    signal_quality_urc_config_type_t config;
    UINT8 rsrp_range;
    UINT8 sinr_range;
    UINT16 reserve;
} fibo_nw_config_signal_quality_urc_t;

//Add signal quality urc of opencpu for MTC0849-69 end c 20250407 


typedef void(*fibo_app_handle_at_cb)(UINT8*, UINT16);
typedef int (*VSIM_COMMAND)(uint8_t *apdu_req, uint16_t apdu_req_len, uint8_t *apdu_rsp, uint16_t *apdu_rsp_len, uint8_t slot);
typedef void(*fibo_tts_set_read_extflash_cb)(void*pParameter, uint8_t* pData, uint32_t ReadAddr, uint32_t Size);

/*FUNCTION*/

//app ota
extern INT32 fibo_app_ota(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_app_handle(INT8 *data, UINT32 len);
extern INT32 fibo_appfota_handle_no_reboot(INT8 *data, UINT32 len);
extern INT32 fibo_appfota_ota(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_appfota_handle(INT8 *data, UINT32 len);
extern INT32 fibo_ota_handle_no_reboot(INT8 *data, UINT32 len);
extern INT32 fibo_ota_handle(INT8 *data, UINT32 len);
extern INT32 fibo_open_ota(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_app_check(INT8 *data, UINT32 len);
extern INT32 fibo_app_dl(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_app_update(void);
extern INT32 fibo_ota_errcode(void);
extern void fibo_ota_set_callback(fota_download_cb_t cb);

//firmware ota
extern INT32 fibo_firmware_handle(INT8 *data, UINT32 len);
extern INT32 fibo_firmware_ota(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_firmware_handle_no_reboot(INT8 *data, UINT32 len);
extern INT32 fibo_firmware_dl(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_firmware_update(void);
extern INT32 fibo_firmware_write(INT8 *data, UINT32 len);
extern INT32 fibo_firmware_verify(UINT32 mode, INT8 *data, UINT32 len);
extern INT32 fibo_firmware_del();

//appfota+firmware ota
extern INT32 fibo_appfw_ota(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_appfw_handle(INT8 *data, UINT32 len);
extern INT32 fibo_appfw_handle_no_reboot(INT8 *data, UINT32 len);

// virtual at channel
extern INT32 fibo_at_send(const UINT8 *cmd, UINT16 len);

//extern UINT32  fibo_atCmd_Register(AtCmdPreDefInfoC* list, UINT32 num);

//i2c
extern INT32 fibo_i2c_open(HAL_I2C_BPS_T bps,i2c_Handle *i2c_handle);
extern INT32 fibo_i2c_close(i2c_Handle *i2c_handle);
extern INT32 fibo_i2c_Write(i2c_Handle i2c_handle, drvI2cSlave_t idAddress, UINT8 *Buffer, UINT8 bufferlen);
extern INT32 fibo_i2c_Read(i2c_Handle i2c_handle, drvI2cSlave_t idAddress, UINT8 *Buffer, UINT8 bufferlen);
extern INT32 fibo_i2c_xfer(i2c_Handle i2c_handle,drvI2cSlave_t idAddress, UINT8 *send_Buffer, UINT8 send_bufferlen, UINT8 *recv_Buffer, UINT8 recv_bufferlen);

//queue
extern UINT32 fibo_queue_create(UINT32 msg_count, UINT32 msg_size);
extern INT32 fibo_queue_put(UINT32 msg_id, const void *msg, UINT32 timeout);
extern INT32 fibo_queue_put_to_front(UINT32 msg_id, const void *msg, UINT32 timeout);
extern INT32 fibo_queue_get(UINT32 msg_id, void *msg, UINT32 timeout);
extern INT32 fibo_queue_put_isr(UINT32 msg_id, const void *msg);
extern INT32 fibo_queue_put_to_front_isr(UINT32 msg_id, const void *msg);
extern INT32 fibo_queue_get_isr(UINT32 msg_id, void *msg);
extern void fibo_queue_delete(UINT32 msg_id);
extern UINT32 fibo_queue_space_available(UINT32 msg_id);
extern void fibo_queue_reset(UINT32 msg_id);

//mutex
extern UINT32 fibo_mutex_create(void);
extern void fibo_mutex_lock(UINT32 mutex_id);
extern INT32 fibo_mutex_try_lock(UINT32 mutex_id, UINT32 timeout);
extern void fibo_mutex_unlock(UINT32 mutex_id);
extern void fibo_mutex_delete(UINT32 mutex_id);
extern INT32 fibo_rng_generate(void *buf, UINT32 len);

//#ifdef CONFIG_CUST_XIAONIU_FEATURE
extern UINT32 fibo_timer_new(UINT32 ms, void (*fn)(void *arg), void *arg);
extern UINT32 fibo_timer_period_new(UINT32 ms, void (*fn)(void *arg), void *arg);
extern INT32 fibo_timer_free(UINT32 timerid);
extern INT32 fibo_softReset(void);
extern bool fibo_TY_wakeup_init(UINT8 pin,UINT8 level);
extern bool fibo_TY_wakeup_enable(void);
extern int fibo_get_4g_temperature(void);
//#endif

extern INT32 fibo_heapinfo_get(uint32_t * size,uint32_t * avail_size,uint32_t *max_block_size);

//tts
extern bool fibo_tts_is_playing(void);
extern INT32 fibo_tts_start(const UINT8 *pData, INT8 cEncode);
extern INT32 fibo_tts_stop(void);
extern INT32 fibo_tts_voice_speed(INT32 speed);
extern INT32 fibo_tts_voice_pitch(INT32 pitch);
extern INT32 fibo_tts_cfg(INT32 key, INT32 mode, INT32 *value);
extern INT32 fibo_tts_set_respath(const char *path);
extern void fibo_tts_set_read_extflash(void*pParameter, fibo_tts_set_read_extflash_cb callback);

//ecm
extern INT32 fibo_get_ip_mask_gw_of_ecm(char *ip_addr, char *netmask, char *gateway);


extern INT32 fibo_ext_flash_init(UINT8 Pinsel);
extern INT32 fibo_ext_flash_info(UINT32 * pulId, UINT32 * pulCapacity);

//thread
extern void fibo_taskSleep(UINT32 msec);
extern INT32 fibo_thread_create(void *pvTaskCode, INT8 *pcName, UINT32 usStackDepth, void *pvParameters, UINT32 uxPriority);
extern INT32 fibo_thread_create_ex(void *pvTaskCode, char *pcName, UINT32 usStackDepth, void *pvParameters, UINT32 uxPriority, UINT32* pThreadId);
extern void fibo_thread_delete(void);
extern UINT64 fibo_get_sys_tick(void);
extern void fibo_specify_thread_delete(UINT32 uThread);
extern UINT32 fibo_get_thread_stack_remaining_size(UINT32 ThreadId);
extern UINT32 fibo_thread_id(void);

//memory mallc and free
extern void *fibo_malloc(UINT32 size);
extern INT32 fibo_free(void *buffer);

extern void *fibo_malloc_v1(UINT32 size);
extern INT32 fibo_free_v1(void *buffer);
extern INT32 fibo_heapinfo_get_v1(uint32_t * size,uint32_t *avail_size,uint32_t *max_block_size);

//soft reset and power off
extern INT32 fibo_soft_reset(void);
extern INT32 fibo_soft_power_off(void);

//sem
extern UINT32 fibo_sem_new(UINT8 inivalue);
extern void fibo_sem_free(UINT32 semid);
extern void fibo_sem_wait(UINT32 semid);
extern bool fibo_sem_try_wait(UINT32 semid, UINT32 timeout);
extern void fibo_sem_signal(UINT32 semid);

extern INT32 fibo_hal_adc_init(void);
extern INT32 fibo_hal_adc_deinit(void);
extern INT32 fibo_hal_adc_get_data(hal_adc_channel_t channel, UINT32 scale_value);
extern INT32 fibo_hal_adc_get_data_polling(hal_adc_channel_t channel, UINT32 *data);

#ifdef CONFIG_FFW_LCD_ENABLE
extern INT32 fibo_lcd_init(void);
extern INT32 fibo_lcd_deinit(void);
extern INT32 fibo_lcd_Sleep(BOOL mode);
extern INT32 fibo_lcd_FrameTransfer(const lcdFrameBuffer_t * pstFrame, const lcdDisplay_t * pstWindow);
extern INT32 fibo_lcd_SetOverLay(const lcdFrameBuffer_t * pstFrame);
extern INT32 fibo_lcd_SetBrushDirection(lcdDirect_t direct_type);
extern INT32 fibo_lcd_SetPixel(UINT16 ulx, UINT16 uly, UINT16 ulcolor);
extern INT32 fibo_lcd_FillRect(const lcdDisplay_t * pstWindow, UINT16 ulBgcolor);
extern INT32 fibo_lcd_DrawLine(UINT16 ulSx, UINT16 ulSy, UINT16 ulEx, UINT16 ulEy, UINT16 ulColor);
extern INT32 fibo_lcd_Getinfo(uint32_t * puldevid, uint32_t * pulwidth, uint32_t * pulheight);
extern INT32 fibo_lcd_Setvideosize(uint32_t width, uint32_t height, uint32_t imageBufWidth);
//extern INT32 fibo_lcd_rstPin_setLevel(SAPP_IO_ID_T id,UINT8 level);
//extern INT32 fibo_lcd_spi_config(uint8_t spilinetype, uint32_t spiclk);
//extern INT32 fibo_lcd_spi_set_freq(uint32_t spiclk);
//extern INT32 fibo_lcd_spi_set_mode(uint8_t spilinetype);
//extern INT32 fibo_lcd_spi_write_cmd(uint16_t cmd);
//extern INT32 fibo_lcd_spi_write_data(uint16_t data);
//extern INT32 fibo_lcd_spi_read_data(uint16_t addr, uint8_t * pData, uint32_t len);
#endif

//extern bool fibo_sweep_code(camastae_t *cam, uint16_t *pFrame, uint8_t *pDatabuf, uint8_t *pResult, uint32_t *pReslen, int *type);

//local time
extern INT32 fibo_set_alarm(const char *alarmTime,uint8_t index,const char *alarmRecurr, bool poweroff);
extern INT32 fibo_del_alarm(uint8_t index);

//sleep
#ifdef CONFIG_FFW_LPM_SUPPORT
extern int32_t fibo_setSleepMode(uint8_t time);
#endif

//gpio
extern INT32 fibo_gpio_mode_set(SAPP_IO_ID_T id, UINT8  mode);
extern INT32 fibo_gpio_cfg(SAPP_IO_ID_T id, SAPP_GPIO_CFG_T cfg);
extern INT32 fibo_gpio_set(SAPP_IO_ID_T id, UINT8 level);
extern INT32 fibo_gpio_get(SAPP_IO_ID_T id, UINT8 *level);
extern INT32 fibo_gpio_pull_disable(SAPP_IO_ID_T id);
extern INT32 fibo_gpio_isr_init(SAPP_IO_ID_T id, oc_isr_t* isr_cfg);
extern INT32 fibo_gpio_isr_deinit(SAPP_IO_ID_T id);
extern INT32 fibo_gpio_pull_up_or_down(SAPP_IO_ID_T id, bool is_pull_up);
extern INT32 fibo_gpio_pull_high_resistance(SAPP_IO_ID_T id, bool is_pull_high);
extern INT32 fibo_gpio_driving_set(SAPP_IO_ID_T id,uint8_t driving);

extern bool fibo_heartbeat_param_set(char *serverip, uint16_t port,  uint16_t time, char *hexdata);
extern bool fibo_enable_heartbeat(uint8_t cid,  uint8_t nsim, int32_t mode, int32_t protocol);
extern bool fibo_set_heartbeat_reconnect_timer(uint16_t time1, uint16_t time2,uint16_t time3,uint16_t time4,uint16_t time5,uint16_t time6);

//MTSM
extern int fibo_get_chip_temperature_by_adc(void);

//dualsim
extern int fibo_set_dualsim(uint8_t simid);
extern uint8_t fibo_get_dualsim();
extern int fibo_set_datasim(uint8_t simid);

//keypad
extern bool fibo_keypad_queue_init(UINT32 timeout);
extern bool fibo_keypad_queue_output(keypad_info_t * key);
extern UINT8 fibo_get_pwrkeystatus(void);
extern bool fibo_keypad_msg_mod(bool en);
extern bool fibo_pwrkeypad_config(UINT32 timeout, bool Ispwroff);
extern keypad_info_t fibo_get_pwrkeypad_status(void);


//Shut down
extern INT32 fibo_set_pwr_callback_ex(key_callback pwr_cb,UINT16 long_press,void *arg);
#if 0
extern INT32 fibo_set_pwr_callback(key_callback pwr_cb,UINT16 long_press);
extern INT32 fibo_set_pwr_callback_v1(key_callback pwr_press_cb,key_callback pwr_release_cb,UINT16 long_press);
extern INT32 fibo_set_shutdown_mode(osiShutdownMode_t mode);
#endif

//uart
extern INT32 fibo_hal_uart_init(hal_uart_port_t uart_port, hal_uart_config_t *uart_config, uart_input_callback_t recv_cb, void *arg);
extern INT32 fibo_hal_uart_deinit(hal_uart_port_t uart_port);
extern INT32 fibo_hal_uart_put(hal_uart_port_t uart_port, UINT8 *buff, UINT32 len);
extern INT32 fibo_hal_uart_data_to_send(int uart_port);

#if 0
//uart-to-485
extern INT32 fibo_rs485_init(hal_uart_port_t uart_port, hal_uart_config_t * uart_config, uint16_t ctrl_pin, uart_input_callback_t recv_cb, void * arg);
extern INT32 fibo_rs485_deinit(hal_uart_port_t uart_port);
extern INT32 fibo_rs485_write(int uart_port, UINT8 * buff, UINT32 len, bool wait_finish);
extern INT32 fibo_rs485_read(int uart_port, UINT8 * buff, UINT32 len);
extern INT32 fibo_rs485_read_try(int uart_port, UINT8 * buff, UINT32 len, uint32_t timeout);
extern INT32 fibo_rs485_read_block(int uart_port, UINT8 * buff, UINT32 len, uint32_t timeout);
#endif

//extflash
extern void fibo_ext_flash_mode_set(uint8_t mode);
extern bool fibo_ext_flash_poweron(uint8_t pinsel, uint8_t val, uint8_t div); // Notice: This interface will be invalid later, Please use the new API, fibo_ext_flash_poweron_v2.
extern bool fibo_ext_flash_poweron_v2(uint8_t pinsel, uint8_t val, uint8_t div);
extern INT32 fibo_ext_flash_init(uint8_t Pinsel);
extern INT32 fibo_ext_flash_info(UINT32 *pulId, UINT32 *pulCapacity);
extern INT32 fibo_ext_flash_read(UINT32 faddr, UINT8 *data,UINT32 size);
extern INT32 fibo_ext_flash_write(UINT32 faddr, UINT8 *data,UINT32 size);
extern INT32 fibo_ext_flash_erase(UINT32 faddr, UINT32 size);
extern INT32 fibo_ffsmountExtflash(UINT32 uladdr_start, UINT32 ulsize,
                              char *dir,UINT8 spi_pin_sel,
                              bool format_on_fail,bool force_format);

//inner flash
extern INT32 fibo_inner_flash_init(void);
extern INT32 fibo_inner_flash_info(UINT32 *pulId, UINT32 *pulCapacity);
extern INT32 fibo_inner_flash_read(UINT32 faddr, UINT8 *data,UINT32 size);
extern INT32 fibo_inner_flash_write(UINT32 faddr, UINT8 *data,UINT32 size);
extern INT32 fibo_inner_flash_erase(UINT32 faddr, UINT32 size);

extern INT8 fibo_usbswitch(uint8_t en);
extern INT32 fibo_set_usbmode(uint8_t usbmode);
extern INT32 fibo_get_usbmode(void);
extern INT8 fibo_get_usbisinsert(void);


extern UINT8 fibo_usbDevice_init(char *name, usb_input_callback_t recv_cb);
extern UINT8 fibo_usbDevice_deinit(int usb_channel);
extern UINT32 fibo_usbDevice_send(int usb_channel, const void *data, size_t length);
extern int FIBO_usbDevice_State_report(int usb_channel, Report_UsbDev_Event  usedev_Event);

#if 0
//usb
extern atDevice_t *FIBO_usbDevice_init(uint32_t name, usb_input_callback_t recv_cb);
extern int FIBO_usbDevice_send(atDevice_t *th, const void *data, size_t length);
extern int FIBO_usbDevice_State_report(atDevice_t *th, Report_UsbDev_Event  usedev_Event);
extern INT32 FIBO_usbDevice_Deinit(atDevice_t *th);
extern UINT8 fibo_get_Usbisinsert(void);
#endif
extern INT32 fibo_charge_vbat_vol_get(void);

//pwm
extern bool fibo_pwm_open(int32_t index);
extern void fibo_pwm_config(int32_t index, uint16_t duty, uint16_t arr, uint16_t psc);
extern bool fibo_pwm_close_v2(int32_t index);
extern void fibo_pwm_close(void);
extern void fibo_pwm_config_lowpower(int32_t index, uint16_t duty, uint16_t arr, uint16_t psc);

//spi
extern INT32 fibo_spi_open(drvSpiConfig_t cfg, SPIHANDLE *spiFd);
extern INT32 fibo_spi_close(SPIHANDLE *spiFd);
extern INT32 fibo_spi_send(SPIHANDLE spiFd, SPI_IOC spiIoc, void *sendaddr, uint32_t size);
extern INT32 fibo_spi_recv(SPIHANDLE spiFd, SPI_IOC spiIoc, void *readaddr, uint32_t size);
extern INT32 fibo_spi_pinctrl(void);
extern INT32 fibo_spi_send_rcv(SPIHANDLE spiFd, SPI_IOC spiIoc, void *sendaddr, void *readaddr, uint32_t size);

//pmu
extern INT32 fibo_pmu_setlevel(UINT8 pmu_type,UINT8 level);
extern bool fibo_pmu_ctrl(UINT32 id, BOOL enabled, BOOL lp_enabled);


#if 0
//xml
extern XML_Parser fibo_xml_parser_create(const XML_Char *encodingName);
extern void fibo_xml_set_user_data(XML_Parser parser, void *p);
extern void fibo_xml_set_element_handle(XML_Parser parser,
                                    XML_StartElementHandler start,
                                    XML_EndElementHandler end);
extern void fibo_xml_set_character_data_handler(XML_Parser parser, XML_CharacterDataHandler handler);
extern enum XML_Status fibo_xml_parse(XML_Parser parser, const char *s, int len, int isFinal);
extern void fibo_xml_parser_free(XML_Parser parser);
#endif

extern INT32 fibo_set_antcmd_callback(void (*callback)(void *));
extern void fibo_antcmd_callback_urc(char *buf, int len);
extern void fibo_get_rrc_connection_time(uint8_t *rat, uint32_t *time_ms);
extern INT32 fibo_get_deviceSN(char* deviceSN, int size);
extern INT32 fibo_send_cmd(int cmd, void* in_param, unsigned int in_len, void* out_param, unsigned int *out_len);
#ifdef CONFIG_FIBOCOM_XINGUODU
char *fibo_get_gtpkgver(char *buffer, int size);
#endif

//BASE64
extern INT32 fibo_base64_encode( unsigned char *dst, size_t dlen, size_t *olen,const unsigned char *src, size_t slen );
extern INT32 fibo_base64_decode( unsigned char *dst, size_t dlen, size_t *olen,const unsigned char *src, size_t slen );

//sink
extern INT32 fibo_sink_OnOff(UINT32 sinknum, BOOL onOff);
extern INT32 fibo_sinkLevel_Set(UINT32 sinknum, UINT32 light_level);

//psm
extern bool fibo_psm_mode(int mode,char *tt3412,char *tt3324);

//boot
extern UINT8 fibo_getbootcause(void);

extern void fibo_app_handle_at_init(fibo_app_handle_at_cb callback);
extern void fibo_app_report_default_urc(const char *text, size_t length);

extern bool fibo_get_security_flag(void);

extern INT32 fibo_log_to_usb(bool on);
extern INT32 fibo_get_log_flag(void);
extern bool fibo_get_log_status(uint8_t *Apsetport, uint8_t *Apport, uint8_t *Apenable, uint8_t *Cpport, uint8_t *Cpenable);

//sfile
extern INT32 fibo_sfile_init(const INT32 *path);
extern INT32 fibo_sfile_size(const INT32 *path);
extern INT32 fibo_sfile_read(const INT32 *path, UINT8 *buf, UINT32 count);
extern INT32 fibo_sfile_write(const INT32 *path, UINT8 *buf, UINT32 count);

//ble basic and service function
extern INT32 fibo_ble_onoff(uint8_t onoff);
extern INT32 fibo_ble_get_ver(char *ver);
extern INT32 fibo_ble_read_addr(uint8_t addr_type,char *addr);
extern INT32 fibo_ble_set_read_name(uint8_t set_read,uint8_t *name,uint8_t name_type);
extern INT32 fibo_ble_get_connect_state(int *state);
extern INT32 fibo_ble_set_dev_param(int param_count,int AdvMin,int AdvMax,int AdvType,int OwnAddrType,int DirectAddrType,int AdvChannMap,int AdvFilter,char *DirectAddr);
extern INT32 fibo_ble_set_dev_data(int data_len,char *data);
extern INT32 fibo_ble_enable_dev(int onoff);
extern INT32 fibo_ble_send_data(int data_len,char *data);
extern INT32 fibo_ble_set_read_mtu(uint8_t set_read, int *mtu);
extern INT32 fibo_ble_get_peer_mtu(int *mtu);
extern INT32 fibo_ble_set_scan_response_data(int data_len,char *data);
extern INT32 fibo_ble_recv_data(ble_data_recv_callback callback);
extern INT32 fibo_ble_data_request(int data_len,void *data, int timeout);
extern INT32 fibo_ble_gtsend_data(int index, int len, void* data);
extern INT32 fibo_ble_set_gtuuid(int index, void *uuid, UINT8 attr);
extern INT32 fibo_ble_response_to_read(int len, void* data);
//gnss
extern INT32 fibo_gnss_lte_prior_set(int prior);

//ble ota
extern INT32 fibo_ble_ota(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_ble_handle(INT8 *data, UINT32 len);

//ri switch
INT32 fibo_uart_ring_switch(bool ri_on);


//ble sleep
extern INT32 fibo_ble_sleep_mode(int32_t mode);

//device Number
extern INT32 fibo_set_get_deviceNUM(INT32 type,INT8 *deviceNUM,size_t length);
extern INT8 fibo_get_cgmm(UINT8 *p_cgmm_info, UINT8 cgmm_info_size);
extern INT8 *fibo_get_hw_verno(void);


//dotMatrixLcd
extern void fibo_dotMatrixLcdSetDisplayParam(uint8_t contrastRatio_coarse, uint8_t contrastRatio_fine, uint8_t bias);
extern void fibo_dotMatrixLcdSendData(uint8_t column, uint8_t row, void *buff, uint32_t size);

extern INT32 fibo_ble_uuid_config(int type,void *uuid);
extern INT32 fibo_ble_disconnect(int index);

extern int fibo_get_NetInfo_by_simid(uint32_t *pNetInfo, uint8_t DataSize, uint8_t simId);
extern void fibo_switch_sim_handle(UINT8 mode);

extern int fibo_sim_lock_pin(uint16_t nFac,char *uPwd,uint8_t simid);
extern int fibo_sim_unlock_pin(uint16_t nFac,char *uPwd,uint8_t simid);
extern int fibo_sim_get_lock_status(uint16_t nFac,int *state,uint8_t simid);


extern int fibo_sim_verify_pin(char *pinorpuk,char *newpin,int sim);
extern INT32 fibo_get_sim_det_level(void);

//extern INT32 fibo_get_imsi(UINT8 *imsi);
extern void fibo_get_chip_id(UINT8 *chipid);

extern INT32 fibo_set_imei(UINT8 *imei, int nSimID);
extern INT32 fibo_get_imsi_by_simid(UINT8 *imsi,uint8_t simId);
extern INT32 fibo_get_imsi_by_simid_v2(UINT8 *imsi,uint8_t simId);
extern uint8_t fibo_get_sim_protect(uint32_t sim_id);
extern INT32 fibo_set_sim_protect(uint8_t op_mode,uint32_t pEFID[8], uint32_t sim_id);

extern void fibo_vsim_init(VSIM_COMMAND vsim_apdu_process, uint8_t nSimID);
extern void fibo_vsim_close(VSIM_COMMAND vsim_apdu_process_fn, uint8_t nSimID);
extern INT32 fibo_xiaoshui_get_mcc_mnc(int nSim, char *mcc, char *mnc);
extern void delay_us(uint32_t us);
extern void vTaskSuspendAll(void);
extern long xTaskResumeAll(void);
extern int fibo_nw_set_config(UINT8 simId, char *name, void *input_param, void *output_param);
extern int fibo_nw_get_config(UINT8 simId, char *name, void *input_param, void *output_param);


#ifdef CONFIG_FIBOCOM_XIAONIU_FEATURE
extern INT32 fibo_niu_ota_handle(INT8 *data, UINT32 len);
extern void fibo_niu_ota_set_callback(fota_download_cb_t cb);
extern INT32 fibo_niu_app_dl(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_niu_app_update(void);
extern INT32 fibo_niu_firmware_handle(INT8 *data, UINT32 len);
extern INT32 fibo_niu_firmware_dl(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
extern INT32 fibo_niu_firmware_update(void);
extern INT32 fibo_niu_appfota_handle(INT8 *data, UINT32 len);
extern INT32 fibo_niu_appfw_handle(INT8 *data, UINT32 len);
extern INT32 fibo_niu_appfota_update(void);
extern INT32 fibo_niu_appfw_update(void);
#endif

extern bool fibo_TY_wakeup_disable(void);

extern INT32 fibo_setRTC_timezone_v2(int8_t timezone);
extern INT32 fibo_getRTC_timezone_v2(void);

extern int HAL_Get_Imei(char *IMEI);
extern int HAL_Get_Cereg(void);
extern int HAL_Get_Iccid(char *iccid);
extern int HAL_Get_IMSI(char *imsi);
extern int HAL_Get_Signal_Strength(void);
extern int HAL_Get_CGMI(char* manufacturer);
extern int HAL_Get_CGMM(char* moduleType);
extern int HAL_Get_ChipManufacturer(char* chipManufacturer);
extern int HAL_Get_ChipType(char* chipType);
extern int HAL_Get_SoftwareVersion(char* sVerion);
extern int HAL_Get_HardwareVersion(char* hVersion);
extern int HAL_Get_NueStats(T_CellInfo* cellInfo);
extern int HAL_Get_LBSParams(char* tac,char* ci);
extern int HAL_Get_PowerMode(T_PowerMode* psm);
extern int HAL_Get_Cops(char* cops);
extern int HAL_Get_CCLK(char* dateTime);
extern int HAL_Get_MemeryInfo(T_MemInfo* memInfo);
extern int HAL_Get_APN(char* apn);

#ifdef CONFIG_MC661_CN_19_70_CUDMP
extern void HAL_Ota_Set_FwSize(unsigned int size);
extern int HAL_Ota_Update_Stage(void);
extern bool HAL_Ota_is_MiniSys(void);
extern int HAL_Ota_Enable_MiniSys(void);
extern bool HAL_Ota_Update_Init(void);
extern int HAL_Ota_Write_Flash(char* data, unsigned int size);
extern bool HAL_Ota_Check_Fw_Validate(void);
extern void HAL_Ota_Update_Start(void);
extern void HAL_Ota_Update_Deinit(void);
#endif

extern INT32 fibo_key_register_callback(ffw_key_cb_t cb, void *ctx, UINT32 long_press_time);
extern INT32 fibo_key_release_callback(void);

extern int fibo_get_NetInfo(uint32_t *pNetInfo, uint8_t DataSize);
//other
extern INT32 fibo_watchdog_feed(void);
#ifdef CONFIG_FFW_FEATURE_ONE_WIRE
extern int fibo_one_wire_communication_deinit(void);
extern int fibo_one_wire_communication_send(uint8_t *data, uint16_t len);
extern int fibo_one_wire_communication_init(fibo_one_wire_config_t *config);
#endif

#ifdef CONFIG_FFW_FEATURE_CAN
extern int32 fibo_can_init(uint8 can_port, f_can_config_t *can_config, can_recv_callback_t recv_cb, void *arg);
extern int32 fibo_can_deinit(uint8 can_port);
extern int32 fibo_can_send(uint8 can_port, f_can_frame_t *pFrame);
#endif

#ifdef CONFIG_CUST_FUTE_FEATURE
extern cJSON * cJSON_Parse(const char *value);
extern void cJSON_Delete(cJSON *item);
extern char * cJSON_Print(const cJSON *item);
extern cJSON * cJSON_CreateArray(void);
extern int cJSON_GetArraySize(const cJSON *array);
extern cJSON * cJSON_GetArrayItem(const cJSON *array, int index);
extern void cJSON_AddItemToArray(cJSON *array, cJSON *item);
extern cJSON * cJSON_CreateString(const char *string);
extern cJSON * cJSON_CreateNumber(double num);
extern cJSON * cJSON_CreateObject(void);
extern void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);
extern cJSON * cJSON_GetObjectItem(const cJSON * const object, const char * const string);
extern char * cJSON_PrintUnformatted(const cJSON *item);
extern void * cJSON_malloc(size_t size);
extern void cJSON_free(void *object);
#endif

#ifdef CONFIG_CUST_NBSANXING_FEATURE
extern int fibo_sim_enable_pin(uint16_t nFac,char *uPwd,uint8_t simid);
extern int fibo_sim_disable_pin(uint16_t nFac,char *uPwd,uint8_t simid);
extern int fibo_sim_change_pin(char *pin, char *newpin, uint8_t simid);
extern char *fibo_get_ICCID(CFW_SIM_ID nSimID);
extern INT32 fibo_getCellInfo_ex(key_callback cellinfo_refponse_cb, CFW_SIM_ID nSimID);
extern INT32 fibo_getRegInfo(reg_info_t *reg_info,CFW_SIM_ID nSimID);
extern INT32 fibo_cfun_one();
extern INT32 fibo_cfun_zero();
extern INT32 fibo_get_moudleID(UINT8 *moudle_id);
extern INT32 fibo_get_sim_tel_num(void (*cb)(UINT8 *sim_tel_num), uint8_t simId);
extern INT32 fibo_set_prior_RAT(UINT8 nPreferRat,CFW_SIM_ID nSim);
#endif
extern INT32 fibo_get_scell_info(Open_SCellInfo *cellinfo_refponse);
extern INT32 fibo_get_cell_info_cache(opencpu_cellinfo *openCellInfo);

extern void *fibo_app_start(uint32_t appimg_flash_offset,uint32_t appimg_flash_size,uint32_t appimg_ram_offset,uint32_t appimg_ram_size,void *parm);
#endif /*_FIBO_OPENCPU_H_*/

