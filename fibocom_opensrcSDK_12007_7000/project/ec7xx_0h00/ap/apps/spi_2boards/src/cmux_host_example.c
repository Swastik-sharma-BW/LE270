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

#include "sfdt.h"

#include "cmux.h"
#include "cmux_chat.h"

#define CMUXHOST_TASK_STACK_SIZE        (1024*6)

#if defined FEATURE_FREERTOS_ENABLE
static StaticTask_t             cmuxhost_task;
static uint8_t                  cmuxhost_task_stack[CMUXHOST_TASK_STACK_SIZE];
#endif

static struct modem_chat_data test_at1_cmd[] =
{
    {"AT",                   MODEM_CHAT_RESP_OK,              3, 5, FALSE},
    {"AT+ECPMUCFG?",         MODEM_CHAT_RESP_OK,              3, 5, FALSE},
};

static struct modem_chat_data test_at2_cmd[] =
{
    {"AT+CEREG?",            MODEM_CHAT_RESP_OK,              3, 5, FALSE},
    {"AT+CGSN=1",            MODEM_CHAT_RESP_OK,              3, 5, FALSE},
};

static struct modem_chat_data test_opaq_cmd[] =
{
    {"AABBCCDDEE",           MODEM_CHAT_RESP_NOT_NEED,        1, 0, TRUE},
    {"ABCEDEFGHIJ",          MODEM_CHAT_RESP_NOT_NEED,        1, 0, TRUE},
};


static err_t cmux_test_start(struct cmux *obj)
{
    err_t result = 0;
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_start_1, P_VALUE, "open AT/ppp/opaq PORT.");
    result = cmux_open(obj, CMUX_AT_PORT);
    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_start_2, P_VALUE, "open AT port failed.");
        goto _end;
    }

    result = cmux_open(obj, CMUX_PPP_PORT);
    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_start_3, P_VALUE, "open PPP port failed.");
        goto _end;
    }

    result = cmux_open(obj, CMUX_OPAQ_PORT);
    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_start_4, P_VALUE, "open OPAQ port failed.");
        goto _end;
    }

    result = modem_chat(obj, CMUX_AT_PORT, test_at1_cmd, sizeof(test_at1_cmd) / sizeof(test_at1_cmd[0]));
    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_start_5, P_VALUE, "test AT failed.");
        goto _end;
    }

    result = modem_chat(obj, CMUX_PPP_PORT, test_at2_cmd, sizeof(test_at2_cmd) / sizeof(test_at2_cmd[0]));
    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_at_6, P_VALUE, "test PPP failed.");
        goto _end;
    }
    result = modem_chat(obj, CMUX_OPAQ_PORT, test_opaq_cmd, sizeof(test_opaq_cmd) / sizeof(test_opaq_cmd[0]));
    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_at_7, P_VALUE, "test OPAQ failed.");
        goto _end;
    }
_end:
    return result;
}

static err_t cmux_test_stop(struct cmux *obj)
{
    err_t result = 0;
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_stop_1, P_VALUE, "stop AT/ppp/opaq PORT.");

    result = cmux_detach(CMUX_AT_PORT);

    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_stop_2, P_VALUE, "cmux stop failed.");
        goto _end;
    }

    result = cmux_detach(CMUX_PPP_PORT);

    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_stop_2, P_VALUE, "cmux stop failed.");
        goto _end;
    }

    result = cmux_detach(CMUX_OPAQ_PORT);

    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_test_stop_2, P_VALUE, "cmux stop failed.");
        goto _end;
    }

_end:
    return result;
}


const struct cmux_ops my_cmux_ops =
{
    cmux_test_start,
    cmux_test_stop,
    NULL
};

static struct cmux *ecMux = NULL;


static void CmuxHostTask(void *arg)
{
    err_t result = CMUX_OK;
    osDelay(2000);

    SFDT_masterInit();

    ecMux = malloc(sizeof(struct cmux));
    memset(ecMux, 0, sizeof(struct cmux));
    ecMux->ops = &my_cmux_ops;

    cmux_init(ecMux, CMUX_PORT_NUMBER, NULL);
    cmux_attach(ecMux, CMUX_AT_PORT);
    cmux_attach(ecMux, CMUX_PPP_PORT);
    cmux_attach(ecMux, CMUX_OPAQ_PORT);

    while(1)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, CmuxHostTask_0, P_VALUE, "loop---");
        if(checkSPITxTrigger())
        {
            result = cmux_start(ecMux);
            if(result != CMUX_OK){
                continue;
            }

            osDelay(2000);
            cmux_stop(ecMux);
        }
        osDelay(10000);
    }
}

void CmuxHostTaskInit(void)
{
    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    task_attr.name = "cmuxhost";

#if defined FEATURE_FREERTOS_ENABLE
    memset(cmuxhost_task_stack, 0xA5,CMUXHOST_TASK_STACK_SIZE);
    task_attr.stack_mem = cmuxhost_task_stack;
    task_attr.cb_mem = &cmuxhost_task;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block
    task_attr.priority = osPriorityNormal;
#elif defined FEATURE_LITEOS_ENABLE
    task_attr.priority = osPriorityNormal1;
#endif
    task_attr.stack_size = CMUXHOST_TASK_STACK_SIZE;

    osThreadNew(CmuxHostTask, NULL, &task_attr);
}

