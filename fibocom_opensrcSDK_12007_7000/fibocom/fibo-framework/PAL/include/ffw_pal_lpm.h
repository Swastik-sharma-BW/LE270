
#ifdef CONFIG_FFW_LPM_SUPPORT

#ifndef _FFW_PAL_PM_H_
#define _FFW_PAL_PM_H_

#define LOCK_MAX 32

typedef union{
    int32_t lpm_mode;
    uint16_t lpm_pin;
}ffw_lpm_type;

typedef struct{
    ffw_lpm_type lpm_type;
    int32_t lpm_level;
}ffw_pm_lpmmode_cfg;

typedef struct {
    int32_t wake_host_mode;
    int32_t wake_host_level;
}ffw_pm_wake_host_cfg;

int32_t ffw_pal_pm_set_autosleep_cfg(int32_t autosleep_time);
int32_t ffw_pal_pm_wake_lock_is_exist(char *lock_name);
int32_t ffw_pal_pm_wake_lock(char *lock_name);
int32_t ffw_pal_pm_wake_unlock(char *lock_name);
int32_t ffw_pal_pm_init();
int32_t ffw_pal_pm_get_wakeup_gpio_level(ffw_pm_lpmmode_cfg *pm_get_wakeup_cfg);
int32_t ffw_pal_pm_set_lpm_cfg(ffw_pm_lpmmode_cfg pm_lpmmode_cfg_set);
int32_t ffw_pal_pm_enable_usb(bool enable);
int32_t ffw_pal_pm_ft_enter_sleep();
int32_t ffw_pal_pm_set_wake_host_config(ffw_pm_wake_host_cfg pm_wake_host_cfg);

#ifdef CONFIG_FFW_LPM_PSM_SUPPORT
int32_t ffw_pal_pm_enter_psm();
int32_t ffw_pal_pm_psm_init();
int32_t ffw_pal_pm_is_psm_mode();
#endif

int32_t ffw_pal_doze_init(uint8_t *id);
void ffw_pal_doze_vote(uint8_t id, bool allow);
void ffw_pal_doze_deinit(uint8_t id);

#endif

#endif

