/****************************************************************************
 *
 * Copy right:   2017-, Copyrigths of EigenComm Ltd.
 * File name:    app.c
 * Description:  EC718 at command demo entry source file
 * History:      Rev1.0   2018-07-12
 *
 ****************************************************************************/
#include "string.h"
#include "bsp.h"
#include "bsp_custom.h"
#include "os_common.h"
#include "ostask.h"
#include DEBUG_LOG_HEADER_FILE
#include "plat_config.h"
#include "slpman.h"
#include "app.h"
#ifdef FEATURE_AT_ENABLE
#include "at_def.h"
#include "at_api.h"
#endif
#if defined(FEATURE_CTCC_DM_ENABLE) || defined(FEATURE_CUCC_DM_ENABLE) || defined(FEATURE_CMCC_DM_ENABLE)
#include "dm_task.h"
#endif
#ifdef FEATURE_ANDLINK_ENABLE
#include "andlink_task.h"
#endif
#ifdef FEATURE_APP_TLS_ENABLE
#include "at_ssl_task.h"
#endif
#ifdef FEATURE_PLAT_HTTP_AT_ENABLE
#include "at_http_task.h"
#endif
#ifdef FEATURE_CTWING_CERTI_ENABLE
#include "ctw_task.h"
#endif
#include "version.h"
#include "ffw_os.h"
#include "flash_rt.h"



#if defined(FFW_ENABLE)
#include "ffw_main.h"
#include "ffw_nv.h"
#include "ffw_log.h"
#include "ffw_lpg_logic.h"
#include "ffw_app_start.h"
#include "ffw_result.h"
#include "ffw_sms.h"
#endif
#include "ps_lib_api.h"
#include "ps_mm_if.h"
#include "ps_dev_if.h"

#ifdef FIBOCOM_BASE
#define USRAPP_TASK_STACK_SIZE        (5*1024)
#else
#define USRAPP_TASK_STACK_SIZE        (1024)
#endif

#ifdef CONFIG_FFW_MIFI_ENABLE
#include "sfdt.h"
#endif

#if defined(CONFIG_FFW_CUST_ETHER_ENABLE)
#include "cecs.h"
#endif

#if defined FEATURE_FREERTOS_ENABLE
static StaticTask_t             usrapp_task;
static uint8_t                  usrapp_task_stack[USRAPP_TASK_STACK_SIZE];
#endif

#if defined(FIBOCOM_OPENCPU)
#include"fibo_opencpu.h"
FIBO_CALLBACK_T *g_user_callback = NULL;
extern void *appimg_enter(void *param);
#endif

#include "mem_zone.h"
void fibo_disply_mem()
{
    uint32_t addr = 0;
    uint32_t size = 0;
    uint32_t total = 0;
    
    addr = fibo_get_ap_load_addr();
    size = fibo_get_ap_load_size();
    ECPLAT_PRINTF(UNILOG_PLA_APP, disply_mem_0, P_VALUE, "AP addr:%x, size:%d", addr, size);
    total+=size;
    addr = fibo_get_reserve_region_start();
    size = fibo_get_reserve_region_size();
    ECPLAT_PRINTF(UNILOG_PLA_APP, disply_mem_1, P_VALUE, "RS addr:%x, size:%d", addr, size);
    total+=size;
    addr = fibo_get_fota_region_start();
    size = fibo_get_fota_region_size();
    ECPLAT_PRINTF(UNILOG_PLA_APP, disply_mem_2, P_VALUE, "FOTA addr:%x, size:%d", addr, size);
    total+=size;
    addr = fibo_get_fs_addr();
    size = fibo_get_fs_size();
    ECPLAT_PRINTF(UNILOG_PLA_APP, disply_mem_3, P_VALUE, "FS addr:%x, size:%d", addr, size);
    total+=size;
    ECPLAT_PRINTF(UNILOG_PLA_APP, disply_mem_4, P_VALUE, "AP total size:%d(3518464)", total);
}

#ifdef CONFIG_FFW_MIFI_ENABLE
static uint32_t spiTxTrigger = 0;

void triggerSPITx(void)
{
    spiTxTrigger++;
}

uint32_t checkSPITxTrigger(void)
{
    if(spiTxTrigger)
    {
        spiTxTrigger = 0;
        return 1;
    }

    return 0;
}

