#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"

void cellinfo_refponse_cb(void* param)
{
    opencpu_cellinfo cellinfo = {0};
    memcpy(&cellinfo,param,sizeof(opencpu_cellinfo));
    APP_LOG_INFO("%x,%x,%04X,%08X,%X,%X,%d,%d,%d,%d,%d,%d\r\n",
                 cellinfo.sCellInfo.mcc, cellinfo.sCellInfo.mnc,
                 cellinfo.sCellInfo.tac,
                 cellinfo.sCellInfo.cellId,
                 cellinfo.sCellInfo.earfcn, cellinfo.sCellInfo.phyCellId,
                 cellinfo.sCellInfo.band,cellinfo.sCellInfo.dlBandWidth,
                 cellinfo.sCellInfo.snr,cellinfo.sCellInfo.srxlev,
                 cellinfo.sCellInfo.rsrp, cellinfo.sCellInfo.rsrq);
}

static void hello_world_demo()
{
    int ret = 0;
    fibo_taskSleep(5000);

    //get reg info
    reg_info_t reg_info = {0};
    ret = fibo_reg_info_get(&reg_info, 0);
    APP_LOG_INFO("fibo_reg_info_get ret = %d,curr_rat = %d,nStatus = %d,tac = %d,cellid = %d,", 
                ret,reg_info.curr_rat,reg_info.nStatus,reg_info.lte_scell_info.tac,reg_info.lte_scell_info.cell_id);

    //active pdp
    fibo_pdp_profile_t pdp_profile = {0};
    pdp_profile.cid = 2;
    strcpy(pdp_profile.apn,"CMNET");
    strcpy(pdp_profile.nPdpType,"IPV4V6");
    pdp_profile.nAuthProt = 1;
    pdp_profile.apnUserSize = 3;
    strncpy(pdp_profile.apnUser,"123",pdp_profile.apnUser);

    pdp_profile.apnPwdSize= 3;
    strncpy(pdp_profile.apnPwd,"321",pdp_profile.apnPwdSize);
    
    ret = fibo_pdp_active(1, &pdp_profile, 0);
    APP_LOG_INFO("fibo_pdp_active ret = %d", ret);

    //release pdp
    ret = fibo_pdp_release(0, 2, 0);
    APP_LOG_INFO("fibo_pdp_release ret = %d", ret);

    //get pdp status
    char ip[123] = {0};
    UINT8 cid_status = 0;
    fibo_cfun_set(4,0);
    uint8_t pdptype = 3;//ipv4v6
    uint8_t nAuthProt = 0
    ret = fibo_initial_pdp_set(1, pdptype, "CMNET", nAuthProt,NULL,NULL, 0);
    APP_LOG_INFO("fibo_initial_pdp_set ret = %d", ret);
    fibo_cfun_set(1,0);

    //get cell info.
    ret = fibo_cell_info_get(cellinfo_refponse_cb,0);
    APP_LOG_INFO("fibo_local_time_set ret = %d", ret);

    //get csq.
    INT32 rssi = 0;
    INT32 ber = 0;
    ret = fibo_get_csq(&rssi, &ber,0);
    APP_LOG_INFO("fibo_get_csq ret = %d,rssi = %d,ber = %d", ret,rssi,ber);

    //get utc time.
    INT32 utc_time = 0;
    fibo_utctime_get(&utc_time);
    APP_LOG_INFO("fibo_local_time_set utc_time = %d", utc_time);

    //local release set
    ret = fibo_locrel_set(3,0)
    APP_LOG_INFO("fibo_locrel_set ret = %d", ret);

    pdp_profile_t pdpctx  = {0};
    ret = fibo_get_pdp_ctx(1, &pdpctx); 
    APP_LOG_INFO("fibo_get_pdp_ctx ret = %d,active_state:%d,apn:%s,ip type:%s,ip addr:%s", ret,
                pdpctx.active_state,pdpctx.apn,pdpctx.nPdpType,pdpctx.pdpAddr);

    return ;
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    return app_init(hello_world_demo);
}

