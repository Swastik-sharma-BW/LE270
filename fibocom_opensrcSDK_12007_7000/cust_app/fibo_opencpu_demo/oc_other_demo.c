#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"

char g_imsi[24] = {0};
void get_imsi(UINT8 *imsi)
{
    memcpy(g_imsi,imsi,strlen((char *)imsi))
    APP_LOG_INFO("get_imsi = %s", g_imsi);    
}
int CustomizeHandleFun()
{
    return 1;
}
static void hello_world_demo()
{
    int ret = 0;
    fibo_taskSleep(5000);

    //get sn
    char deviceSN[48] = {0};
    ret = fibo_get_sn(deviceSN, 0);
    APP_LOG_INFO("fibo_get_sn deviceSN = %s", deviceSN);

    //get imei
    char imei[48] = {0};
    ret = fibo_get_imei(imei, 0);
    APP_LOG_INFO("fibo_get_imei imei = %s", imei);

    //set app version
    char ver[] = "APP_VER_001";
    ret = fibo_app_version_set(ver);
    APP_LOG_INFO("fibo_app_version_set ret = %d", ret);

    //get app version
    char outstr[48] = {0};
    ret = fibo_app_version_get (outstr, strlen(ver));
    APP_LOG_INFO("fibo_app_version_get outstr = %s", outstr);

    //set local time.
    local_time_t time= {0};
    time.year = 2025;
    time.month = 1;
    time.wDay = 13;
    time.hour = 10;
    time.min = 20;
    time.sec = 10;
    time.tz = 32;
    ret = fibo_local_time_set(&time);
    APP_LOG_INFO("fibo_local_time_set ret = %d", ret);

    //get local time.
    local_time_t get_time= {0};
    ret = fibo_local_time_get(&get_time);
    if(ret == 0)
        APP_LOG_INFO("fibo_local_time_get: \"%02d/%02d/%02d,%02d:%02d:%02d %02d,%d\"", 
                    get_time.year, get_time.month,get_time.day,get_time.hour, get_time.min,
                    get_time.sec, get_time.wDay,get_time.tz);

    //get utc time.
    INT32 utc_time = 0;
    fibo_utctime_get(&utc_time);
    APP_LOG_INFO("fibo_local_time_set utc_time = %d", utc_time);

    //non-signaling test
    fibo_cfun_set(0,0);//enter non-signaling mode
    ret =  fibo_set_mcal_dev(1,0);//open non-signaling test
    APP_LOG_INFO("fibo_set_mcal_dev open non-signaling test ret = %d", ret);

    if(ret == 0)
    {
        CFW_NW_NST_TX_PARA_CONFIG nst_para_config = {0};
        nst_para_config.band = 0x1;//band 1
        nst_para_config.carrier_freq = 0x4c2c;
        nst_para_config.tx_power = 0x78;
        nst_para_config.tx_mode = 1;//0:send only once,1:Periodic send
        nst_para_config.sig_type = 0xff;
        nst_para_config.mcs_index =  0xf;
        nst_para_config.rb_start_idx = 0;
        nst_para_config.rb_num = 1;
        ret = fibo_set_mcal (&nst_para_config, 0)
        APP_LOG_INFO("fibo_set_mcal ret = %d", ret);
    }

    ret =  fibo_set_mcal_dev(0,0);//close non-signaling test 
    APP_LOG_INFO("fibo_set_mcal_dev close non-signaling test ret = %d", ret);
    fibo_cfun_set(15,0);//reboot device,exit the non-signaling test mode

    //open sim detect function
    ret = fibo_sim_detect_switch(1);
    APP_LOG_INFO("fibo_sim_detect_switch ret = %d", ret);

    //get imsi
    ret = fibo_get_imsi_asyn(get_imsi,0);
    APP_LOG_INFO("fibo_get_imsi_asyn ret = %d", ret);

    //get iccid
    char iccid[24] = {0};
    ret = fibo_get_iccid(iccid,0);
    APP_LOG_INFO("fibo_get_iccid ret = %d,iccid = %s", ret,iccid);

    //set sim slot  
    ret = fibo_set_sim_slot(1);
    APP_LOG_INFO("fibo_set_sim_slot ret = %d", ret);

    uint8_t sim_slot = fibo_get_sim_slot();
    APP_LOG_INFO("fibo_get_sim_slot sim_slot = %d", sim_slot);

    //customize app register
    ret = fibo_set_customize_app_register(CustomizeHandleFun);
    APP_LOG_INFO("fibo_set_customize_app_register ret = %d,", ret);
    
    //get sim status
    uint8_t pucSimStatus = 0;
    ret = fibo_get_sim_status_v2(&pucSimStatus,0);
    APP_LOG_INFO("fibo_get_sim_status_v2 ret = %d,pucSimStatus:%d", ret,pucSimStatus);

    //get sim pin status
    int state = 0;
    unsigned pinremaintries = 0;
    unsigned pukremaintries = 0;
    ret = fibo_sim_pin_status(&state, &pinremaintries,&pukremaintries,0);
    APP_LOG_INFO("fibo_sim_pin_status ret = %d,state:%d,pinremaintries:%d,pukremaintries:%d", ret,state,pinremaintries,pukremaintries);

    //close lpg
    ret =  fibo_lpg_switch(0);
    APP_LOG_INFO("fibo_lpg_switch ret = %d:%d", ret);

    return ;
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    return app_init(hello_world_demo);
}
