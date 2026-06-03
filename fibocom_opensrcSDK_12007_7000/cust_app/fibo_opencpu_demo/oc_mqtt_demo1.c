#include "app_image.h"
#include "fibo_opencpu.h"

static UINT32 g_lock = 0;

UINT8 id = 1;
INT8 client_id[] = "lens_0ptfvqWWCzblt9phGtddgOdO3z3";
INT8 host[] = "47.110.234.36";
UINT16 port = 1883;
UINT8 clear_session = 0;
UINT16 keepalive = 10;
UINT8 UseTls = false;
INT32 timeout = 60 * 1000;
BOOL run = false;
BOOL quit = true;
int connect_res_result = 0;
char *topic = "topic";
UINT8 qos = 0;
BOOL retain = 0;
char* message = "hello";

static void mqtt_thread_entry(void *param);

static void create_mqtt_connect()
{
    run = false;
    connect_res_result = 0;
    fibo_textTrace("mqttapi thread create start");
    while (!quit)
    {
        fibo_taskSleep(1000);
    }
    fibo_textTrace("mqttapi thread create");
    fibo_thread_create(mqtt_thread_entry, (int8_t*)"mqtt-thread", 1024 * 16, NULL, OSI_PRIORITY_NORMAL);
}

void user_signal_process(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    fibo_textTrace("mqttapi RECV SIGNAL:%d", sig);
    char *topic;
    int8_t qos;
    char *message;
    uint32_t len;
    int ret = 1;
    switch (sig)
    {
    case GAPP_SIG_CONNECT_RSP1:
        connect_res_result = ret = va_arg(arg, int);
        fibo_textTrace("mqttapi connect :%s", ret ? "ok" : "fail");
        fibo_sem_signal(g_lock);
        break;
    case GAPP_SIG_CLOSE_RSP1:
        ret = va_arg(arg, int);
        fibo_textTrace("mqttapi close :%s", ret ? "ok" : "fail");
        break;
    case GAPP_SIG_SUB_RSP1:
        ret = va_arg(arg, int);
        fibo_textTrace("mqttapi sub :%s", ret ? "ok" : "fail");
        fibo_sem_signal(g_lock);
        break;
    case GAPP_SIG_UNSUB_RSP1:
        ret = va_arg(arg, int);
        fibo_textTrace("mqttapi unsub :%s", ret ? "ok" : "fail");
        fibo_sem_signal(g_lock);
        break;
    case GAPP_SIG_PUB_RSP1:
        ret = va_arg(arg, int);
        fibo_textTrace("mqttapi pub :%s", ret ? "ok" : "fail");
        fibo_sem_signal(g_lock);
        break;
    case GAPP_SIG_INCOMING_DATA_RSP1:
        id = va_arg(arg, int);
        topic = va_arg(arg, char *);
        qos = va_arg(arg, int);
        message = va_arg(arg, char *);
        len = va_arg(arg, uint32_t);
        fibo_textTrace("mqttapi recv message :id:%d,topic:%s, qos=%d message=%s len=%d", id,topic, qos, message, len);
        break;

    default:
        break;
    }
}

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = user_signal_process};

static void mqtt_thread_entry(void *param)
{
    fibo_textTrace("application thread enter, param 0x%x", param);
    int ret = 0;
    int test = 1;
    fibo_textTrace("mqttapi wait network");
    reg_info_t reg_info;
    quit = false;
    run = true;
    fibo_pdp_profile_t pdp_profile;

    memset(&pdp_profile, 0, sizeof(fibo_pdp_profile_t));
    pdp_profile.cid = 1;
    memcpy(pdp_profile.nPdpType, "IP", strlen("IP"));

    if (g_lock == 0)
    {
        g_lock = fibo_sem_new(0);
    }
    while (test)
    {
        fibo_reg_info_get(&reg_info, 0);
        fibo_taskSleep(1000);
        fibo_textTrace("[%s-%d]", __FUNCTION__, __LINE__);
        if(reg_info.nStatus == 1)
        {
            test = 0;
            fibo_pdp_active(1, &pdp_profile, 0);
            fibo_taskSleep(1000);
            fibo_textTrace("[%s-%d]", __FUNCTION__, __LINE__);
            break;
        }
    }

    fibo_taskSleep(1 * 1000);
    fibo_textTrace("mqtt [%s-%d]", __FUNCTION__, __LINE__);
    fibo_mqtt_set1(id, (int8_t*)"test", (int8_t*)"123");
    //fibo_set_ssl_chkmode(0);

    do
    {
        ret = fibo_mqtt_connect1(id, client_id, host, port, clear_session, keepalive, UseTls);
        fibo_taskSleep(2000);
        fibo_textTrace("mqttapi connect ret = %d", ret);
    } while (ret < 0);

    fibo_textTrace("mqttapi connect finish");
    fibo_sem_wait(g_lock);
    fibo_textTrace("mqttapi start sub topic");
    if (connect_res_result)
    {
        ret = fibo_mqtt_sub1(id, (int8_t*)topic, qos);
        if (ret < 0)
        {
            fibo_textTrace("mqttapi sub failed");
        }
        ret = fibo_mqtt_pub1(id, (int8_t*)topic, qos, retain, (int8_t*)message, strlen(message));
        if (ret < 0)
        {
            fibo_textTrace("mqttapi sub failed");
        }
    }
    else
    {
        run = false;
        fibo_textTrace("mqttapi connect res fail");
    }

    while (run)
    {
        fibo_textTrace("mqttapi running");
        fibo_taskSleep(2 * 1000);
    }
    quit = true;
    fibo_sem_wait(g_lock);
    fibo_textTrace("mqttapi thread exit");
}


void *appimg_enter(void *param)
{
    fibo_textTrace("application image enter, param 0x%x", param);
    create_mqtt_connect();
    return (void *)&user_callback;
}
