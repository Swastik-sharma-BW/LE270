#ifndef __OC_FOTA_H__
#define __OC_FOTA_H__
#include "commontypedef.h"

/**
 * \brief firmware update status
 */
typedef enum fupdateResult
{
    FUPDATE_RESULT_NOT_READY,   ///< not ready
    FUPDATE_RESULT_CANNT_START, ///< failed to start
    FUPDATE_RESULT_FAILED,      ///< started and failed
    FUPDATE_RESULT_FINISHED,    ///< finished
} fupdateResult_t;
    

INT32 fibo_firmware_ota(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
INT32 fibo_firmware_handle_no_reboot(INT8 *data, UINT32 len);
INT32 fibo_firmware_handle(INT8 *data, UINT32 len);
INT32 fibo_firmware_dl(UINT8 type,UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
INT32 fibo_firmware_update(void);
INT32 fibo_firmware_ota(UINT8 type, UINT8 *url,UINT8 *filename,UINT8 *username,UINT8 *password);
void fibo_ota_set_callback(fota_download_cb_t cb);
INT32 fibo_ota_errcode(void);
INT32 fibo_firmware_verify(UINT32 mode, INT8 *data, UINT32 len);
INT32 fibo_firmware_option_set(UINT8 type, INT8 *path);
INT32  fibo_firmware_del();
INT32 fibo_device_get_update_status(fupdateResult_t *status);


#endif
