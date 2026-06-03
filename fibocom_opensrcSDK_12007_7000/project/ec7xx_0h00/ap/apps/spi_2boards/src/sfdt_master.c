/******************************************************************************

*(C) Copyright 2024 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description:
*
*  History: Rev1.0   2024-01-02
*
*  Notes:
*
******************************************************************************/

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/
#include "sfdt.h"
#include DEBUG_LOG_HEADER_FILE

#if defined FEATURE_FREERTOS_ENABLE
#include "FreeRTOS.h"
#endif
#include "cmsis_os2.h"
#include "bsp_spi.h"
#include "slpman.h"


/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/
#ifdef MAX
#undef MAX
#endif
#define MAX(a,b)   ((a) > (b) ? (a) : (b))

#define MASTER_TRANSFER_TASK_STACK_SIZE       (1024)

#define MASTER_TX_MSGQUEUE_CAPACITY           (32)
#define MASTER_RX_MSGQUEUE_CAPACITY           (1)

#define MASTER_TRANSFER_TX_FLAG               (0x1)
#define MASTER_TRANSFER_RX_FLAG               (0x2)
#define MASTER_TRANSFER_FLOW_CONTROL_FLAG     (0x4)

#define MASTER_TRANSFER_TRIGGER_FLAG          (MASTER_TRANSFER_TX_FLAG | MASTER_TRANSFER_RX_FLAG | MASTER_TRANSFER_FLOW_CONTROL_FLAG)

#define MASTER_TRANSFER_DONE_FLAG             (0x1)

/*----------------------------------------------------------------------------*
 *                    DATA TYPE DEFINITION                                    *
 *----------------------------------------------------------------------------*/
// SPI send message queue element typedef
typedef struct
{
    uint8_t * payLoadPtr;

    uint32_t  payLoadLen : 13;
    uint32_t  resv1      : 3;
    uint32_t  flowControl: 1;
    uint32_t  resv2      : 15;
} MasterMsgQueueObj_t;


/*----------------------------------------------------------------------------*
 *                      GLOBAL VARIABLES                                      *
 *----------------------------------------------------------------------------*/

#if SFDT_MASTER_INSTANCE == 0

#if RTE_SPI0 == 0
#error SPI0 shall be enabled in rte
#endif

extern ARM_DRIVER_SPI Driver_SPI0;
static ARM_DRIVER_SPI *spiMasterDrv = &CREATE_SYMBOL(Driver_SPI, 0);

#if RTE_SPI0_IO_MODE != DMA_MODE
#error SPI shall be set to DMA mode
#endif

#elif SFDT_MASTER_INSTANCE == 1

#if RTE_SPI1 == 0
#error SPI1 shall be enabled in rte
#endif

extern ARM_DRIVER_SPI Driver_SPI1;
static ARM_DRIVER_SPI *spiMasterDrv = &CREATE_SYMBOL(Driver_SPI, 1);

#if RTE_SPI1_IO_MODE != DMA_MODE
#error SPI shall be set to DMA mode
#endif

#else
#error unsupported spi instance
#endif


static StaticTask_t             gMasterTransferTask;
static uint8_t                  gMasterTransferTaskStack[MASTER_TRANSFER_TASK_STACK_SIZE];

static osMessageQueueId_t       gMasterTxMsgQueueId = NULL;
static StaticQueue_t            gMasterTxQueueCb;
static uint8_t                  gMasterTxQueueBuf[MASTER_TX_MSGQUEUE_CAPACITY * sizeof(MasterMsgQueueObj_t)];

static osMessageQueueId_t       gMasterRxMsgQueueId = NULL;
static StaticQueue_t            gMasterRxQueueCb;
static uint8_t                  gMasterRxQueueBuf[MASTER_RX_MSGQUEUE_CAPACITY * sizeof(MasterMsgQueueObj_t)];

static osEventFlagsId_t         gMasterTransferTriggerFlag = NULL;
static osEventFlagsId_t         gMasterTransferDoneFlag = NULL;
static SFDTFrameHeader_t        gMasterTransferHeader = {0};

// Ping-pong buffer for rx
static uint8_t *gMasterRxBuffer0 = NULL;
static uint8_t *gMasterRxBuffer1 = NULL;

static uint32_t gMasterSrdyMask = 0;

