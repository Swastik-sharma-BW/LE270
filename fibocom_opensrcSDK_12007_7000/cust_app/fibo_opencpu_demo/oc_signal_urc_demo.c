#include <stdio.h>
#include "fibo_opencpu_api.h"
#include "fibo_opencpu_comm.h"



#include "fibo_opencpu_api.h"

static uint8_t get_mcc_mnc_from_plmn(uint8_t *plmn, uint8_t *mcc, uint8_t *mnc)
{
    if (plmn == NULL || mcc == NULL || mnc == NULL)
    {
        return 0;
    }
    mcc[0] = plmn[0] & 0x0F;
    mcc[1] = plmn[0] >> 4;
    mcc[2] = plmn[1] & 0x0F;

    mnc[0] = plmn[2] & 0x0F;
    mnc[1] = plmn[2] >> 4;
    mnc[2] = plmn[1] >> 4;

    if (mnc[2] > 9)
    {
        mnc[2] = 0;
        return 2;
    }
    else
    {
        return 3;
    }
}



static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    fibo_textTrace("sig_res_callback  sig = %d (200 is opencpu test.)", sig);
	 switch (sig)
    {
        case GAPP_SIG_NW_SIGNAL_QUALITY_IND:
		{
			UINT8 nSim = (UINT8)va_arg(arg, int);
            INT16 rsrp = (INT16)va_arg(arg, int);
            INT16 rsrq = (INT16)va_arg(arg, int);
            INT16 sinr = (INT16)va_arg(arg, int);
            UINT8 plmn[3] = {0};
            plmn[0] = (UINT8)va_arg(arg, int);
            plmn[1] = (UINT8)va_arg(arg, int);
            plmn[2] = (UINT8)va_arg(arg, int);
            UINT16 tac = (UINT16)va_arg(arg, int);
            UINT32 cellid = (UINT32)va_arg(arg, uint32_t);
			
			fibo_textTrace("[%s-%d]sim_id = %d,rsrp = %d,rsrq = %d,sinr = %d", __FUNCTION__, __LINE__,nSim,rsrp,rsrq,sinr);

            uint8_t mcc[3] = {0};
            uint8_t mnc[3] = {0};
            get_mcc_mnc_from_plmn(&plmn[0], &mcc[0], &mnc[0]);
            fibo_textTrace("MCC= %d%d%d,MNC= %d%d,tac = %X,cellid = %X",
            mcc[0],mcc[1],mcc[2],mnc[0],mnc[1],
            tac,
            cellid);
            va_end(arg);
		}
		break;
		default:
        break;
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
    

    for (int n = 0; n < 10; n++)
    {
        fibo_textTrace("hello world %d", n);
        fibo_taskSleep(1000);
    }

    fibo_nw_config_signal_quality_urc_t config_signal_quality_urc = {0};
    //config_signal_quality_urc.config = FIBO_SIGNAL_QUALITY_URC_DEFAULT;
	config_signal_quality_urc.config = FIBO_SIGNAL_QUALITY_URC_CONFIG1;
	config_signal_quality_urc.rsrp_range = 5;
	
    //config_signal_quality_urc.config = FIBO_SIGNAL_QUALITY_URC_CONFIG2;
	//config_signal_quality_urc.sinr_range = 5;

    int ret = fibo_send_cmd(FIBO_CMD_SET_SIGNAL_QUALITY_URC,&config_signal_quality_urc,0,NULL,NULL);
	while (1)
    {
        fibo_textTrace("fibo_send_cmd ret %d",ret);
        fibo_taskSleep(1000);
    }


}


FIBO_CALLBACK_T* fibo_main(void)
{	

    //init
    fibo_thread_create(hello_world_demo, (INT8*)"hello_world_demo", 1024*3, NULL, OSI_PRIORITY_NORMAL);
    return &user_callback;

}
