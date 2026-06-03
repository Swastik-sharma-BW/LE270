#include "app_image.h"
#include "fibo_opencpu.h"

#define AEP_HOSTNAME "mqtt.ctwing.cn"
#define AEP_HOST_PORT 1883

#define AEP_USERNAME "aep_test"
#define AEP_PASSWORD "plS7YsYqjijmCzN6pJxtscrTt1e6zLNjr1_6jrv8V60"
#define AEP_CLIENTID "15357005dev001"

static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    int result = 0;
    char* topic = NULL;
    char* message = NULL;
    int qos = 0;
    uint32_t len = 0;
    uint16_t pacid = 0;
    
    switch (sig)
    {
        case GAPP_SIG_CONNECT_RSP:
        {
            result = va_arg(arg, int);
            fibo_textTrace("connect result:%d", result);
            if (result == 1)
            {
                fibo_mqtt_pub_v2((INT8*)"data_report", 0, 0,(INT8*)"{\"water_flow\": 0.1}",strlen("{\"water_flow\": 0.1}"), &pacid);
            }
            break;
        }
        case GAPP_SIG_CLOSE_RSP:
        {
            result = va_arg(arg, int);
            fibo_textTrace("close result:%d", result);
            break;
        }
        case GAPP_SIG_SUB_RSP:
        {
            result = va_arg(arg, int);
            fibo_textTrace("sub result:%d", result);
            break;
        }
        case GAPP_SIG_UNSUB_RSP:
        {
            result = va_arg(arg, int);
            fibo_textTrace("unsub result:%d", result);
            break;
        }
        case GAPP_SIG_PUB_RSP:
        {
            result = va_arg(arg, int);
            fibo_textTrace("pub result:%d", result);
            break;
        }
        case GAPP_SIG_INCOMING_DATA_RSP:
        {
            topic = va_arg(arg, char *);
            qos = va_arg(arg, int);
            message = va_arg(arg, char *);
            len = va_arg(arg, uint32_t);
            fibo_textTrace("mqttapi recv message :topic:%s, qos=%d message=%s len=%d", topic, qos, message, len);
            fibo_textTrace("incomming result:%d", result);
            break;
        }
        default:
        {
            break;
        }
    }
}

static void at_res_callback(UINT8 *buf, UINT16 len)
{
    fibo_textTrace("FIBO <--%s", buf);
}


static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};

static void hello_world_demo()
{
    fibo_textTrace("application thread enter");
    set_app_ver(app_ver);
    for (int n = 0; n < 10; n++)
    {
        fibo_textTrace("app image hello world %d", n);
        fibo_taskSleep(500);
    }

    reg_info_t reg_info;
    while (1)
    {
        fibo_reg_info_get(&reg_info, 0);
        fibo_taskSleep(1000);
        fibo_textTrace("[fibocom %s-%d]", __FUNCTION__, __LINE__);
        if (reg_info.nStatus == 1)
        {
            fibo_pdp_active(1, NULL,0);
            fibo_taskSleep(1000);
            fibo_textTrace("[fibocom %s-%d]", __FUNCTION__, __LINE__);
            break;
        }
    }

    fibo_taskSleep(20*1000);
    fibo_mqtt_set((INT8*)AEP_USERNAME, (INT8*)AEP_PASSWORD);
    fibo_mqtt_connect_v2((INT8*)AEP_CLIENTID, (INT8*)AEP_HOSTNAME, AEP_HOST_PORT,1,300,0,5000);
    fibo_textTrace("[fibocom %s-%d]", __FUNCTION__, __LINE__);
    
    while(1)
    {
        fibo_taskSleep(10*1000);
    }
    
}


void *appimg_enter(void *param)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_enter_1, P_INFO, "application appimg_enter");
    fibo_textTrace("app image enter");
    fibo_thread_create(hello_world_demo, (INT8 *)"mythread", 10*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_exit_1, P_INFO, "application image exit");
    return;
}