static SFDTFlowControl_e        gMasterFlowControlState  = SFDT_FLOW_CONTROL_XON;
static SFDTFlowControl_e        gSlaveFlowControlState = SFDT_FLOW_CONTROL_XON;

/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTION DECLEARATION                         *
 *----------------------------------------------------------------------------*/
static void SFDT_masterTransferTaskEntry(void *arg);
static int32_t SFDT_masterHwInit(void);
static void SFDT_createMasterTransferTask(void);
static void SFDT_masterCallback(uint32_t event);
static void SFDT_enterLowPowerCallBack(void *pdata, slpManLpState state);
static void SFDT_exitLowPowerCallBack(void *pdata, slpManLpState state);
static void SFDT_pullMrdyLow(void);
static void SFDT_pullMrdyHigh(void);
static void *SFDT_mallocRxFrameBuffer(void);


/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTIONS                                     *
 *----------------------------------------------------------------------------*/

static void SFDT_masterCallback(uint32_t event)
{
    osEventFlagsSet(gMasterTransferDoneFlag, MASTER_TRANSFER_DONE_FLAG);
}

static void SFDT_enterLowPowerCallBack(void *pdata, slpManLpState state)
{
    switch(state)
    {
        case SLPMAN_HIBERNATE_STATE:
        case SLPMAN_SLEEP2_STATE:
        case SLPMAN_SLEEP1_STATE:

            break;
        default:
            break;
    }

    return;
}

static void SFDT_exitLowPowerCallBack(void *pdata, slpManLpState state)
{
    switch(state)
    {
        case SLPMAN_HIBERNATE_STATE:
        case SLPMAN_SLEEP2_STATE:
        case SLPMAN_SLEEP1_STATE:
            // Pull MRDY low
            {
                GpioPinConfig_t config;
                config.pinDirection = GPIO_DIRECTION_OUTPUT;
                config.misc.initOutput = 0;

                GPIO_pinConfig(SFDT_MASTER_MRDY_GPIO_INSTANCE, SFDT_MASTER_MRDY_GPIO_INDEX, &config);
            }
            break;
        default:
            break;
    }

    return;
}

static void SFDT_pullMrdyLow(void)
{
    GPIO_pinWrite(SFDT_MASTER_MRDY_GPIO_INSTANCE, 1 << SFDT_MASTER_MRDY_GPIO_INDEX, 0);
}

static void SFDT_pullMrdyHigh(void)
{
    GPIO_pinWrite(SFDT_MASTER_MRDY_GPIO_INSTANCE, 1 << SFDT_MASTER_MRDY_GPIO_INDEX, 1 << SFDT_MASTER_MRDY_GPIO_INDEX);
}


static int32_t SFDT_masterHwInit(void)
{
    int32_t ret = ARM_DRIVER_OK;

#if SFDT_BUS_SPEED > 4000000
    apmuVoteToDozeState(PMU_DOZE_SPI_MOD, false);
    CLOCK_clockEnable(CLK_HF102M);
    CLOCK_setClockSrc(CLK_APB_MP, CLK_APB_MP_SEL_102M);

    // Switch clock before spi init
    CLOCK_clockEnable(CLK_HF612M);
    CLOCK_setClockSrc(FCLK_SPI0, FCLK_SPI0_SEL_612M);
#endif

    // Initialize master spi
    ret = spiMasterDrv->Initialize(SFDT_masterCallback);

    if(ret != ARM_DRIVER_OK)
        return ret;

    // Power on
    ret = spiMasterDrv->PowerControl(ARM_POWER_FULL);

    if(ret != ARM_DRIVER_OK)
        return ret;

    // Configure spi bus
    ret = spiMasterDrv->Control(ARM_SPI_MODE_MASTER | SFDT_SPI_FRAME_FORMAT | ARM_SPI_DATA_BITS(8) |
                                  ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_UNUSED, SFDT_BUS_SPEED);

    ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_masterHwInit, P_DEBUG, "SFDT_masterHwInit, retCode: %d", ret);

    if(ret != ARM_DRIVER_OK)
        return ret;

    spiMasterDrv->Control(ARM_SPI_SET_DEFAULT_TX_VALUE, 0xF0F0);

    // Handshake Pins
    // MRDY pin(output), have done in bsp_customInit();
    //SFDT_masterMrdyInit();

    // SRDY pin(input)
    APmuWakeupPadSettings_t wakeupPadSetting;

    if(slpManGetLastSlpState() == SLP_ACTIVE_STATE)
    {
        wakeupPadSetting.negEdgeEn = false;
        wakeupPadSetting.posEdgeEn = true;
        wakeupPadSetting.pullDownEn = false;
        wakeupPadSetting.pullUpEn = true;

        NVIC_DisableIRQ(SFDT_MASTER_SRDY_IRQ);
        NVIC_ClearPendingIRQ(SFDT_MASTER_SRDY_IRQ);

        slpManSetWakeupPadCfg(SFDT_MASTER_SRDY_PAD, true, &wakeupPadSetting);
    }

    return ret;
}