static uint32_t flowControl = 0;

void triggerSPIFlowControl(void)
{
    flowControl++;
#ifdef SFDT_MASTER_BOARD
    SFDT_masterFlowControl(flowControl & 0x1);
#else
    SFDT_slaveFlowControl(flowControl & 0x1);
#endif
}

static void sfdtSlaveTask(void *arg)
{
    uint32_t loopCnt = 0;

    SFDT_slaveInit();

    while(1)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, sfdtSlaveTask_0, P_VALUE, "AP: sfdt slave task loop-%d", loopCnt++);
        osDelay(2000);
    }
}

void sfdtSlaveTaskInit(void)
{
    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    task_attr.name = "spislave";

#if defined FEATURE_FREERTOS_ENABLE
    memset(usrapp_task_stack, 0xA5,USRAPP_TASK_STACK_SIZE);
    task_attr.stack_mem = usrapp_task_stack;
    task_attr.cb_mem = &usrapp_task;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block
    task_attr.priority = osPriorityNormal;
#elif defined FEATURE_LITEOS_ENABLE
    task_attr.priority = osPriorityNormal1;
#endif
    task_attr.stack_size = USRAPP_TASK_STACK_SIZE;

    osThreadNew(sfdtSlaveTask, NULL, &task_attr);
}
#endif

#ifdef CONFIG_FFW_CUST_ETHER_ENABLE
static void custEtherTask(void *arg)
{
    uint32_t loopCnt = 0;

    CECS_MasterInit();

    while(1)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, custEtherTask_0, P_VALUE, "AP: cust ether task loop-%d", loopCnt++);
        osDelay(2000);
    }
}

void custEtherTaskInit(void)
{
    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    task_attr.name = "custEther";

#if defined FEATURE_FREERTOS_ENABLE
    memset(usrapp_task_stack, 0xA5,USRAPP_TASK_STACK_SIZE);
    task_attr.stack_mem = usrapp_task_stack;
    task_attr.cb_mem = &usrapp_task;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block
    task_attr.priority = osPriorityNormal;
#elif defined FEATURE_LITEOS_ENABLE
    task_attr.priority = osPriorityNormal1;
#endif
    task_attr.stack_size = USRAPP_TASK_STACK_SIZE;

    osThreadNew(custEtherTask, NULL, &task_attr);
}
#endif

void usb_portmon_task_init(void);

static void UserAppTask(void *arg)
{
    uint32_t loopCnt = 0;
    while(1)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, ApUserAppTask_0, P_VALUE, "AP: Daemon task loop-%d", loopCnt++);
        osDelay(2000);
    }
}

