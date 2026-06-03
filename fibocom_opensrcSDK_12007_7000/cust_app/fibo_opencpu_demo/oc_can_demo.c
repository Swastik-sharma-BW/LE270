#include "app_image.h"
#include "fibo_opencpu.h"
#include "app.h"
#include "app_pdp.h"

#define OSI_LOG_TAG OSI_MAKE_LOG_TAG('M', 'Y', 'A', 'P')

#define CAN_TX_PIN  145
#define CAN_RX_PIN  146

void can_recv(f_can_frame_t *pFrame)
{
    APP_LOG_INFO("can api recv id:%x, dlc:%d", pFrame->id,pFrame->dlc);
    for(uint32_t i =0; i < pFrame->dlc; i++)
    {
	APP_LOG_INFO("%2x ",pFrame->data[i]);
    }
    
}

f_can_frame_t send_fram1 = 
{
   .id = 0x10,
   .data = {1,2,3,4,5,6},
   .dlc =6,
};

f_can_frame_t send_fram2 =
{
   .id = 0x11,
   .data = {1,2,3,4,5,6,7},
   .dlc = 7,
};

f_can_frame_t send_fram3 =
{
   .id = 0x12,
   .data = {1,2,3,4,5,6,7,8},
   .dlc = 8,
};

f_can_frame_t send_fram4 =
{
   .id = 0x12345,
   .data = {1,2,3,4,5,6,7,8},
   .dlc = 4,
   .bis_ext_or_stand = 1, //1:ARM_CAN_EXTENDED_ID 0:ARM_CAN_STANDARD_ID
};

static void can_demo()
{
    int ret = -1;
    set_app_ver(app_ver);

    for (int n = 0; n < 5; n++)
    {
        APP_LOG_INFO("app image hello world %d", n);
        fibo_taskSleep(500);
    }

    fibo_pmu_setlevel(FIBO_POWER_NORMAL_GPIO, FIBO_IOVOLT_3_20V);
    fibo_gpio_mode_set(CAN_TX_PIN,7); //Pin145 mapping CAN TX
    fibo_gpio_mode_set(CAN_RX_PIN,7);//Pin146 mapping CAN RX
    f_can_config_t drvcfg = {0};
    memset(&drvcfg,0,sizeof(drvcfg));
    void *arg = NULL;

    drvcfg.can_port = 0;
    drvcfg.can_mode = 1;
    drvcfg.bit_rate_mode = 0;
    drvcfg.bit_rate = 125*1000;

    /******set can filter mode:1 (no filter)******/
    drvcfg.filter_operation =1; //no filter

    /******set can filter mode:0 (filter exact id)******/
    //drvcfg.filter_operation = 0;  //filter exact id
    //drvcfg.can_id = 0x99;         //accept id reg
    //drvcfg.bis_ext_or_stand = 0;  //filter canid type(1:CAN_EXTENDED_ID 0:CAN_STANDARD_ID)

    /******set can filter mode:2 (filter maskable id)******/
    //drvcfg.filter_operation = 2;  //filter exact id
    //drvcfg.can_id = 0x1230;         //accept id reg
    //drvcfg.mask_id = 0x1FFFFFF8;      //mask id reg
    //drvcfg.bis_ext_or_stand = 1;  //filter canid type(1:CAN_EXTENDED_ID 0:CAN_STANDARD_ID)

    ret = fibo_can_init(0, &drvcfg, can_recv, arg);
    APP_LOG_INFO("can init1 ret: %d", ret);
    ret = fibo_can_init(0, &drvcfg, can_recv, arg);  //Abnormal test, expected return -1
    APP_LOG_INFO("can init2 ret: %d", ret);
    fibo_taskSleep(2000);

    ret = fibo_can_deinit(0);
    APP_LOG_INFO("can deint ret: %d", ret);

    ret=fibo_can_send(0,&send_fram1);
    APP_LOG_INFO("send_fram1:ret=%d",ret);
    ret = fibo_can_init(0, &drvcfg, can_recv, arg);
    APP_LOG_INFO("can init3 ret: %d", ret);

    APP_LOG_INFO("can test send start");
    for(int n = 0; n < 3; n++) {
        ret=fibo_can_send(0,&send_fram1);
        APP_LOG_INFO("send_fram1:ret=%d",ret);
        fibo_taskSleep(1000);
        ret=fibo_can_send(0,&send_fram2);
        APP_LOG_INFO("send_fram2:ret=%d",ret);
        fibo_taskSleep(1000);
        ret=fibo_can_send(0,&send_fram3);
        APP_LOG_INFO("send_fram3:ret=%d",ret);
        fibo_taskSleep(1000);
        ret=fibo_can_send(0,&send_fram4);
        APP_LOG_INFO("send_fram4:ret=%d",ret);
        fibo_taskSleep(1000);
    }
    APP_LOG_INFO("can test send end");

    while(1)
    {
        fibo_taskSleep(2000);
    }

}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application image enter, param 0x%x", param);
    fibo_thread_create(can_demo, (INT8 *)"canthread", 8*1024, NULL, OSI_PRIORITY_NORMAL);
    return 0; 
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
    return;
}