static void *SFDT_mallocRxFrameBuffer(void)
{
    void* ptr = malloc(SFDT_FRAME_MTU);
    return ptr;
}

static void SFDT_createMasterTransferTask(void)
{
    if(gMasterTxMsgQueueId != NULL)
    {
        return;
    }

    osThreadId_t threadId;
    osThreadAttr_t task_attr;
    osMessageQueueAttr_t queue_attr;
    slpManRet_t ret = RET_TRUE;

    gMasterTransferTriggerFlag = osEventFlagsNew(NULL);

    EC_ASSERT(gMasterTransferTriggerFlag, gMasterTransferTriggerFlag, 0, 0);

    // Enable wakeup IRQ when we're ready
    NVIC_EnableIRQ(SFDT_MASTER_SRDY_IRQ);

    gMasterTransferDoneFlag = osEventFlagsNew(NULL);

    EC_ASSERT(gMasterTransferDoneFlag, gMasterTransferDoneFlag, 0, 0);

    memset(&queue_attr, 0, sizeof(queue_attr));

    queue_attr.mq_size = sizeof(gMasterTxQueueBuf);
    queue_attr.mq_mem = gMasterTxQueueBuf;
    queue_attr.cb_mem = &gMasterTxQueueCb;
    queue_attr.cb_size = sizeof(gMasterTxQueueCb);

    gMasterTxMsgQueueId = osMessageQueueNew(MASTER_TX_MSGQUEUE_CAPACITY, sizeof(MasterMsgQueueObj_t), &queue_attr);
    EC_ASSERT(gMasterTxMsgQueueId, gMasterTxMsgQueueId, 0, 0);

    memset(&queue_attr, 0, sizeof(queue_attr));

    queue_attr.mq_size = sizeof(gMasterRxQueueBuf);
    queue_attr.mq_mem = gMasterRxQueueBuf;
    queue_attr.cb_mem = &gMasterRxQueueCb;
    queue_attr.cb_size = sizeof(gMasterRxQueueCb);

    gMasterRxMsgQueueId = osMessageQueueNew(MASTER_RX_MSGQUEUE_CAPACITY, sizeof(MasterMsgQueueObj_t), &queue_attr);
    EC_ASSERT(gMasterRxMsgQueueId, gMasterRxMsgQueueId, 0, 0);

    memset(&task_attr,0,sizeof(task_attr));
    memset(gMasterTransferTaskStack, 0xA5, MASTER_TRANSFER_TASK_STACK_SIZE);
    task_attr.name = "sfdt master";
    task_attr.stack_mem = gMasterTransferTaskStack;
    task_attr.stack_size = MASTER_TRANSFER_TASK_STACK_SIZE;
    task_attr.priority = osPriorityLow;
    task_attr.cb_mem = &gMasterTransferTask;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block

    threadId = osThreadNew(SFDT_masterTransferTaskEntry, NULL, &task_attr);

    EC_ASSERT(threadId, threadId, 0, 0);

    ret = slpManRegisterUsrdefinedBackupCb(SFDT_enterLowPowerCallBack, NULL);
    EC_ASSERT(ret == RET_TRUE, ret, 0, 0);

    ret = slpManRegisterUsrdefinedRestoreCb(SFDT_exitLowPowerCallBack, NULL);
    EC_ASSERT(ret == RET_TRUE, ret, 0, 0);

}


/**
  \fn          static void SFDT_masterTransferTaskEntry(void *arg)
  \brief
  \return
 */
