#include "app_image.h"
#include "fibo_opencpu.h"
#include <time.h>
UINT32 quid = 0;
extern void *appimg_enter(void *param);

typedef enum  e_option
{
    E_ACTIVE = 0x01,
    E_SEND_AT,
    E_MSG_NUM
    
}e_option;
typedef struct ota_msg
{
    int   opetion;
    int      type;
}ota_msg;


static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    int mode = va_arg(arg, int); //
    int num  = va_arg(arg, int);
    va_end(arg); 
    ota_msg  msg;
    
    fibo_textTrace("sig_res_callback  sig = %d (200 is opencpu test.) mode =%d num =%d", sig, mode, num);
    if (GAPP_SIG_OPENCPU_API_TEST != sig)
        return;
    fibo_textTrace("sig_res_callback  quid = %x mode =%d num =%d", quid, mode, num);
    if (0 == quid)
    {
        return;
    }
    switch(num)
    {
        case 1: //active
            msg.opetion = E_ACTIVE;
            fibo_queue_put(quid,&msg,10);
            break;

            
        case 2: 
            msg.opetion = E_SEND_AT;
            fibo_queue_put(quid,&msg,10);
            break;  

        default:
            msg.opetion = E_MSG_NUM;
            fibo_queue_put(quid,&msg,10);
            break;
    }

    return;
}


static void at_res_callback(UINT8 *buf, UINT16 len)
{
    fibo_textTrace("FIBO <--%s", buf);
}

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};

extern void fibo_set_data_buffer(char *p);
void demo_active_pdp()
{
    fibo_pdp_profile_t  prof;
    memset(&prof,0,sizeof(prof));
    prof.cid=1;
    memcpy(prof.apn,"CMIOT",sizeof("CMIOT"));
    memcpy(prof.nPdpType,"IP",sizeof("IP"));
    fibo_pdp_active(1,&prof,1);
    return;
}



int demo_send_at()
{
    char str[] = "AT+COPS?\r\n";
    int len = strlen(str);

    fibo_textTrace("application thread enter appimg_enter addr = %x", appimg_enter);
    return fibo_at_send((UINT8 *)str,len);
}

int demo_send_at_cereg()
{
    char str[] = "AT+CEREG=2\r\n";
    int len = strlen(str);
    return fibo_at_send((UINT8 *)str,len);
}




static void at_demo()
{
	int count = 0;
	int ret = -1;
    ota_msg  quemsg;
    
    fibo_textTrace("application thread enter");
    
    set_app_ver(app_ver);
    fibo_textTrace("app image at demo");
    if(0 == quid)
    {
        quid = fibo_queue_create(100, sizeof(ota_msg));
    }
    
   if(0 == quid)
   {
       fibo_textTrace("app image ota_demo get msg error!!!");
       goto error;
   }
       
    while (1)
    {
        memset(&quemsg, 0, sizeof(quemsg));
        ret = fibo_queue_get(quid, (void*)&quemsg, 5000);
        fibo_textTrace("app image at demo fibo_queue_get ret =%d--op[%d]", ret, quemsg.opetion);

        if(ret < 0)
        {
            continue;
        }


        fibo_textTrace("app image ota demo %d--op[%d]", count++, quemsg.opetion);

        switch(quemsg.opetion)
        {
            case E_ACTIVE:
                {
                    demo_active_pdp();
                    break;
                }
  

            case E_SEND_AT:
                {
                    demo_send_at_cereg();
                    fibo_taskSleep(2000);
                    demo_send_at();
                    break;
                }

            default:
                {
                    break;
                }
        }
    }
    
error:
    
}



void *appimg_enter(void *param)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_enter_1, P_INFO, "application appimg_enter");
    fibo_textTrace("app image enter");
    fibo_thread_create(at_demo, (INT8 *)"at_task", 16*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_exit_1, P_INFO, "application image exit");
    return;
}
