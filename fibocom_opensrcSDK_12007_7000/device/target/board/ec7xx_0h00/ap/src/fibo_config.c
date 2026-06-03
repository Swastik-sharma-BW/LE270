#include "fibo_config.h"


char *g_open_cgmr = CONFIG_FFW_CGMR;
char *g_open_cgmm = CONFIG_FFW_CGMM;
char *g_open_cgmm_new = CONFIG_FFW_CGMM_NEW;
char *g_open_cgmm_abs = CONFIG_FFW_CGMM_ABS;

char *get_open_cgmr_value(void)
{
    return g_open_cgmr;
}
char *get_open_cgmm_new_value(void)
{
    return g_open_cgmm_new;
}

char *get_open_cgmm_abs_value(void)
{
    return g_open_cgmm_abs;
}
char *get_open_cgmm_value(void)
{
    return g_open_cgmm;
}

//fibo driver
int fibo_dirver_get_project()
{
#ifdef CONFIG_LE230_PROJECT
    return E_DRIVER_LE230_PROJECT;
#elif defined CONFIG_LE370_PROJECT
    return E_DRIVER_LE370_PROJECT;
#elif defined CONFIG_LE371_PROJECT
    return E_DRIVER_LE371_PROJECT;
#elif defined CONFIG_LE250_PROJECT
    return E_DRIVER_LE250_PROJECT;
#elif defined CONFIG_LE270_1S3W8_PROJECT
    return E_DRIVER_LE270_1S3W8_PROJECT;
#else
    return E_DRIVER_LE270_PROJECT;
#endif
}

int ffw_dirver_main_uart_wakeup_en()
{
#ifdef CONFIG_FFW_MAIN_UART_SUPPORT_WAKEUP
    return 1;
#endif
    return 0;
}
int is_jiuan_project(void)
{
#ifdef CONFIG_FFW_CUST_SDK_JIUAN
    return 1;
#else
    return 0;
#endif
}
//Resume normal gpio port output after sleep wake-up
int ffw_normal_gpio_out_restore_cfg(void)
{
#ifdef CONFIG_FFW_NORMAL_GPIO_OUT_RESTORE_CFG
    return 1;
#else
    return 0;
#endif
}

static char *usb_product = CONFIG_FFW_CGMM_ABS;
char *ffw_update_usb_product_name(void)
{
    return usb_product;
}

//oc_isr_t->debounce param take effect
int ffw_gpio_isr_debounce_en()
{
#ifdef CONFIG_FFW_FEATURE_GPIO_ISR_DEBOUNCE_EN
    return 1;
#else
    return 0;
#endif
}

//if disable, AP no control RI, RI low level
int ffw_wakeup_host_en()
{
#ifdef CONFIG_FFW_LPM_WAKE_HOST_SUPPORT
    return 1;
#else
    return 0;
#endif
}

int ffw_codec_es8311_into_sleep()
{
#if defined CONFIG_FFW_CODEC_ES8311_LONG_ON_INTO_SLEEP
    return CODEC_ES8311_LONG_ON_INTO_SLEEP;
#elif defined CONFIG_FFW_CODEC_ES8311_POWER_OFF
    return CODEC_ES8311_POWER_OFF;
#else //CODEC_ES8311_LONG_ON_NOT_INTO_SLEEP
    return CODEC_ES8311_LONG_ON_NOT_INTO_SLEEP;
#endif
}

int ffw_lpg_fun_en()
{
#ifdef CONFIG_FFW_FEATURE_LPG_DEFAULT_DISABLE
    return 0;
#else
    return 1;
#endif
}

int fibo_get_unzip_option()
{
    return 1;
}

int ffw_http_post_no_copy_get(void)
{
    return 0;
}

int ffw_uart_tx_send_dma_en(void)
{
#ifdef CONFIG_FFW_FEATURE_UART_TX_DMA_EN
    return 1;
#else
    return 0;
#endif
}

int ffw_audio_stop_no_wait_en(void)
{
    return 0;
}
int ffw_audio_gain_cust_set_percent_or_db(void)
{
    //return val = 0:dB(default) 1:percent
    return 0;
}

void ffw_audio_gain_cust_set(int * algorithm_gain_list,int cnt)
{
    /** when ffw_audio_gain_cust_set_percent_or_db() set return value = 0
     ** Volume adjustment ranges from -11 to 11 dB.*/

    /*
    int volumelist[] = {-11, -9, -7, -5, -3, -1, 1, 3}; //default
    for(int i = 0;i < cnt; i++)
    {
        algorithm_gain_list[i] = volumelist[i];
    }
    */

    /** when ffw_audio_gain_cust_set_percent_or_db() set return value = 1
     ** The proportion of volume adjustment ranges from 0 to 100.*/

    /*
    int volumelist[8] = {1, 2, 3, 4, 6, 7, 8, 10}; //default
    for(int i = 0;i < cnt; i++)
    {
        algorithm_gain_list[i] = volumelist[i];
    }
    */
    return ;
}