static void SFDT_masterTransferTaskEntry(void *arg)
{
    MasterMsgQueueObj_t txMsg, rxMsg;

    uint32_t mask, flags, timeout, loop = 0;

    uint32_t txNum, txMsgCnt = 0, txSeqNo = 0;

    uint32_t pendingRxCnt = 0, rxCnt = 0, rxPayLoadLen = 0;

    uint8_t* rxBuffer = NULL;

    SFDTFrameHeader_t *header = NULL;

    // alloc rx buffer
    rxBuffer = SFDT_mallocRxFrameBuffer();
    EC_ASSERT(rxBuffer, rxBuffer, 0, 0);

    osStatus_t status;

    while(1)
    {
        txNum = SFDT_MIN_TRANSFER_DUMMY_BYTES;
        timeout = 200;
        flags = 0;

        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_masterTransferTaskEntry_0, P_WARNING, "master task loop: %d, pending rx cnt: %d", loop++, pendingRxCnt);

        if(pendingRxCnt == 0)
        {
            // wait for tx or rx trigger
            flags = osEventFlagsWait(gMasterTransferTriggerFlag, MASTER_TRANSFER_TRIGGER_FLAG, osFlagsWaitAny, osWaitForever);

            // condition((flags & MASTER_TRANSFER_TRIGGER_FLAG) != 0) fails in case of osFlagsErrorISR(0xFFFFFFFAU) error codes (primask may be set by mistake)
            EC_ASSERT(flags < osFlagsError, flags, 0 , 0);

            // skip handshake if rx flag is set
            if(flags & MASTER_TRANSFER_RX_FLAG)
            {
                goto TRANSFER;
            }

        }
        else
        {
            // No need to send handshake req
            goto WAIT_HANDSHAKE;
        }


TRIG_HANDSHAKE:
        // Rising edge
        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_masterTransferTaskEntry_1, P_WARNING, "master send Mrdy handshake, timeout: %d ms", timeout);

        SFDT_pullMrdyHigh();
WAIT_HANDSHAKE:
        flags = osEventFlagsWait(gMasterTransferTriggerFlag, MASTER_TRANSFER_RX_FLAG, osFlagsWaitAll, timeout);

        SFDT_pullMrdyLow();

        if(flags == osFlagsErrorTimeout)
        {
            timeout += 1000;
            goto TRIG_HANDSHAKE;
        }
        else
        {
            EC_ASSERT((flags < osFlagsError) && ((flags & MASTER_TRANSFER_RX_FLAG) == MASTER_TRANSFER_RX_FLAG), flags, 0 , 0);
        }


