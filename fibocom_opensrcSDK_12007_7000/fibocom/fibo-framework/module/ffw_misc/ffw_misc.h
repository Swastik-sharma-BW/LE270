#ifndef __FFW_MISC_H__
#define  __FFW_MISC_H__


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "ffw_device.h"

typedef struct
{
    uint32_t type;
    uint32_t rate;
} st_mtsm;

int32_t ffw_misc_cmd_tgpio(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t ffw_misc_cmd_gpio(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t ffw_misc_cmd_mmad(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t ffw_misc_cmd_gtmlv(int32_t cmd_type, void *params, int32_t params_count, void *engine);
#ifdef CONFIG_FFW_FEATURE_SUPPORT_AT_SIMPHASE
int32_t ffw_misc_cmd_simphase(int32_t cmd_type, void *params, int32_t params_count, void *engine);
#endif
int32_t ffw_misc_cmd_cbc(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t ffw_misc_cmd_mtsm(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t ffw_misc_cmd_cpwroff(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t ffw_misc_cmd_secbooten(int32_t cmd_type, void *params, int32_t params_count, void *engine);

#ifdef CONFIG_FFW_EC200_AT_ENABLE
int32_t ffw_misc_cmd_qpowd(int32_t cmd_type, void *params, int32_t params_count, void *engine);
#endif

int32_t ffw_misc_cmd_gtdlmode(int32_t cmd_type, void *params, int32_t params_count, void *engine);
#ifdef CONFIG_FFW_THERMAL_ENABLE
void ffw_misc_temp_control_init(void);
#endif
int32_t ffw_misc_charge_vbat_vol_get(int32_t *data);

#ifdef CONFIG_CUST_TUOKUO_FEATURE
typedef struct
{
    uint8_t adc_det_on_off;
    uint8_t adc_channel;
    uint16_t adc_threshold;
    uint16_t adc_det_dur;
}ffw_adcdet_t;

int32_t ffw_misc_cmd_qadc(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t ffw_misc_cmd_adcdet(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t ffw_misc_atcmd_gtvrictr(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void ffw_misc_gtvrictr_init(void);
#endif

#ifdef CONFIG_CUST_KR_FEATURE
int32_t ffw_misc_atcmd_gtwdtrst(int32_t cmd_type, void *params, int32_t params_count, void *engine);
#endif

#endif
