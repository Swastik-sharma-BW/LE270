#include "app_image.h"
#include "fibo_opencpu.h"

char onenet_clientid[256] = {0};
uint32_t g_onenet_sem = 0;

#define ONENET_PRODUCT_ID "POT5tD4mdR"
#define ONENET_DEVICE_NAME "867145060001323"
#define ONENET_ACCESSKEY "aHNXRUU5UlBDQ3pwRDNRbWdoU3hEWkNNT1E1OU1QUDM="

#define ONENET_HOSTNAME "mqtts.heclouds.com"
#define ONENET_HOST_PORT 1883

#define ONENET_PROPERTY_PAYLOAD "{\"id\":\"123\",\"version\":\"1.0\",\"params\":{\"BrightValue\": {\"value\": 56,\"time\":1692946833000}}}"


static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    char *username = NULL;
    char *password = NULL;
    char *clientid = NULL;
    int result = 0;
    char* topic = NULL;
    char* message = NULL;
    int qos = 0;
    uint32_t len = 0;
    uint16_t pacid = 0;
    
    switch (sig)
    {
        case GAPP_SIG_MQTT_CLOUD_AUTH_ONENET:
        {
            fibo_textTrace("fibocom mqtt cloud request finish sig: %d",sig);
            username = va_arg(arg, char *);
            password = va_arg(arg, char *);
            clientid = va_arg(arg, char *);
            fibo_textTrace("fibocom cloud username: %s",username);
            fibo_textTrace("fibocom cloud password: %s",password);
            fibo_textTrace("fibocom cloud clientid: %s",clientid);
            fibo_mqtt_set((INT8*)username, (INT8*)password);
            memcpy(onenet_clientid, clientid, strlen(clientid));
            fibo_sem_signal(g_onenet_sem);
            fibo_textTrace("fibocom signal sem");
            break;
        }
        case GAPP_SIG_CONNECT_RSP:
        {
            result = va_arg(arg, int);
            fibo_textTrace("fibocom connect result:%d", result);
            fibo_mqtt_sub_v2((INT8*)"$sys/POT5tD4mdR/867145060001323/thing/property/post/reply", 0, &pacid);
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
            fibo_textTrace("fibocom sub result:%d", result);
            fibo_mqtt_pub_v2((INT8*)"$sys/POT5tD4mdR/867145060001323/thing/property/post", 0, 0,(INT8*)ONENET_PROPERTY_PAYLOAD,strlen(ONENET_PROPERTY_PAYLOAD), &pacid);
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
            fibo_textTrace("fibocom pub result:%d", result);
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
    g_onenet_sem = fibo_sem_new(0);
    while (1)
    {
        fibo_reg_info_get(&reg_info, 0);
        fibo_taskSleep(10*1000);
        fibo_textTrace("[fibocom %s-%d]", __FUNCTION__, __LINE__);
        if (reg_info.nStatus == 1)
        {
            fibo_pdp_active(1, NULL,0);
            fibo_taskSleep(1000);
            fibo_textTrace("[fibocom %s-%d]", __FUNCTION__, __LINE__);
            break;
        }
    }
    fibo_taskSleep(10*1000);
    fibo_mqtt_onenet_config((uint8_t*)ONENET_PRODUCT_ID,(uint8_t*)ONENET_DEVICE_NAME,(uint8_t*)ONENET_ACCESSKEY);
    fibo_textTrace("[fibocom %s-%d]onenet config", __FUNCTION__, __LINE__);
    fibo_sem_wait(g_onenet_sem);
    fibo_textTrace("[fibocom %s-%d] get username/password/clientid", __FUNCTION__, __LINE__);
    fibo_mqtt_connect_v2((INT8*)onenet_clientid, (INT8*)ONENET_HOSTNAME, ONENET_HOST_PORT,1,300,0,5000);
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
