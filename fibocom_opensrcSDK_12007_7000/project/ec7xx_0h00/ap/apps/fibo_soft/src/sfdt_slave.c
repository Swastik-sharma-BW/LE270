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
#ifdef FEATURE_CCIO_ENABLE
#include "spi_device.h"
#include "ccio_custom.h"
#endif

#ifdef FIBOCOM_BASE
#include "ffw_gpio.h"
#endif

#define SFDT_SLAVE_LOG_ENABLE

/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/
#define SLAVE_DEVICE_INDEX                   (0)

#define SLAVE_TRANSFER_TASK_STACK_SIZE       (1024)

#define SLAVE_TRANSFER_TX_FLAG               (0x1)
#define SLAVE_TRANSFER_RX_FLAG               (0x2)
#define SLAVE_TRANSFER_FLOW_CONTROL_FLAG     (0x4)

#define SLAVE_TRANSFER_TRIGGER_FLAG          (SLAVE_TRANSFER_TX_FLAG | SLAVE_TRANSFER_RX_FLAG)

#define SLAVE_TRANSFER_DONE_FLAG             (0x1)

/*----------------------------------------------------------------------------*
 *                    DATA TYPE DEFINITION                                    *
 *----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*
 *                      GLOBAL VARIABLES                                      *
 *----------------------------------------------------------------------------*/
#if SFDT_SLAVE_INSTANCE == 0

#if RTE_SPI0 == 0
#error SPI0 shall be enabled in rte
#endif

extern ARM_DRIVER_SPI Driver_SPI0;
static ARM_DRIVER_SPI *spiSlaveDrv = &CREATE_SYMBOL(Driver_SPI, 0);

#if RTE_SPI0_IO_MODE != DMA_MODE
#error SPI shall be set to DMA mode
#endif

#elif SFDT_SLAVE_INSTANCE == 1

#if RTE_SPI1 == 0
#error SPI1 shall be enabled in rte
#endif

extern ARM_DRIVER_SPI Driver_SPI1;
static ARM_DRIVER_SPI *spiSlaveDrv = &CREATE_SYMBOL(Driver_SPI, 1);

#if RTE_SPI1_IO_MODE != DMA_MODE
#error SPI shall be set to DMA mode
#endif

#else
#error unsupported spi instance
#endif

static StaticTask_t             gSlaveTransferTask;
static uint8_t                  gSlaveTransferTaskStack[SLAVE_TRANSFER_TASK_STACK_SIZE];

static osEventFlagsId_t         gSlaveTransferTriggerFlag = NULL;
static osEventFlagsId_t         gSlaveTransferDoneFlag = NULL;

#if 0
static SFDTFlowControl_e        gMasterFlowControlState  = SFDT_FLOW_CONTROL_XON;
static SFDTFlowControl_e        gSlaveFlowControlState = SFDT_FLOW_CONTROL_XON;
static SFDTFrameHeader_t        gSlaveTransferHeader = {0};
#endif
/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTION DECLEARATION                         *
 *----------------------------------------------------------------------------*/
static void SFDT_slaveTransferTaskEntry(void *arg);
static int32_t SFDT_slaveHwInit(void);
static void SFDT_createSlaveTransferTask(void);
static void SFDT_slaveCallback(uint32_t event);
static void SFDT_enterLowPowerCallBack(void *pdata, slpManLpState state);
static void SFDT_exitLowPowerCallBack(void *pdata, slpManLpState state);
static void SFDT_pullSrdyLow(void);
static void SFDT_pullSrdyHigh(void);

/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTIONS                                     *
 *----------------------------------------------------------------------------*/
static void SFDT_slaveCallback(uint32_t event)
{
#ifdef SFDT_SLAVE_LOG_ENABLE
    ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveCallback, P_WARNING, "slave transfer done");
#endif
    osEventFlagsSet(gSlaveTransferDoneFlag, SLAVE_TRANSFER_DONE_FLAG);
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
            // Pull SRDY low
            {
                GpioPinConfig_t config;
                config.pinDirection = GPIO_DIRECTION_OUTPUT;
                config.misc.initOutput = 0;

                GPIO_pinConfig(SFDT_SLAVE_SRDY_GPIO_INSTANCE, SFDT_SLAVE_SRDY_GPIO_INDEX, &config);
            }
            break;
        default:
            break;
    }

    return;
}

