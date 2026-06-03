#include "app_image.h"
#include "fibo_opencpu.h"
#include "app.h"
#include "app_pdp.h"

#if 1

static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    APP_LOG_INFO("sig_res_callback  sig = %d (200 is opencpu test.)", sig);
    
}

static void at_res_callback(UINT8 *buf, UINT16 len)
{
    APP_LOG_INFO("FIBO <--%s", buf);
}
#endif

void uart_recv(hal_uart_port_t uart_port, UINT8 *data, UINT16 len, void *arg)
{
    APP_LOG_INFO("uartapi recv uart_port=%d len=%d, data=%s", uart_port, len, (char *)data);
}

void uart3_recv(hal_uart_port_t uart_port, UINT8 *data, UINT16 len, void *arg)
{
    APP_LOG_INFO("uartapi recv uart_port=%d len=%d, data=%s", uart_port, len, (char *)data);
}

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};

static void uart_demo()
{
    APP_LOG_INFO("application thread enter");
    set_app_ver(app_ver);

    for (int n = 0; n < 10; n++)
    {
        APP_LOG_INFO("app image hello world %d", n);
        fibo_taskSleep(500);
    }

    int ret = -1;
    hal_uart_config_t drvcfg = {0};
    void *arg = NULL;
    char *buff2 = "uart_test2 test";
    char *buff3 = "uart_test3 test";

    drvcfg.baud = 115200;
    drvcfg.parity = HAL_UART_NO_PARITY;
    drvcfg.data_bits = HAL_UART_DATA_BITS_8;
    drvcfg.stop_bits = HAL_UART_STOP_BITS_1;
    drvcfg.rx_buf_size = 2048;
    drvcfg.tx_buf_size = 2048;

    //UART2
    fibo_gpio_mode_set(28,3);
    fibo_gpio_mode_set(29,3);
    
    //UART3
    fibo_gpio_mode_set(52,3);
    fibo_gpio_mode_set(53,3);

    ret = fibo_hal_uart_init(2, &drvcfg, uart_recv, arg);
    APP_LOG_INFO("uart_test2 uart init ret: %d", ret);
    ret = fibo_hal_uart_init(3, &drvcfg, uart3_recv, arg);
    APP_LOG_INFO("uart_test1 uart init ret: %d", ret);

    for(int n = 0; n < 3; n++) {
        APP_LOG_INFO("uart_test send start");
        fibo_hal_uart_put(2, (UINT8 *)buff2, strlen(buff2));
        fibo_hal_uart_put(3, (UINT8 *)buff3, strlen(buff3));
        fibo_taskSleep(2000);
    }

    fibo_hal_uart_deinit(2);  //uart2 will not recv data after deinit
    fibo_hal_uart_put(2, (UINT8 *)buff2, strlen(buff2));    //will not put
    fibo_hal_uart_put(3, (UINT8 *)buff3, strlen(buff3));    //will put
}


void *appimg_enter(void *param)
{
    APP_LOG_INFO("app image enter");
    fibo_thread_create(uart_demo, (INT8 *)"uartthread", 10*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
    return;
}