#ifdef FIBOCOM_BASE
#define FLASH_WRITE_GRANT_SIZE   (128)
extern uint8_t FLASH_writeSafe(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t FLASH_appWrite(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    uint32_t numOfWriteTrunk = 0, i=0;
    uint8_t RetValue = 0;

    numOfWriteTrunk = (Size/FLASH_WRITE_GRANT_SIZE);
    //uint32_t mask = 0;
    if(Size <= FLASH_WRITE_GRANT_SIZE)
    {
        RetValue = FLASH_writeSafe(pData, WriteAddr, Size);
        if (RetValue != QSPI_OK)
        {
            ECPLAT_PRINTF(UNILOG_FOTA, FLASH_appWrite_1, P_ERROR, "RetValue1 : %d", RetValue);
            return RetValue;
        }
    }
    else
    {
        for(i=0; i<numOfWriteTrunk; i++)
        {
            osDelay(10);
            RetValue = FLASH_writeSafe(pData+i*FLASH_WRITE_GRANT_SIZE, WriteAddr+i*FLASH_WRITE_GRANT_SIZE, FLASH_WRITE_GRANT_SIZE);
            if (RetValue != QSPI_OK)
            {
                ECPLAT_PRINTF(UNILOG_FOTA, FLASH_appWrite_2, P_ERROR, "RetValue2 : %d", RetValue);
                return RetValue;
            }
        }
        if((Size - i*FLASH_WRITE_GRANT_SIZE) != 0)
        {
            RetValue = FLASH_writeSafe(pData+i*FLASH_WRITE_GRANT_SIZE, WriteAddr+i*FLASH_WRITE_GRANT_SIZE, Size-i*FLASH_WRITE_GRANT_SIZE);
            if (RetValue != QSPI_OK)
            {
                ECPLAT_PRINTF(UNILOG_FOTA, FLASH_appWrite_3, P_ERROR, "RetValue3 : %d", RetValue);
                return RetValue;
            }
        }
    }
    return RetValue;
}
#endif

void daemonTaskInit(void)
{
    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    task_attr.name = "usrapp";
#if defined FEATURE_FREERTOS_ENABLE
    memset(usrapp_task_stack, 0xA5,USRAPP_TASK_STACK_SIZE);
    task_attr.stack_mem = usrapp_task_stack;
    task_attr.cb_mem = &usrapp_task;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block
    task_attr.priority = osPriorityNormal;
#elif defined FEATURE_LITEOS_ENABLE
    task_attr.priority = osPriorityNormal1;
#endif
    task_attr.stack_size = USRAPP_TASK_STACK_SIZE;
#ifdef TYPE_EC718M
    task_attr.reserved = osThreadDynamicStackAlloc;
#endif
    osThreadNew(UserAppTask, NULL, &task_attr);

	
}

#if defined(FFW_ENABLE)
extern uint8_t fibo_get_cur_sim_slot();
extern int fibo_switch_sim_slot(uint8_t simid);
extern void start_mcell_update_periodic();
extern void update_mcell_info_fun();
void set_sim_slot(void)
{
    int32_t val = 0;
    if(ffw_nv_get("SIM_SLOT", &val,1) == FFW_R_SUCCESS && (val == 0 || val == 1))
    {
        if(val != fibo_get_cur_sim_slot())
        {
            fibo_switch_sim_slot(val);
        }
    }
}
void set_locrel(void)
{
    int32_t idleTime = 0;
    CmiPsSetTrafficIdleMonitorReq  setTrafficIdleMonitorReq = {0};
    ffw_nv_get("LOCREL",&idleTime,1);
    if(idleTime != 0)
    {
        setTrafficIdleMonitorReq.bTrafficIdleMonitorTimeSPresent = TRUE;
        setTrafficIdleMonitorReq.bRetryTrafficIdleTimeSPresent = FALSE;
        setTrafficIdleMonitorReq.bEnableTrafficIdleMonitor = 1;
        setTrafficIdleMonitorReq.trafficIdleMonitorTimeS = idleTime;
        setTrafficIdleMonitorReq.retryTrafficIdleMonitorTimeS = 0;
    }
    appSetECSCLKEXSync(&setTrafficIdleMonitorReq);
}
#ifdef CONFIG_CUST_HUAZHIRONG_FEATURE
extern void fibo_init_qcell_info();
void fibo_set_forBTac_cfg()
{
    int ret = 0;
    CmiDevSetExtCfgReq setExtCfgReq = {0};

    setExtCfgReq.clrForbiddenTacPst = TRUE;
    setExtCfgReq.clrForbiddenTac    = TRUE;

    setExtCfgReq.forbTacTimerPst = TRUE;
    setExtCfgReq.forbTacTimerS = 3600;//1h

    ret = devSetExCfgParaSync(&setExtCfgReq);
    FFW_LOG_INFO("set ret %d",ret);
}
#endif
void modem_init_thread()
{
    uint8_t psm_mode;
    set_sim_slot();
    set_locrel();
    mmSetOperFormatSync(0);  //set cops_format

#ifdef FFW_LIUXIN_CONFIG
    //2024-12-09,QYL,MTC0964-31 begin,Close softreset function for LIUXIN.
    CmiDevSetExtCfgReq setExtCfgReq;
    memset(&setExtCfgReq, 0x00, sizeof(CmiDevSetExtCfgReq));
    setExtCfgReq.psSoftResetPresent = TRUE;
    setExtCfgReq.bEnablePsSoftReset = FALSE;
    devSetExCfgParaSync(&setExtCfgReq);
    //2024-12-09,QYL,MTC0964-31 end.
#endif

    appGetPSMModeSync(&psm_mode);
    if (psm_mode == 0)
    {
        ffw_thread_sleep(1000);
        update_mcell_info_fun();
    }
    start_mcell_update_periodic();

    osThreadExit();
}

#endif

#if APP_IMAGE_FLASH_LOAD_SIZE
extern BOOL fibo_full_app_update_judge(void);
extern void fibo_full_app_update_flag_reset(void);
extern INT32 fibo_app_update_begin(void);
#endif

#if defined(FFW_ENABLE)
static void ffw_init_fni(void)
{
    ffw_lpg_start();

#if defined(FIBOCOM_OPENCPU)
    ECPLAT_PRINTF(UNILOG_PLA_APP, EC_CHIP_VERSION_2, P_INFO, "OPENCPU VER.");
#if APP_IMAGE_FLASH_LOAD_SIZE
//2024-11-25, mahongmei, MTC0756-1361, Begin. fix full app update bug.
    if (fibo_full_app_update_judge() && fibo_app_update_begin() >= 0)
//2024-11-25, mahongmei, MTC0756-1361, End        
    {
        FFW_LOG_INFO("[FOTA]app update success!");
        fibo_full_app_update_flag_reset();
        ffw_thread_sleep(500);
        ffw_soft_reset();
    }

    if (NULL == fibo_app_start(0,0,0,0,NULL))
#endif
    {
        FFW_LOG_INFO("run app fail, exe appimg_enter");
        g_user_callback = appimg_enter(NULL);
    }
#endif
#ifdef CONFIG_CUST_KR_FEATURE
    ffw_sms_thread_init();
#endif
}
#endif


static void appInit(void *arg)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, EC_CHIP_VERSION_1, P_INFO, "%s", EC_CHIP_VERSION);
#if defined(FEATURE_CTCC_DM_ENABLE) || defined(FEATURE_CUCC_DM_ENABLE) || defined(FEATURE_CMCC_DM_ENABLE)
    ecAutoRegisterInit();