static void SFDT_pullSrdyLow(void)
{
    GPIO_pinWrite(SFDT_SLAVE_SRDY_GPIO_INSTANCE, 1 << SFDT_SLAVE_SRDY_GPIO_INDEX, 0);
}

static void SFDT_pullSrdyHigh(void)
{
    GPIO_pinWrite(SFDT_SLAVE_SRDY_GPIO_INSTANCE, 1 << SFDT_SLAVE_SRDY_GPIO_INDEX, 1 << SFDT_SLAVE_SRDY_GPIO_INDEX);
}

#ifdef FIBOCOM_BASE
static void srdy_isrcallback(void *level)
{
#ifdef SFDT_SLAVE_LOG_ENABLE
    ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveMrdyIsr, P_WARNING, "slave got mrdy handshake");
#endif
    if(gSlaveTransferTriggerFlag)
    {
        osEventFlagsSet(gSlaveTransferTriggerFlag, SLAVE_TRANSFER_RX_FLAG);
    }
}

static int32_t SFDT_slaveHwInit(void)
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

    //config spi
#ifdef CHIP_EC718
    ffw_gpiomode_set(66, 1);
    ffw_gpiomode_set(67, 1);
    ffw_gpiomode_set(28, 1);
    ffw_gpiomode_set(29, 1);
#else
    ffw_gpiomode_set(62, 1);
    ffw_gpiomode_set(64, 1);
    ffw_gpiomode_set(28, 1);
    ffw_gpiomode_set(29, 1);
#endif

    // Initialize master spi
    ret = spiSlaveDrv->Initialize(SFDT_slaveCallback);

    if(ret != ARM_DRIVER_OK)
        return ret;

    // Power on
    ret = spiSlaveDrv->PowerControl(ARM_POWER_FULL);

    if(ret != ARM_DRIVER_OK)
        return ret;

    // Configure spi bus
    ret = spiSlaveDrv->Control(ARM_SPI_MODE_SLAVE | SFDT_SPI_FRAME_FORMAT | ARM_SPI_DATA_BITS(8) |
                                  ARM_SPI_MSB_LSB, SFDT_BUS_SPEED);

#ifdef SFDT_SLAVE_LOG_ENABLE
    ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveHwInit, P_DEBUG, "SFDT_slaveHwInit, retCode: %d", ret);
#endif

    if(ret != ARM_DRIVER_OK)
        return ret;


    spiSlaveDrv->Control(ARM_SPI_SET_DEFAULT_TX_VALUE, 0xF0F0);


    // Handshake Pins
    // SRDY pin(output), have done in bsp_customInit();
    // SFDT_slaveSrdyInit();

    // MRDY pin(input)
     ffw_gpioisr_t slave_isr;
     memset(&slave_isr,0,sizeof(slave_isr));
     
    slave_isr.intr_enable = true;
    slave_isr.intr_level = false;
    slave_isr.falling = false;
    slave_isr.rising = true;
    slave_isr.pullen = true;
    slave_isr.pullup = true;
    slave_isr.cb = srdy_isrcallback;

#ifdef CHIP_EC718
    ffw_gpiomode_set(22, 0);
#else
    ffw_gpiomode_set(22, 4);
#endif
    ffw_gpio_isr_init(22, &slave_isr);

    return ret;
}
#else
static int32_t SFDT_slaveHwInit(void)
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
    ret = spiSlaveDrv->Initialize(SFDT_slaveCallback);

    if(ret != ARM_DRIVER_OK)
        return ret;

    // Power on
    ret = spiSlaveDrv->PowerControl(ARM_POWER_FULL);

    if(ret != ARM_DRIVER_OK)
        return ret;

    // Configure spi bus
    ret = spiSlaveDrv->Control(ARM_SPI_MODE_SLAVE | SFDT_SPI_FRAME_FORMAT | ARM_SPI_DATA_BITS(8) |
                                  ARM_SPI_MSB_LSB, SFDT_BUS_SPEED);

#ifdef SFDT_SLAVE_LOG_ENABLE
    ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveHwInit, P_DEBUG, "SFDT_slaveHwInit, retCode: %d", ret);