TRANSFER:

        txMsgCnt = osMessageQueueGetCount(gMasterTxMsgQueueId);

        memset(&txMsg, 0, sizeof(txMsg));

        if(gSlaveFlowControlState == SFDT_FLOW_CONTROL_XON)
        {
            status = osMessageQueueGet(gMasterTxMsgQueueId, &txMsg, 0, 0);
        }
        else
        {
            // Shall not consume tx item in XOFF state
            status = osErrorResource;
        }

        EC_ASSERT((status == osOK) || (status == osErrorResource), status, txMsgCnt, 0);

        header = &gMasterTransferHeader;

        if(txMsg.payLoadLen && txMsg.payLoadPtr)
        {
            header = (SFDTFrameHeader_t *)(txMsg.payLoadPtr - SFDT_FRAME_HEADER_SIZE);
        }

        memset(header, 0, SFDT_FRAME_HEADER_SIZE);
        header->field.payLoadLen = txMsg.payLoadLen;
        header->field.flowControl = gMasterFlowControlState;
        header->field.seqNo = txSeqNo++;

        // Slave will ask master to send if bContinue flag is set and it brings endless req in case of slave XOFF state(txMsgCnt can't be consumed)
        if((txMsgCnt > 1) && (gSlaveFlowControlState == SFDT_FLOW_CONTROL_XON))
        {
            header->field.bContinue = true;
        }

        header->field.checkSum = SFDT_csum(header);

        txNum = MAX(header->field.payLoadLen + SFDT_FRAME_HEADER_SIZE, pendingRxCnt);
        txNum = MAX(SFDT_MIN_TRANSFER_DUMMY_BYTES, txNum);

        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_masterTransferTaskEntry_3, P_WARNING, "master transfer, payLoadLen: %d, flowControl: %d, seqNo: %d, continue: %d, checkSum: 0x%x",
                                                                                    header->field.payLoadLen, header->field.flowControl, header->field.seqNo, header->field.bContinue, header->field.checkSum);

        mask = SaveAndSetIRQMask();
        if(gMasterSrdyMask)
        {
            gMasterSrdyMask = 0;
        }
        RestoreIRQMask(mask);

        spiMasterDrv->Transfer(header, rxBuffer, txNum);

        flags = osEventFlagsWait(gMasterTransferDoneFlag, MASTER_TRANSFER_DONE_FLAG, osFlagsWaitAll, osWaitForever);
        EC_ASSERT(flags == MASTER_TRANSFER_DONE_FLAG, flags, 0, 0);

        rxCnt = spiMasterDrv->GetDataCount();
        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_masterTransferTaskEntry_4, P_WARNING, "master rx cnt: %d", rxCnt);
        ECPLAT_DUMP(UNILOG_PLA_APP, SFDT_masterTransferTaskEntry_5, P_WARNING, "master rx dump:", MIN(16, rxCnt), rxBuffer);

        // parse rx data
        header = (SFDTFrameHeader_t *)rxBuffer;

        if(pendingRxCnt == 0)
        {
            if(SFDT_csum(header) == header->field.checkSum)
            {
                rxPayLoadLen = header->field.payLoadLen;

                if(rxPayLoadLen <= (SFDT_FRAME_MTU - SFDT_FRAME_HEADER_SIZE))
                {
                    if((gSlaveFlowControlState == SFDT_FLOW_CONTROL_XOFF) &&
                        (header->field.flowControl == SFDT_FLOW_CONTROL_XON) &&
                        (txMsgCnt > 0))
                    {
                        // Resume tx when flow control switches from XOFF to XON
                        osEventFlagsSet(gMasterTransferTriggerFlag, MASTER_TRANSFER_TX_FLAG);
                    }
                    gSlaveFlowControlState = header->field.flowControl;
                    rxMsg.flowControl = header->field.flowControl;
                    rxMsg.payLoadLen = header->field.payLoadLen;
                    rxMsg.payLoadPtr = rxBuffer + SFDT_FRAME_HEADER_SIZE;

                    if((rxPayLoadLen + SFDT_FRAME_HEADER_SIZE) > rxCnt)
                    {
                        pendingRxCnt = rxPayLoadLen + SFDT_FRAME_HEADER_SIZE - rxCnt;
                        //adjust rxBuffer position to receive tailing data
                        rxBuffer += rxCnt;
                    }
                    else
                    {
                        goto REPORT;
                    }
                }
            }
        }
        else if(rxCnt >= pendingRxCnt)
        {
REPORT:
            // rx report, only if we have new buffer for rx, otherwise we have to overwrite the old one
            mask = SaveAndSetIRQMask();

            if(gMasterRxBuffer1)
            {
                rxBuffer = gMasterRxBuffer1;
                gMasterRxBuffer1 = NULL;
                RestoreIRQMask(mask);

                osMessageQueuePut(gMasterRxMsgQueueId, &rxMsg, 0, 0);
            }

            RestoreIRQMask(mask);

            pendingRxCnt = 0;
        }
        else
        {
            // something is wrong
            EC_ASSERT(0, rxCnt, pendingRxCnt, 0);
        }



        if(txMsgCnt != 0)
        {
            if(txMsg.payLoadPtr)
            {
                free(txMsg.payLoadPtr - SFDT_FRAME_HEADER_SIZE);
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *                      GLOBAL FUNCTIONS                                      *
 *----------------------------------------------------------------------------*/
SFDT_MASTER_SRDY_IRQ_ISR
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_masterSrdyIsr, P_WARNING, "master got srdy handshake, srdyMask: 0x%x", gMasterSrdyMask);

    uint32_t mask = SaveAndSetIRQMask();

    if(gMasterTransferTriggerFlag && (gMasterSrdyMask == 0))
    {
        gMasterSrdyMask++;
        RestoreIRQMask(mask);
        osEventFlagsSet(gMasterTransferTriggerFlag, MASTER_TRANSFER_RX_FLAG);
    }

    RestoreIRQMask(mask);
}