#endif

#if defined(FEATURE_ANDLINK_ENABLE)
	andlinkAppInit();
#endif

#ifdef FEATURE_CTWING_CERTI_ENABLE
    ecCtwAutoRegisterInit();
#endif

#ifdef FEATURE_APP_TLS_ENABLE
    sslEngineInit();
#endif
#ifdef FEATURE_PLAT_HTTP_AT_ENABLE
    httpEngineInit();
#endif

#ifdef FEATURE_PLAT_CTLWM2M_AT_ENABLE
        extern void  ctiotEngineInit();
        ctiotEngineInit();
#endif
#ifdef FEATURE_PLAT_ONENET_AT_ENABLE
        extern void onenetEngineInit();
        onenetEngineInit();
#endif
#if (RTE_USB_EN == 1)
    if (BSP_UsbGetVBUSMode()==1)
    {
        usb_portmon_task_init() ;
    }
#endif
	
#ifdef FEATURE_PLAT_MISC_ECIDLEP_ENABLE
	extern void apPrintIdlePercent(void);
	apPrintIdlePercent();
#endif

#if defined(FFW_ENABLE)
    ffw_init_gtset();
    ffw_init_async(ffw_init_fni);
    osThreadAttr_t t_task_attr;

    memset(&t_task_attr,0,sizeof(osThreadAttr_t));
    t_task_attr.name = "modem_init";
    t_task_attr.priority = osPriorityNormal;
    t_task_attr.stack_size = 1024*10;

    osThreadNew(modem_init_thread, NULL, &t_task_attr);
#ifdef CONFIG_CUST_HUAZHIRONG_FEATURE
    fibo_init_qcell_info();
    fibo_set_forBTac_cfg();
#endif
#if defined(FIBOCOM_OPENCPU)
    fibo_disply_mem();
#endif
#endif

#ifdef CONFIG_FIBO_DEVCFG
    extern void devcfg_entry(void);
    devcfg_entry();
#endif

#ifdef CONFIG_FFW_MIFI_ENABLE
    sfdtSlaveTaskInit();
#endif

#ifdef CONFIG_FFW_CUST_ETHER_ENABLE
    //custEtherTaskInit(); //Users can open it themselves for open
#endif

}

/**
  \fn          int main_entry(void)
  \brief       main entry function.
  \return
*/
void main_entry(void)
{
    BSP_CommonInit();
    osKernelInitialize();
    registerAppEntry(appInit, NULL);
    if (osKernelGetState() == osKernelReady)
    {
        osKernelStart();
    }
    while(1);

}