#endif

    if(ret != ARM_DRIVER_OK)
        return ret;


    spiSlaveDrv->Control(ARM_SPI_SET_DEFAULT_TX_VALUE, 0xF0F0);


    // Handshake Pins
    // SRDY pin(output), have done in bsp_customInit();
    // SFDT_slaveSrdyInit();

    // MRDY pin(input)
    APmuWakeupPadSettings_t wakeupPadSetting;

    if(slpManGetLastSlpState() == SLP_ACTIVE_STATE)
    {
        wakeupPadSetting.negEdgeEn = false;
        wakeupPadSetting.posEdgeEn = true;
        wakeupPadSetting.pullDownEn = false;
        wakeupPadSetting.pullUpEn = true;

        NVIC_DisableIRQ(SFDT_SLAVE_MRDY_IRQ);
        NVIC_ClearPendingIRQ(SFDT_SLAVE_MRDY_IRQ);

        slpManSetWakeupPadCfg(SFDT_SLAVE_MRDY_PAD, true, &wakeupPadSetting);
    }

    return ret;
}
#endif

static void SFDT_createSlaveTransferTask(void)
{
    if(gSlaveTransferTriggerFlag)
    {
        return;
    }

    osThreadId_t threadId;
    osThreadAttr_t task_attr;
    slpManRet_t ret = RET_TRUE;

    gSlaveTransferTriggerFlag = osEventFlagsNew(NULL);

    EC_ASSERT(gSlaveTransferTriggerFlag, gSlaveTransferTriggerFlag, 0, 0);

#ifndef FIBOCOM_BASE
    // Enable wakeup IRQ when we're ready
    NVIC_EnableIRQ(SFDT_SLAVE_MRDY_IRQ);
#endif

    gSlaveTransferDoneFlag = osEventFlagsNew(NULL);

    EC_ASSERT(gSlaveTransferDoneFlag, gSlaveTransferDoneFlag, 0, 0);

    memset(&task_attr,0,sizeof(task_attr));
    memset(gSlaveTransferTaskStack, 0xA5, SLAVE_TRANSFER_TASK_STACK_SIZE);
    task_attr.name = "sfdt slave";
    task_attr.stack_mem = gSlaveTransferTaskStack;
    task_attr.stack_size = SLAVE_TRANSFER_TASK_STACK_SIZE;
    task_attr.priority = osPriorityNormal2;
    task_attr.cb_mem = &gSlaveTransferTask;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block

    threadId = osThreadNew(SFDT_slaveTransferTaskEntry, NULL, &task_attr);

    EC_ASSERT(threadId, threadId, 0, 0);

    ret = slpManRegisterUsrdefinedBackupCb(SFDT_enterLowPowerCallBack, NULL);
    EC_ASSERT(ret == RET_TRUE, ret, 0, 0);

    ret = slpManRegisterUsrdefinedRestoreCb(SFDT_exitLowPowerCallBack, NULL);
    EC_ASSERT(ret == RET_TRUE, ret, 0, 0);

}


/**
  \fn          static void SFDT_slaveTransferTaskEntry(void *arg)
  \brief
  \return
 */
