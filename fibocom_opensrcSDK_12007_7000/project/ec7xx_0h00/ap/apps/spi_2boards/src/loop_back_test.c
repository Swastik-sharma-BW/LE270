/****************************************************************************
 *
 * Copy right:   2017-, Copyrigths of EigenComm Ltd.
 * File name:    app.c
 * Description:  EC718 at command demo entry source file
 * History:      Rev1.0   2018-07-12
 *
 ****************************************************************************/
#include "os_common.h"
#include "ostask.h"
#include DEBUG_LOG_HEADER_FILE
#include "app.h"
#include "sfdt.h"

#define TASK_STACK_SIZE        (1024)

#if defined FEATURE_FREERTOS_ENABLE
static StaticTask_t             tx_task;
static uint8_t                  tx_task_stack[TASK_STACK_SIZE];

static StaticTask_t             rx_task;
static uint8_t                  rx_task_stack[TASK_STACK_SIZE];
#endif

typedef struct
{
    uint16_t seqNo;
    uint16_t txNum;
} Header_t;

static void txTask(void *arg)
{
    uint32_t loopCnt = 0;

    uint8_t* txBuffer = NULL;
    uint32_t txNum;

    Header_t header = {0};

    while(1)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, txTask, P_VALUE, "AP: sfdt master tx task loop-%d", loopCnt++);

        if(checkSPITxTrigger())
        {
            txNum = loopCnt % 1024 + sizeof(Header_t);

            txBuffer = SFDT_mallocTxFrameBuffer(txNum);

            if(txBuffer)
            {
                header.seqNo++;
                header.txNum = txNum;

                memcpy(txBuffer, &header, sizeof(Header_t));

                for(uint32_t i = sizeof(Header_t); i < txNum; i++)
                {
                    txBuffer[i] = i;
                }

                SFDT_masterSend(txBuffer, txNum);

            }
        }

        osDelay(500);
    }
}


static void rxTask(void *arg)
{
    uint32_t loopCnt = 0;

    uint16_t seqNo = 1;
    uint8_t *rxPtr = NULL;
    uint32_t rxLen = 0;
    uint32_t flag = 0;

    Header_t* headerPtr;

    while(1)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, rxTask, P_VALUE, "AP: sfdt master rx task loop-%d", loopCnt++);

        SFDT_masterReceive(&rxPtr, &rxLen, &flag);

        if(rxLen >= sizeof(Header_t))
        {
            headerPtr = (Header_t*)rxPtr;
            EC_ASSERT(rxLen == headerPtr->txNum, rxLen, headerPtr->txNum, 0);
            EC_ASSERT(seqNo == headerPtr->seqNo, seqNo, headerPtr->seqNo, 0);
            seqNo++;

            for(uint32_t i = sizeof(Header_t); i < rxLen; i++)
            {
                if(rxPtr[i] != ((uint8_t)i))
                {
                    EC_ASSERT(0, i, rxPtr[i], 0);
                }
            }
        }

        SFDT_freeRxFrameBuffer(rxPtr);
    }
}


void loopBackTaskInit(void)
{

    SFDT_masterInit();

    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    task_attr.name = "spitx";

#if defined FEATURE_FREERTOS_ENABLE
    memset(tx_task_stack, 0xA5,TASK_STACK_SIZE);
    task_attr.stack_mem = tx_task_stack;
    task_attr.cb_mem = &tx_task;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block
    task_attr.priority = osPriorityNormal;
#elif defined FEATURE_LITEOS_ENABLE
    task_attr.priority = osPriorityNormal1;
#endif
    task_attr.stack_size = TASK_STACK_SIZE;

    osThreadNew(txTask, NULL, &task_attr);

    memset(&task_attr,0,sizeof(task_attr));
    task_attr.name = "spirx";

#if defined FEATURE_FREERTOS_ENABLE
    memset(rx_task_stack, 0xA5,TASK_STACK_SIZE);
    task_attr.stack_mem = rx_task_stack;
    task_attr.cb_mem = &rx_task;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block
    task_attr.priority = osPriorityNormal;
#elif defined FEATURE_LITEOS_ENABLE
    task_attr.priority = osPriorityNormal1;
#endif
    task_attr.stack_size = TASK_STACK_SIZE;

    osThreadNew(rxTask, NULL, &task_attr);

}

