#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"



/*get network roaming status*/
int fibo_app_get_nw_roaming_status(uint8_t simId, uint8_t *roaming_status)
{
    return fibo_nw_get_config(simId, FIBO_NW_ROAMING_STATUS, NULL, roaming_status);
}


/*test demo: roaming status*/
void fibo_test_demo_roaming_status(void)
{
    uint8_t roaming_status = 0;
    int ret = 0;


    ret = fibo_app_get_nw_roaming_status(0, &roaming_status);
    if (0 != ret)
    {
        APP_LOG_INFO("FIBO APP TEST: %s error %d", __FUNCTION__, ret);
        return;
    }

    if (1 == roaming_status)
    {
        APP_LOG_INFO("FIBO APP TEST: %s camping network: home network", __FUNCTION__); //home network
    }
    else if (5 == roaming_status)
    {
        APP_LOG_INFO("FIBO APP TEST: %s camping network: roaming network", __FUNCTION__); //roaming network
    }
    else
    {
        APP_LOG_INFO("FIBO APP TEST: %s camping network: no registered", __FUNCTION__); //no registered;
    }
}


static void hello_world_demo()
{
    fibo_textTrace("application thread enter");
	
	for (int n = 0; n < 15; n++)
    {
        APP_LOG_INFO("app image hello world %d", n);

        fibo_taskSleep(1000);
    }
    fibo_taskSleep(3*1000);
    reg_info_t reg_info;

    while(1)
    {
        fibo_reg_info_get(&reg_info, 0);
        if (reg_info.nStatus == 1)
            break;
        fibo_taskSleep(1*1000);
        fibo_textTrace("FIBO APP TEST fibo_reg_info_get reg_info.nStatus:%d",reg_info.nStatus);
    }
	fibo_test_demo_roaming_status();


}



void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter11");
    return app_init(hello_world_demo);

}