//#define SPI_LB_TEST  1
PLAT_FM_RAMCODE static void SFDT_slaveTransferTaskEntry(void *arg)
{
    SpiDevConf_t spiDevConf = {
#if (RTE_IPOS_EN == 1)
    .mainUsage  = CSIO_DT_OPAQ,
    .custFlags  = CSIO_OCF_IPOS,
    .custExtras = 1 << CSIO_IPOS_CID_DFT,
#else
#if (SPI_LB_TEST == 1)
    .mainUsage  = CSIO_DT_OPAQ,
    .custFlags  = CSIO_OCF_SPITEST,
    .custExtras = 0,
#else
    .mainUsage  = CSIO_DT_AT,
    .custFlags  = CSIO_OCF_UNDEF,
    .custExtras = 0,
#endif
#endif
    .bmCreateFlag = CCIO_TASK_FLAG_RX | CCIO_TASK_FLAG_TX2,
    .rbufFlags = CUST_RBUF_FOR_SPI
    };

    int32_t errCode = CCIO_EOK;

#ifdef SFDT_SLAVE_LOG_ENABLE
    uint32_t loop = 0;
#endif

    uint32_t eventFlags = 0, timeout;

    uint32_t txNum = 0, pendingTxCnt = 0, txCnt = 0, txSeqNo = 0;

    uint32_t rxPayLoadLen = 0;

    uint8_t* rxBuffer = NULL, *rxNewBuffer = NULL, *txPtr = NULL;

    CcioBufDesc_t txBufDesc;

    SFDTFrameHeader_t *header = NULL;

    errCode = spiDevCreate(SLAVE_DEVICE_INDEX, &spiDevConf);

    EC_ASSERT(errCode == CCIO_EOK, errCode, 0, 0);

    // alloc rx buffer
    rxBuffer = spiDevTryGetRxBuf(SLAVE_DEVICE_INDEX, 0);
    EC_ASSERT(rxBuffer, rxBuffer, 0, 0);

    while(1)
    {
        timeout = 200;

#ifdef SFDT_SLAVE_LOG_ENABLE
        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_0, P_WARNING, "slave transfer task loop: %d", loop++);
#endif
        if(pendingTxCnt == 0)
        {
            eventFlags = 0;

            // Check flow control state of peer side, we can ONLY send header and AREN'T able to consume tx item in XOFF state
            // Poll tx list to check if we have something to send
CHECK_TX:
            errCode = spiDevPickTxBuf(SLAVE_DEVICE_INDEX, &txBufDesc);

            if((errCode == CCIO_EFOUND) && (eventFlags == 0))
            {
                // tx list is empty, wait for tx or rx trigger
                eventFlags = osEventFlagsWait(gSlaveTransferTriggerFlag, SLAVE_TRANSFER_TRIGGER_FLAG, osFlagsWaitAny, osWaitForever);

                EC_ASSERT((eventFlags & SLAVE_TRANSFER_TRIGGER_FLAG) != 0, eventFlags, 0 , 0);

                // go to check tx no matter which source triggers
                goto CHECK_TX;

            }
            else if(errCode == CCIO_EOK)
            {
                // noise suppress
                osEventFlagsClear(gSlaveTransferTriggerFlag, SLAVE_TRANSFER_TX_FLAG);

                header = (SFDTFrameHeader_t *)txBufDesc.buf;
                memset(header, 0, SFDT_FRAME_HEADER_SIZE);

                header->field.payLoadLen = txBufDesc.len - SFDT_FRAME_HEADER_SIZE;
                header->field.flowControl = SFDT_FLOW_CONTROL_XON;
                header->field.seqNo = txSeqNo++;
                header->field.checkSum = SFDT_csum(header);
                txPtr = txBufDesc.buf;
                txNum = txBufDesc.len;
                pendingTxCnt = txNum;

#ifdef SFDT_SLAVE_LOG_ENABLE
                ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_1, P_WARNING, "slave transfer, payLoadLen: %d, flowControl: %d, seqNo: %d, checkSum: 0x%x", header->field.payLoadLen, header->field.flowControl, header->field.seqNo, header->field.checkSum);
                ECPLAT_DUMP(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_2, P_WARNING, "slave tx dump(total 16B):", 16, (uint8_t*)header);
#endif
            }
        }

        spiSlaveDrv->Control(ARM_SPI_PURGE_COMM, 0);

        if(txNum == 0)
        {
            spiSlaveDrv->Receive(rxBuffer, SFDT_FRAME_MTU);
        }
        else
        {
            spiSlaveDrv->Transfer(txPtr, rxBuffer, SFDT_FRAME_MTU);
        }
#ifdef SFDT_SLAVE_LOG_ENABLE
        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_3, P_WARNING, "slave is ready to transfer, txNum: %d", txNum);
#endif

HANDSHAKE:
        // Rising edge
        SFDT_pullSrdyHigh();
        eventFlags = osEventFlagsWait(gSlaveTransferDoneFlag, SLAVE_TRANSFER_DONE_FLAG, osFlagsWaitAll, timeout);
        SFDT_pullSrdyLow();

        if(eventFlags == osFlagsErrorTimeout)
        {
            timeout += 1000;
#ifdef SFDT_SLAVE_LOG_ENABLE
            ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_4, P_WARNING, "slave wait for transfer timeout, send handshake, next timeout: %d ms", timeout);
#endif
            goto HANDSHAKE;
        }
        else if(eventFlags != SLAVE_TRANSFER_DONE_FLAG)
        {
            EC_ASSERT(0, eventFlags, 0 , 0);
        }

        txCnt = spiSlaveDrv->GetDataCount();

#ifdef SFDT_SLAVE_LOG_ENABLE
        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_5, P_WARNING, "slave tx cnt: %d", txCnt);
#endif
        if(txNum && txCnt)
        {
            if(pendingTxCnt > txCnt)
            {
                pendingTxCnt -= txCnt;
#ifdef SFDT_SLAVE_LOG_ENABLE
                ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_6, P_WARNING, "slave has pending tx, toal: %d, pending: %d", txNum, pendingTxCnt);
#endif
                txPtr += txCnt;
            }
            else
            {
                spiDevFreeTxBuf(SLAVE_DEVICE_INDEX, txBufDesc.buf);
                pendingTxCnt = 0;
                txNum = 0;
            }
        }

        // parse rx data
        header = (SFDTFrameHeader_t *)rxBuffer;
