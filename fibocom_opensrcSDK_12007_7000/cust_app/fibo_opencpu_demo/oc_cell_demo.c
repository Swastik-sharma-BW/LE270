#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"

void get_cell_info_rsp(void* param)
{
    opencpu_cellinfo openCellInfo = {0};
    int i = 0;
    memcpy(&openCellInfo, (opencpu_cellinfo *)param, sizeof(opencpu_cellinfo));

    APP_LOG_INFO("%x,%x,%04X,%08X,%X,%X,%d,%d,%d,%d,%d\r\n",
                 openCellInfo.sCellInfo.mcc, openCellInfo.sCellInfo.mnc,
                 openCellInfo.sCellInfo.tac,
                 openCellInfo.sCellInfo.cellId,
                 openCellInfo.sCellInfo.earfcn, openCellInfo.sCellInfo.phyCellId,
                 openCellInfo.sCellInfo.band,openCellInfo.sCellInfo.dlBandWidth,
                 openCellInfo.sCellInfo.snr,openCellInfo.sCellInfo.srxlev,
                 openCellInfo.sCellInfo.rsrp, openCellInfo.sCellInfo.rsrq);
    for(i =0; i<openCellInfo.nCellNum;i++)
    {
        APP_LOG_INFO("%x,%x,%04X,%08X,%X,%X,%d,%d,%d,%d,%d,%d\r\n",
                 openCellInfo.nCellList[i].mcc, openCellInfo.nCellList[i].mnc,
                 openCellInfo.nCellList[i].tac,
                 openCellInfo.nCellList[i].cellId,
                 openCellInfo.nCellList[i].earfcn, openCellInfo.nCellList[i].phyCellId,
                 openCellInfo.sCellInfo.dlBandWidth,
                 openCellInfo.nCellList[i].snr,openCellInfo.nCellList[i].srxlev,
                 openCellInfo.nCellList[i].rsrp, openCellInfo.nCellList[i].rsrq);
    }
}

static void hello_world_demo()
{
    int i = 0;
    int ret = 0;
    cell_info_cfg_t input_param = {0};
    cell_info_cfg_t output_param = {0};

    //set maxtime
    input_param.maxQueryTime = 5;
    ret = fibo_nw_set_config(0, FIBO_NW_CELL_INFO_CFG, &input_param, NULL);
    APP_LOG_INFO("fibo_nw_set_config ret = %d", ret);

    fibo_taskSleep(1000);
    ret = fibo_nw_get_config(0, FIBO_NW_CELL_INFO_CFG, NULL, &output_param);
    APP_LOG_INFO("fibo_nw_get_config ret = %d,output_param:%d,maxQueryTime:%d", ret,output_param.cache_mode,output_param.maxQueryTime);
    for (i = 0; i < 10; i++)
    {
        APP_LOG_INFO("Hello world %d", i);
        fibo_taskSleep(4000);
    }
    ret = fibo_cell_info_get(get_cell_info_rsp,0);

    //cache mode
    input_param.cache_mode = true;
    ret = fibo_nw_set_config(0, FIBO_NW_CELL_INFO_CFG, &input_param, NULL);
    APP_LOG_INFO("fibo_nw_set_config ret = %d", ret);

    fibo_taskSleep(1000);
    ret = fibo_nw_get_config(0, FIBO_NW_CELL_INFO_CFG, NULL, &output_param);
    APP_LOG_INFO("fibo_nw_get_config ret = %d,output_param:%d,maxQueryTime:%d", ret,output_param.cache_mode,output_param.maxQueryTime);
    for (i = 0; i < 10; i++)
    {
        APP_LOG_INFO("Hello world %d", i);
        fibo_taskSleep(4000);
        ret = fibo_cell_info_get(get_cell_info_rsp,0);
    }

    return ;
}


void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    return app_init(hello_world_demo);
}
