#ifndef  __FIBO_CONFIG_H__
#define  __FIBO_CONFIG_H__

char *get_open_cgmr_value(void);
char *get_open_cgmm_new_value(void);
char *get_open_cgmm_abs_value(void);
char *get_open_cgmm_value(void);

//fibo driver
typedef enum
{
    E_DRIVER_LE270_PROJECT = 0,
    E_DRIVER_LE230_PROJECT,
    E_DRIVER_LE370_PROJECT,
    E_DRIVER_LE371_PROJECT,
    E_DRIVER_LE250_PROJECT,
    E_DRIVER_LE270_1S3W8_PROJECT
}fibo_dirver_project_e;

//fibo codec sleep
typedef enum
{
    CODEC_ES8311_LONG_ON_INTO_SLEEP = 0,
    CODEC_ES8311_POWER_OFF,
    CODEC_ES8311_LONG_ON_NOT_INTO_SLEEP
}fibo_codec_es8311_power_state_e;

int fibo_dirver_get_project();

int ffw_dirver_main_uart_wakeup_en();
int is_jiuan_project(void);
int ffw_normal_gpio_out_restore_cfg(void);
char *ffw_update_usb_product_name(void);
int ffw_gpio_isr_debounce_en();
int ffw_wakeup_host_en();
int ffw_lpg_fun_en();
int ffw_codec_es8311_into_sleep();
int fibo_get_unzip_option();
int ffw_uart_tx_send_dma_en(void);
int ffw_audio_stop_no_wait_en(void);
int ffw_audio_gain_cust_set_percent_or_db(void);
void ffw_audio_gain_cust_set(int * algorithm_gain_list,int cnt);

#endif
