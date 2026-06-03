#ifndef LIB_INCLUDE_FFW_ATDEVICE_PLAT
#define LIB_INCLUDE_FFW_ATDEVICE_PLAT
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "ffw_list.h"

typedef enum
{
    FFW_ATDEVICE_NONE,
    FFW_ATDEVICE_UART,
    FFW_ATDEVICE_USB,
    FFW_ATDEVICE_DEVICE_NUM,
} ffw_atdevice_device_type_t;

typedef enum
{
    FFW_ATDEVICE_FORMAT_8N2 = 1, ///< 8 Data; 2 Stop
    FFW_ATDEVICE_FORMAT_811,     ///< 8 Data; 1 Parity; 1 Stop
    FFW_ATDEVICE_FORMAT_8N1,     ///< 8 Data; 1 Stop
} ffw_atdevice_format_t;

typedef enum
{
    FFW_ATDEVICE_PARITY_ODD,  ///< Odd
    FFW_ATDEVICE_PARITY_EVEN, ///< Even
    FFW_ATDEVICE_PARITY_MARK, ///< Mark
    FFW_ATDEVICE_PARITY_SPACE ///< Space
} ffw_atdevice_parity_t;

typedef enum
{
    FFW_ATDEVICE_RXFC_NONE,  ///< None
    FFW_ATDEVICE_RXFC_HW = 2 ///< Circuit 133 (Ready for Receiving)
} ffw_atdevice_rxfc_t;

typedef enum
{
    FFW_ATDEVICE_TXFC_NONE,  ///< None
    FFW_ATDEVICE_TXFC_HW = 2 ///< Circuit 106 (Clear to Send/Ready for Sending)
} ffw_atdevice_txfc_t;

typedef enum
{
    FFW_ATDEVICE_DCD_MODE0,   /// DCD turn ON always
    FFW_ATDEVICE_DCD_MODE1,    /// DCD turn ON when in data state
    FFW_ATDEVICE_DCD_MODE_MAX
} ffw_atdevice_dcd_mode_t; /// AT&C[value] Circuit 109 (Received line signal detector) behaviour

typedef enum
{
    FFW_ATDEVICE_DTR_MODE0,
    FFW_ATDEVICE_DTR_MODE1,
    FFW_ATDEVICE_DTR_MODE2,
    FFW_ATDEVICE_DTR_MODE_MAX
} ffw_atdevice_dtr_mode_t;

int32_t ffw_atdevice_set_baudrate(void *atDevice, uint32_t baud, bool persist);

int32_t ffw_atdevice_get_baudrate(void *atDevice, uint32_t *baud);

int32_t ffw_atdevice_set_format(void *atDevice, ffw_atdevice_format_t format, ffw_atdevice_parity_t parity);

int32_t ffw_atdevice_get_format(void *atDevice, ffw_atdevice_format_t *format, ffw_atdevice_parity_t *parity);

int32_t ffw_atdevice_set_flow_ctrl(void *atDevice, ffw_atdevice_rxfc_t rxfc, ffw_atdevice_txfc_t txfc);

int32_t ffw_atdevice_get_flow_ctrl(void *atDevice, ffw_atdevice_rxfc_t *rxfc, ffw_atdevice_txfc_t *txfc);

int32_t ffw_atdevice_support_baud_list(void *atDevice, uint32_t **baud_list);

int32_t ffw_atdevice_set_dcd_mode(void *atDevice, ffw_atdevice_dcd_mode_t mode);

int32_t ffw_atdevice_get_dcd_mode(void *atDevice, ffw_atdevice_dcd_mode_t* mode);

int32_t ffw_atdevice_set_dtr_mode(void *atDevice, ffw_atdevice_dtr_mode_t mode);

int32_t ffw_atdevice_get_dtr_mode(void *atDevice, ffw_atdevice_dtr_mode_t* mode);


#endif /* LIB_INCLUDE_FFW_ATDEVICE_PLAT */
