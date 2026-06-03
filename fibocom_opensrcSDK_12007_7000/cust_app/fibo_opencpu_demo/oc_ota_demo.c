#include "app_image.h"
#include "fibo_opencpu.h"

UINT32 quid = 0;


typedef enum  e_option
{
    E_ACTIVE = 0x01,
    E_OTA_DL_UP,
    E_OTA,
    E_OTA_HD,
    E_OTA_HD_NR_UP,
    O_OTA_NUM
    
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
            
        case 2: //dl up ftp
            msg.opetion = E_OTA_DL_UP;
            fibo_queue_put(quid,&msg,10);
            break;
            
        case 3: //ota  http
            msg.opetion = E_OTA;
            fibo_queue_put(quid,&msg,10);
            break;   
        case 4: //dl up  http
            msg.opetion = E_OTA_HD;
            fibo_queue_put(quid,&msg,10);
            break;  
            
         case 5: //dl up ftp
            msg.opetion = E_OTA_HD_NR_UP;
            fibo_queue_put(quid,&msg,10);
            break;   
            
        default:
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
#if 0
static void hello_world_demo()
{
	int count = 0;
    int ret = 0;
    fibo_textTrace("application thread enter");
    set_app_ver(app_ver);
    for (int n = 0; n < 10; n++)
    {
        fibo_textTrace("app image hello world %d", n);
        fibo_taskSleep(500);
    }
    
    while (1)
    {
        fibo_textTrace("app image hello_world_demo %d", count++);
        fibo_taskSleep(5000);
        if (0 == ota_switch)
        {
            ret = fibo_firmware_ota(0,(UINT8 *)"http://xatest.fibocom.com:8000/httpfota/test1/chaixq/temp/ota.par",NULL,NULL,NULL);
        }
        else if (1 == ota_switch)
        {
            ret = fibo_firmware_ota(1,(UINT8 *)"ftp://47.110.234.36",(UINT8 *)"/FOTA/LE130/ota.par",(UINT8 *)"ftpuser4",(UINT8 *)"test");
        }
        fibo_textTrace("OTA ota_switch = %d--ret = %d.",ota_switch, ret);
        ota_switch = 0xFF;
    }

    
}
#endif

char g_ota_data[100*1024] = {0};
UINT32 g_ota_len  = 0;

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

void demo_data_malloc()
{
    char *p = NULL;
    p = fibo_malloc(32*1024);
    fibo_textTrace("demo_data_malloc %x", p);
    fibo_free(p);
}

void demo_recv_data_display(UINT32 recv, UINT32 total)
{
    fibo_textTrace("recv %d, total =%d",recv, total);
    g_ota_len = total;
    return;
}


int demo_app_ota()
{
    int ret = -1;
    fibo_firmware_option_set(2, NULL);
    ret = fibo_firmware_ota(0,(UINT8 *)"http://111.231.250.105/le270/default_delta.par",NULL,NULL,NULL);
    return ret;
}


int demo_app_full_update()
{
    int ret = -1;
    fibo_firmware_option_set(1, NULL);
    ret = fibo_firmware_ota(0,(UINT8 *)"http://111.231.250.105/le270/FOTA/app2.bin",NULL,NULL,NULL);    
    return ret;
}

int demo_frm_ota()
{
    int ret = -1;
    fibo_firmware_option_set(0, NULL);
    ret = fibo_firmware_ota(0,(UINT8 *)"http://xatest.fibocom.com:8000/httpfota/test1/chaixq/temp/ota.par",NULL,NULL,NULL);
    return ret;
}

int demo_frm_ota_dl_up()
{
    int ret = -1;
    
    fibo_firmware_option_set(0, NULL);
    ret = fibo_firmware_dl(1,(UINT8 *)"ftp://47.110.234.36",(UINT8 *)"/FOTA/LE130/ota.par",(UINT8 *)"ftpuser4",(UINT8 *)"test");


    if(0 == ret)
    {
        ret = fibo_firmware_update();
    }
    return ret;
}

int demo_frm_ota_hd()
{
    int   ret = -1; 
    fibo_firmware_option_set(0, NULL);
    fibo_set_data_buffer(g_ota_data);
    fibo_ota_set_callback(demo_recv_data_display);
    ret = fibo_firmware_dl(0,(UINT8 *)"http://xatest.fibocom.com:8000/httpfota/test1/chaixq/temp/ota.par",NULL,NULL,NULL);
    fibo_textTrace("DOTA application thread enter ret = %d", ret);
    if(0 == ret)
    {
        ret = fibo_firmware_handle((INT8 *)g_ota_data, g_ota_len);
    }
    return ret;
}

int demo_frm_ota_hd_nr_up()
{
    int   ret = -1; 
    fibo_set_data_buffer(g_ota_data);
    fibo_ota_set_callback(demo_recv_data_display);
    ret = fibo_firmware_dl(1,(UINT8 *)"ftp://47.110.234.36",(UINT8 *)"/FOTA/LE130/ota.par",(UINT8 *)"ftpuser4",(UINT8 *)"test");
    fibo_textTrace("DOTA application thread enter ret = %d", ret);
    if(0 == ret)
    {
        ret = fibo_firmware_handle_no_reboot((INT8 *)g_ota_data,g_ota_len);
    }
    
    /*
    if(0 == ret)
    {
        ret = fibo_firmware_update();
    }
    */
    
    return ret;
}



static void ota_demo()
{
	int count = 0;
	int ret = -1;
    ota_msg  quemsg;
    
    fibo_textTrace("application thread enter");
    
    set_app_ver(app_ver);
    fibo_textTrace("app image ota");
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
        fibo_textTrace("app image ota demo fibo_queue_get ret =%d--op[%d]", ret, quemsg.opetion);

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
            case E_OTA_DL_UP:
                {
                    demo_frm_ota_dl_up();
                    break;
                }
            case E_OTA:
                {
                    demo_frm_ota();
                    break;
                }
            case E_OTA_HD:
                {
                    demo_frm_ota_hd();
                    break;
                }
            case E_OTA_HD_NR_UP:
                {
                    demo_frm_ota_hd_nr_up();
                    break;
                }
            
            default:
                {
                    //TODO
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
    fibo_thread_create(ota_demo, (INT8 *)"ota_task", 10*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_exit_1, P_INFO, "application image exit");
    return;
}
