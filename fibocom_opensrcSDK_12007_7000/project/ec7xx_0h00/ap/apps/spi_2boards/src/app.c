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
#include "slpman.h"
#include "app.h"
#ifdef FEATURE_AT_ENABLE
#include "at_def.h"
#include "at_api.h"
#endif
#if defined(FEATURE_CTCC_DM_ENABLE) || defined(FEATURE_CUCC_DM_ENABLE) || defined(FEATURE_CMCC_DM_ENABLE)
#include "dm_task.h"
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
#include "sfdt.h"

#ifdef FEATURE_WIFI_SWITCH_ENABLE
#include "switchWifi.h"
#endif
#define USRAPP_TASK_STACK_SIZE        (1024)

#if defined FEATURE_FREERTOS_ENABLE
static StaticTask_t             usrapp_task;
static uint8_t                  usrapp_task_stack[USRAPP_TASK_STACK_SIZE];
#endif


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
#ifdef FEATURE_WIFI_SWITCH_ENABLE
    sfdt_switchWifiInit();
#endif

    while(1)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, ApUserAppTask_0, P_VALUE, "AP: sfdt slave task loop-%d", loopCnt++);
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


static void appInit(void *arg)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, EC_CHIP_VERSION_1, P_INFO, "%s", EC_CHIP_VERSION);
#if defined(FEATURE_CTCC_DM_ENABLE) || defined(FEATURE_CUCC_DM_ENABLE) || defined(FEATURE_CMCC_DM_ENABLE)
    ecAutoRegisterInit();
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
        void usb_portmon_task_init(void);
        usb_portmon_task_init() ;
    }
#endif

// #undef in makefile to select sfdt slave function
#ifdef SFDT_MASTER_BOARD
#define CMUX_HOST_EXAMPLE 0
#define MASTER_LOOP_BACK  1

    uint32_t demoSelect = CMUX_HOST_EXAMPLE;

    if(demoSelect == CMUX_HOST_EXAMPLE)
    {
        CmuxHostTaskInit();
    }
    else
    {
        loopBackTaskInit();
    }
#else
    sfdtSlaveTaskInit();
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