void SFDT_masterSend(uint8_t* payLoadPtr, uint32_t payLoadLen)
{
    if(gSlaveFlowControlState == SFDT_FLOW_CONTROL_XOFF)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_masterSend_0, P_WARNING, "Slave flow control is XOFF!");
    }

    if(gMasterTxMsgQueueId != NULL)
    {
        uint32_t freeSlot = osMessageQueueGetSpace(gMasterTxMsgQueueId);

        MasterMsgQueueObj_t msg = {0};

        msg.payLoadPtr = payLoadPtr;
        msg.payLoadLen = payLoadLen;

        // Full
        if(freeSlot == 0)
        {
            osEventFlagsSet(gMasterTransferTriggerFlag, MASTER_TRANSFER_TX_FLAG);
            osMessageQueuePut(gMasterTxMsgQueueId, &msg, 0, osWaitForever);
        }
        else
        {
            osMessageQueuePut(gMasterTxMsgQueueId, &msg, 0, osWaitForever);
            osEventFlagsSet(gMasterTransferTriggerFlag, MASTER_TRANSFER_TX_FLAG);
        }
    }
}

void SFDT_masterReceive(uint8_t** reportData, uint32_t* reportLen, uint32_t* flag)
{
    osStatus_t status;
    MasterMsgQueueObj_t msg = {0};

    // alloc a new rx frame buffer as need
    if(gMasterRxBuffer0 == NULL)
    {
        gMasterRxBuffer0 = SFDT_mallocRxFrameBuffer();
    }

    uint32_t mask = SaveAndSetIRQMask();

    if((gMasterRxBuffer1 == NULL) && (gMasterRxBuffer0 != NULL))
    {
        gMasterRxBuffer1 = gMasterRxBuffer0;
        gMasterRxBuffer0 = NULL;
    }

    RestoreIRQMask(mask);

    if(gMasterRxMsgQueueId)
    {
        status = osMessageQueueGet(gMasterRxMsgQueueId, &msg, 0, osWaitForever);

        if(status == osOK)
        {
            *reportData = msg.payLoadPtr;
            *reportLen = msg.payLoadLen;
            *flag = msg.flowControl;
        }
    }

}



void *SFDT_mallocTxFrameBuffer(uint32_t size)
{
    if(size > (SFDT_FRAME_MTU - SFDT_FRAME_HEADER_SIZE))
    {
        return NULL;
    }

    void* ptr = malloc(size + SFDT_FRAME_HEADER_SIZE);

    if(ptr != NULL)
    {
        return ptr + SFDT_FRAME_HEADER_SIZE;
    }
    else
    {
        return ptr;
    }
}

void SFDT_freeRxFrameBuffer(uint8_t *payLoadPtr)
{
   free(payLoadPtr - SFDT_FRAME_HEADER_SIZE);
}

int32_t SFDT_masterInit(void)
{
    int32_t ret = SFDT_masterHwInit();

    if(ret != ARM_DRIVER_OK)
        return ret;

    SFDT_createMasterTransferTask();

    return 0;
}

void SFDT_masterMrdyInit(void)
{
    slpManAONIOPowerOn();

    // MRDY pin(output)
    PadConfig_t padConfig;
    PAD_getDefaultConfig(&padConfig);

    padConfig.mux = SFDT_MASTER_MRDY_PAD_ALT_FUNC;
    PAD_setPinConfig(SFDT_MASTER_MRDY_PAD_INDEX, &padConfig);

    GpioPinConfig_t config;
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    config.misc.initOutput = 0; // Pull down MRDY for init state

    GPIO_pinConfig(SFDT_MASTER_MRDY_GPIO_INSTANCE, SFDT_MASTER_MRDY_GPIO_INDEX, &config);

}

void SFDT_masterFlowControl(SFDTFlowControl_e setting)
{
    uint32_t mask = SaveAndSetIRQMask();

    if(gMasterFlowControlState != setting)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_masterFlowControl_0, P_WARNING, "master flow control setting: %d", setting);

        gMasterFlowControlState = setting;

        if(gMasterTransferTriggerFlag)
        {
            osEventFlagsSet(gMasterTransferTriggerFlag, MASTER_TRANSFER_FLOW_CONTROL_FLAG);
        }
    }

    RestoreIRQMask(mask);

}