#ifdef SFDT_SLAVE_LOG_ENABLE
        ECPLAT_DUMP(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_7, P_WARNING, "slave rx dump:", 16, rxBuffer);
#endif
        if(SFDT_csum(header) == header->field.checkSum)
        {
            rxPayLoadLen = header->field.payLoadLen;

            if((rxPayLoadLen + SFDT_FRAME_HEADER_SIZE) <= txCnt)
            {
                rxNewBuffer = spiDevTryGetRxBuf(SLAVE_DEVICE_INDEX, rxPayLoadLen);

                if(header->field.bContinue == true)
                {
#ifdef SFDT_SLAVE_LOG_ENABLE
                    ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_8, P_WARNING, "slave frame continue flag is set");
#endif
                    osEventFlagsSet(gSlaveTransferTriggerFlag, SLAVE_TRANSFER_RX_FLAG);
                }

                // rx report, only if we have new buffer for rx, otherwise we have to overwrite the old one
                if(rxNewBuffer)
                {
                    spiDevInput(SLAVE_DEVICE_INDEX, rxPayLoadLen, header->field.flowControl);
                    rxBuffer = rxNewBuffer;
                }
            }
            else
            {
#ifdef SFDT_SLAVE_LOG_ENABLE
                ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveTransferTaskEntry_9, P_WARNING, "slave rx frame is corrupt, frame len: %d, payLoadLen: %d", txCnt, rxPayLoadLen);
#endif
            }

        }

    }
}


/*----------------------------------------------------------------------------*
 *                      GLOBAL FUNCTIONS                                      *
 *----------------------------------------------------------------------------*/
SFDT_SLAVE_MRDY_IRQ_ISR
{
#ifdef SFDT_SLAVE_LOG_ENABLE
    ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveMrdyIsr, P_WARNING, "slave got mrdy handshake");
#endif
    if(gSlaveTransferTriggerFlag)
    {
        osEventFlagsSet(gSlaveTransferTriggerFlag, SLAVE_TRANSFER_RX_FLAG);
    }
}

void SFDT_slaveSendTrigger(void)
{
    if(gSlaveTransferTriggerFlag)
    {
        osEventFlagsSet(gSlaveTransferTriggerFlag, SLAVE_TRANSFER_TX_FLAG);
    }

}

int32_t SFDT_slaveInit(void)
{
    int32_t ret = SFDT_slaveHwInit();

    if(ret != ARM_DRIVER_OK)
        return ret;

    SFDT_createSlaveTransferTask();

    return 0;
}

void SFDT_slaveSrdyInit(void)
{
    slpManAONIOPowerOn();
    slpManNormalIOVoltSet(IOVOLT_3_30V);
    slpManAONIOVoltSet(IOVOLT_3_30V);

    // SRDY pin(output)
    PadConfig_t padConfig;
    PAD_getDefaultConfig(&padConfig);

    padConfig.mux = SFDT_SLAVE_SRDY_PAD_ALT_FUNC;
    PAD_setPinConfig(SFDT_SLAVE_SRDY_PAD_INDEX, &padConfig);

    GpioPinConfig_t config;
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    config.misc.initOutput = 0; // Pull down SRDY for init state

    GPIO_pinConfig(SFDT_SLAVE_SRDY_GPIO_INSTANCE, SFDT_SLAVE_SRDY_GPIO_INDEX, &config);

}

void SFDT_slaveFlowControl(uint32_t control)
{
#if 0
    SFDTFlowControl_e setting = (SFDTFlowControl_e)control;

    uint32_t mask = SaveAndSetIRQMask();

    if(gSlaveFlowControlState != setting)
    {
#ifdef SFDT_SLAVE_LOG_ENABLE
        ECPLAT_PRINTF(UNILOG_PLA_APP, SFDT_slaveFlowControl_0, P_WARNING, "slave flow control setting: %d", setting);
#endif
        gSlaveFlowControlState = setting;

        if(gSlaveTransferTriggerFlag)
        {
            osEventFlagsSet(gSlaveTransferTriggerFlag, SLAVE_TRANSFER_FLOW_CONTROL_FLAG);
        }
    }

    RestoreIRQMask(mask);
#endif
}

