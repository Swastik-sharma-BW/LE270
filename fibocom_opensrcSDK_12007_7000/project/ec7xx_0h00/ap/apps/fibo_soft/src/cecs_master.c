/******************************************************************************

*(C) Copyright 2024 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description:
*
*  History: Rev1.0   2024-07-01
*
*  Notes:
*
******************************************************************************/

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/

#include "cecs.h"
#include DEBUG_LOG_HEADER_FILE

#if defined FEATURE_FREERTOS_ENABLE
#include "FreeRTOS.h"
#endif
#include "cmsis_os2.h"
#include "bsp_spi.h"
#include "slpman.h"
#include "spi_device.h"
#include "ccio_custom.h"
#include "CH390.h"
#include "psdial_net.h"

#ifdef FIBOCOM_BASE
#include "ffw_gpio.h"
#endif

#define CECS_MASTER_LOG_ENABLE
/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/
 #define CECS_DEVICE_INDEX                   (0)

#define MASTER_TRANSFER_TASK_STACK_SIZE       (1024)

#define MASTER_TRANSFER_TX_FLAG               (0x1)
#define MASTER_TRANSFER_RX_FLAG               (0x2)
#define MASTER_TRANSFER_TRIGGER_FLAG          (MASTER_TRANSFER_TX_FLAG | MASTER_TRANSFER_RX_FLAG)

#define MASTER_TRANSFER_DONE_FLAG             (0x1)

/*----------------------------------------------------------------------------*
 *                    DATA TYPE DEFINITION                                    *
 *----------------------------------------------------------------------------*/
	typedef struct
	{
		uint32_t  magic;	 /* "SPIC" */
		uint16_t  type;
		uint16_t  len;  /* including hdr size */
		uint32_t  rsvd;
	}initMsgHdr_t;

typedef enum{
	LINK_CHG_IDLE,
	LINK_CHG_INITIALIZING,
	LINK_CHG_LINK_UP,
	LINK_CHG_RESET,
	LINK_CHG_DE_INITIALIZING,
	LINK_CHG_LINK_DOWN
}linkChgStaus_E;

/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTION DECLEARATION                         *
 *----------------------------------------------------------------------------*/
static void rst(uint8_t level);
static void scs(uint8_t level);
static uint8_t spi_exchange_byte(uint8_t byte);
static uint32_t cecs_data_reveive(uint8_t *recvData);
static uint32_t cecs_data_send(uint8_t *sendData,uint16_t sendLen);
static void CECS_masterTransferTaskEntry(void *arg);


/*----------------------------------------------------------------------------*
 *                      GLOBAL VARIABLES                                      *
 *----------------------------------------------------------------------------*/

#if CECS_MASTER_INSTANCE == 0

#if RTE_SPI0 == 0
#error SPI0 shall be enabled in rte
#endif
#define CECS_SPI_SSN_GPIO_INSTANCE	RTE_SPI0_SSN_GPIO_INSTANCE
#define CECS_SPI_SSN_GPIO_INDEX		RTE_SPI0_SSN_GPIO_INDEX

extern ARM_DRIVER_SPI Driver_SPI0;
static ARM_DRIVER_SPI *spiMasterDrv = &CREATE_SYMBOL(Driver_SPI, 0);

#elif CECS_MASTER_INSTANCE == 1

#if RTE_SPI1 == 0
#error SPI1 shall be enabled in rte
#endif
#define CECS_SPI_SSN_GPIO_INSTANCE 	RTE_SPI1_SSN_GPIO_INSTANCE
#define	CECS_SPI_SSN_GPIO_INDEX		RTE_SPI1_SSN_GPIO_INDEX

extern ARM_DRIVER_SPI Driver_SPI1;
static ARM_DRIVER_SPI *spiMasterDrv = &CREATE_SYMBOL(Driver_SPI, 1);


#else
#error unsupported spi instance
#endif

static StaticTask_t             gMasterTransferTask;
static uint8_t                  gMasterTransferTaskStack[MASTER_TRANSFER_TASK_STACK_SIZE];

static osEventFlagsId_t         gMasterTransferTriggerFlag = NULL;
static osEventFlagsId_t         gMasterTransferDoneFlag = NULL;

ch390_interface_t ch390_interface = {
	rst,
	delay_us,
	scs,
	spi_exchange_byte,
};


/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTIONS                                     *
 *----------------------------------------------------------------------------*/
static void CECS_masterCallback(uint32_t event)
{
    osEventFlagsSet(gMasterTransferDoneFlag, MASTER_TRANSFER_DONE_FLAG);
}
static void rst(uint8_t level)
{
	ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_rst, P_DEBUG, "level [%d]", level);
	if(level)
		GPIO_pinWrite(CECS_MASTER_RST_GPIO_INSTANCE, 1 << CECS_MASTER_RST_GPIO_INDEX, 1 << CECS_MASTER_RST_GPIO_INDEX);
	else
		GPIO_pinWrite(CECS_MASTER_RST_GPIO_INSTANCE, 1 << CECS_MASTER_RST_GPIO_INDEX, 0);
}

static void scs(uint8_t level)
{
	if(level)
		GPIO_pinWrite(CECS_SPI_SSN_GPIO_INSTANCE, 1 << CECS_SPI_SSN_GPIO_INDEX, 1 << CECS_SPI_SSN_GPIO_INDEX);
	else
		GPIO_pinWrite(CECS_SPI_SSN_GPIO_INSTANCE, 1 << CECS_SPI_SSN_GPIO_INDEX, 0);
}

static uint8_t spi_exchange_byte(uint8_t byte)
{
	uint8_t u8Out = 0;

    spiMasterDrv->Transfer(&byte, &u8Out, 1);
	osEventFlagsWait(gMasterTransferDoneFlag, MASTER_TRANSFER_DONE_FLAG, osFlagsWaitAll, 5);
	return u8Out;
}

static uint32_t cecs_data_reveive(uint8_t *recvData)
{
	uint16_t rxLen;
	
	uint8_t rx_ready;
	uint8_t ReceiveData[4];

	ch390_read_reg(CH390_MRCMDX);
    rx_ready = ch390_read_reg(CH390_MRCMDX);
#ifdef CECS_MASTER_LOG_ENABLE
	ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_cecs_data_reveive, P_DEBUG, "rx_ready [0x%x]", rx_ready);
#endif
	if (rx_ready & CH390_PKT_ERR)
    {
        // Reset RX FIFO pointer
        ch390_write_reg(CH390_RCR, 0);        //RX disable
        ch390_write_reg(CH390_MPTRCR, 0x01);  //Reset RX FIFO pointer
        ch390_write_reg(CH390_MRRH, 0x0c);
        osDelay(1);
        ch390_write_reg(CH390_RCR, RCR_RXEN); //RX Enable
        return 0;
    }

	if(!(rx_ready & CH390_PKT_RDY))
	{
		return 0;
	}
	//ch390_read_mem(ReceiveData, 4);
	//rxLen = ReceiveData[2] | (ReceiveData[3] << 8);
	
	scs(0);
	spi_exchange_byte(OPC_MEM_READ);
	spiMasterDrv->Receive(ReceiveData,4);
	osEventFlagsWait(gMasterTransferDoneFlag, MASTER_TRANSFER_DONE_FLAG, osFlagsWaitAll, 5);
	rxLen = ReceiveData[2] | (ReceiveData[3] << 8);
	spiMasterDrv->Receive(recvData,rxLen);
	osEventFlagsWait(gMasterTransferDoneFlag, MASTER_TRANSFER_DONE_FLAG, osFlagsWaitAll, 5);
	scs(1);
	return (uint32_t)rxLen;
}

static uint32_t cecs_data_send(uint8_t *sendData,uint16_t sendLen)
{
	//ch390_write_mem(sendData, sendLen);
	scs(0);
	spi_exchange_byte(OPC_MEM_WRITE);
	spiMasterDrv->Send(sendData,sendLen);
	osEventFlagsWait(gMasterTransferDoneFlag, MASTER_TRANSFER_DONE_FLAG, osFlagsWaitAll, 5);
	scs(1);
	while(ch390_read_reg(CH390_TCR) & TCR_TXREQ){};
    // Set current packet length
    ch390_write_reg(CH390_TXPLL, sendLen & 0xff);
    ch390_write_reg(CH390_TXPLH, (sendLen >> 8) & 0xff);
    // Issue transmit request
    ch390_send_request();
	return 0;
}

#ifdef FIBOCOM_BASE
static void int_isrcallback(void *level)
{
#ifdef CECS_MASTER_LOG_ENABLE
    ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterIntIsr, P_WARNING, "master got INT");
#endif
    if(gMasterTransferTriggerFlag)
    {
        osEventFlagsSet(gMasterTransferTriggerFlag, MASTER_TRANSFER_RX_FLAG);
    }
}

static int32_t CECS_masterHwInit(void)
{
	int32_t ret = ARM_DRIVER_OK;

#if CECS_BUS_SPEED > 4000000
	apmuVoteToDozeState(PMU_DOZE_SPI_MOD, false);
	CLOCK_clockEnable(CLK_HF102M);
	CLOCK_setClockSrc(CLK_APB_MP, CLK_APB_MP_SEL_102M);

	// Switch clock before spi init
	CLOCK_clockEnable(CLK_HF612M);
	CLOCK_setClockSrc(FCLK_SPI0, FCLK_SPI0_SEL_612M);
#endif

    //config spi
    ffw_gpiomode_set(66, 1);
    ffw_gpiomode_set(67, 1);
    ffw_gpiomode_set(28, 1);
    ffw_gpiomode_set(29, 1);

	// Initialize master spi
	ret = spiMasterDrv->Initialize(CECS_masterCallback);
#ifdef CECS_MASTER_LOG_ENABLE
	ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterHwInit_0, P_DEBUG, "CECS_masterHwInit, init retCode: %d", ret);
#endif
	if(ret != ARM_DRIVER_OK)
		return ret;

	// Power on
	ret = spiMasterDrv->PowerControl(ARM_POWER_FULL);
#ifdef CECS_MASTER_LOG_ENABLE
	ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterHwInit_1, P_DEBUG, "CECS_masterHwInit, powerctl retCode: %d", ret);
#endif
	if(ret != ARM_DRIVER_OK)
		return ret;

	// Configure spi bus
	ret = spiMasterDrv->Control(ARM_SPI_MODE_MASTER | CECS_SPI_FRAME_FORMAT | ARM_SPI_DATA_BITS(8) |
								  ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW, CECS_BUS_SPEED);
#ifdef CECS_MASTER_LOG_ENABLE
	ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterHwInit, P_DEBUG, "CECS_masterHwInit, retCode: %d", ret);
#endif

	if(ret != ARM_DRIVER_OK)
		return ret;


	//spiMasterDrv->Control(ARM_SPI_SET_DEFAULT_TX_VALUE, 0xF0F0);


	// INT pin(input)
     ffw_gpioisr_t int_isr;
     memset(&int_isr,0,sizeof(int_isr));
     
    int_isr.intr_enable = true;
    int_isr.intr_level = false;
    int_isr.falling = false;
    int_isr.rising = true;
    int_isr.pullen = true;
    int_isr.pullup = true;
    int_isr.cb = int_isrcallback;

    ffw_gpiomode_set(53, 0);
    ffw_gpio_isr_init(53, &int_isr);

	return ret;
}
#else
static int32_t CECS_masterHwInit(void)
{
	int32_t ret = ARM_DRIVER_OK;

#if CECS_BUS_SPEED > 4000000
	apmuVoteToDozeState(PMU_DOZE_SPI_MOD, false);
	CLOCK_clockEnable(CLK_HF102M);
	CLOCK_setClockSrc(CLK_APB_MP, CLK_APB_MP_SEL_102M);

	// Switch clock before spi init
	CLOCK_clockEnable(CLK_HF612M);
	CLOCK_setClockSrc(FCLK_SPI0, FCLK_SPI0_SEL_612M);
#endif

	// Initialize master spi
	ret = spiMasterDrv->Initialize(CECS_masterCallback);
#ifdef CECS_MASTER_LOG_ENABLE
	ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterHwInit_0, P_DEBUG, "CECS_masterHwInit, init retCode: %d", ret);
#endif
	if(ret != ARM_DRIVER_OK)
		return ret;

	// Power on
	ret = spiMasterDrv->PowerControl(ARM_POWER_FULL);
#ifdef CECS_MASTER_LOG_ENABLE
	ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterHwInit_1, P_DEBUG, "CECS_masterHwInit, powerctl retCode: %d", ret);
#endif
	if(ret != ARM_DRIVER_OK)
		return ret;

	// Configure spi bus
	ret = spiMasterDrv->Control(ARM_SPI_MODE_MASTER | CECS_SPI_FRAME_FORMAT | ARM_SPI_DATA_BITS(8) |
								  ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW, CECS_BUS_SPEED);
#ifdef CECS_MASTER_LOG_ENABLE
	ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterHwInit, P_DEBUG, "CECS_masterHwInit, retCode: %d", ret);
#endif

	if(ret != ARM_DRIVER_OK)
		return ret;


	//spiMasterDrv->Control(ARM_SPI_SET_DEFAULT_TX_VALUE, 0xF0F0);


	// INT pin(input)
	APmuWakeupPadSettings_t wakeupPadSetting;

	if(slpManGetLastSlpState() == SLP_ACTIVE_STATE)
	{
		wakeupPadSetting.negEdgeEn = false;
		wakeupPadSetting.posEdgeEn = true;
		wakeupPadSetting.pullDownEn = false;
		wakeupPadSetting.pullUpEn = true;

		NVIC_DisableIRQ(CECS_MASTER_INT_IRQ);
		NVIC_ClearPendingIRQ(CECS_MASTER_INT_IRQ);

		slpManSetWakeupPadCfg(CECS_MASTER_INT_PAD, true, &wakeupPadSetting);
	}

	return ret;
}
#endif

static void CECS_createMasterTransferTask(void)
{
    if(gMasterTransferTriggerFlag)
    {
        return;
    }

    osThreadId_t threadId;
    osThreadAttr_t task_attr;
    //slpManRet_t ret = RET_TRUE;

    gMasterTransferTriggerFlag = osEventFlagsNew(NULL);

    EC_ASSERT(gMasterTransferTriggerFlag, gMasterTransferTriggerFlag, 0, 0);

    // Enable wakeup IRQ when we're ready
    NVIC_EnableIRQ(CECS_MASTER_INT_IRQ);

    gMasterTransferDoneFlag = osEventFlagsNew(NULL);

    EC_ASSERT(gMasterTransferDoneFlag, gMasterTransferDoneFlag, 0, 0);

    memset(&task_attr,0,sizeof(task_attr));
    memset(gMasterTransferTaskStack, 0xA5, MASTER_TRANSFER_TASK_STACK_SIZE);
    task_attr.name = "cecs master";
    task_attr.stack_mem = gMasterTransferTaskStack;
    task_attr.stack_size = MASTER_TRANSFER_TASK_STACK_SIZE;
    task_attr.priority = osPriorityNormal2;
    task_attr.cb_mem = &gMasterTransferTask;//task control block
    task_attr.cb_size = sizeof(StaticTask_t);//size of task control block

    threadId = osThreadNew(CECS_masterTransferTaskEntry, NULL, &task_attr);

    EC_ASSERT(threadId, threadId, 0, 0);

}

PLAT_FM_RAMCODE static void CECS_masterTransferTaskEntry(void *arg)
{
    SpiDevConf_t spiDevConf = {
#if (RTE_EOS_EN == 1)
    .mainUsage = CSIO_DT_ETH,
    .custFlags = CSIO_OCF_SPITEST,
#else
    .mainUsage = CSIO_DT_AT,
    .custFlags = CSIO_OCF_SPITEST,
#endif
    .bmCreateFlag = CCIO_TASK_FLAG_RX | CCIO_TASK_FLAG_TX2,
    .rbufFlags = CUST_RBUF_FOR_SPI
    };

    int32_t errCode = CCIO_EOK;

#ifdef CECS_MASTER_LOG_ENABLE
    uint32_t loop = 0;
#endif

	uint8_t int_status = 0;

    uint32_t eventFlags = 0;

    uint32_t txNum = 0;

    uint32_t rxPayLoadLen = 0;

	uint8_t linkChgStatus = LINK_CHG_IDLE;

	uint32_t eventTimeout = osWaitForever;
	
    uint8_t* rxBuffer = NULL, *rxNewBuffer = NULL, *txPtr = NULL;
	initMsgHdr_t *spicInitPtr = NULL;
    CcioBufDesc_t txBufDesc;

    errCode = spiDevCreate(CECS_DEVICE_INDEX, &spiDevConf);

    EC_ASSERT(errCode == CCIO_EOK, errCode, 0, 0);

    // alloc rx buffer
    rxBuffer = spiDevTryGetRxBuf(CECS_DEVICE_INDEX, 0);
    EC_ASSERT(rxBuffer, rxBuffer, 0, 0);
	
    while(1)
    {
#ifdef CECS_MASTER_LOG_ENABLE
        ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_0, P_WARNING, "master transfer task loop: %d", loop++);
#endif
        eventFlags = 0;
		switch(linkChgStatus)
		{
			case LINK_CHG_IDLE:
			{
				spicInitPtr = (initMsgHdr_t*)rxBuffer;
				spicInitPtr->magic = 0x43495053;/* 'C','I','P','S' */
				spicInitPtr->type = 0x0402;/*SPIC_NET_INITIAL_MSG*/
				spicInitPtr->len = sizeof(initMsgHdr_t);
				spicInitPtr->rsvd = 0;
				rxNewBuffer = spiDevTryGetRxBuf(CECS_DEVICE_INDEX, spicInitPtr->len);
				if(rxNewBuffer)
			    {
			        spiDevInput(CECS_DEVICE_INDEX, spicInitPtr->len, 0);
			        rxBuffer = rxNewBuffer;
					linkChgStatus = LINK_CHG_INITIALIZING;
			    }
			}
			break;
			case LINK_CHG_RESET:
			{
				spicInitPtr = (initMsgHdr_t*)rxBuffer;
				spicInitPtr->magic = 0x43495053;/* 'C','I','P','S' */
				spicInitPtr->type = 0x0401;/*SPIC_NET_RESET_MSG*/
				spicInitPtr->len = sizeof(initMsgHdr_t);
				spicInitPtr->rsvd = 0;
				rxNewBuffer = spiDevTryGetRxBuf(CECS_DEVICE_INDEX, spicInitPtr->len);
				if(rxNewBuffer)
			    {
			        spiDevInput(CECS_DEVICE_INDEX, spicInitPtr->len, 0);
			        rxBuffer = rxNewBuffer;
					linkChgStatus = LINK_CHG_DE_INITIALIZING;
			    }
			}
			break;
			default:
			break;
		}
        // Poll tx list to check if we have something to send
CHECK_TX:
        errCode = spiDevPickTxBuf(CECS_DEVICE_INDEX, &txBufDesc);
        if((errCode == CCIO_EFOUND) && (eventFlags == 0))
        {
            // tx list is empty, wait for tx or rx trigger
            eventFlags = osEventFlagsWait(gMasterTransferTriggerFlag, MASTER_TRANSFER_TRIGGER_FLAG, osFlagsWaitAny, eventTimeout);
            //EC_ASSERT((eventFlags & MASTER_TRANSFER_TRIGGER_FLAG) != 0, eventFlags, 0 , 0);
            if(osErrorTimeout == eventFlags)
        	{
        		uint8_t gpr = ch390_read_reg(CH390_GPR);
				ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_001, P_WARNING, "CH390 REG GPR [0x%x]", gpr);

				uint8_t imr = ch390_read_reg(CH390_IMR);
				ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_002, P_WARNING, "CH390 REG IMR [0x%x]", imr);
				if(gpr == 0x01 && imr == 0x00)
				{
					ch390_default_config();
				}
        	}

			if(eventFlags & MASTER_TRANSFER_TX_FLAG)
			{
				goto CHECK_TX;
			}
        }
        else if(errCode == CCIO_EOK)
        {
            // noise suppress
            osEventFlagsClear(gMasterTransferTriggerFlag, MASTER_TRANSFER_TX_FLAG);

            txPtr = txBufDesc.buf;
            txNum = txBufDesc.len;
			spicInitPtr = (initMsgHdr_t*)txBufDesc.buf;
			if(spicInitPtr->magic == 0x43495053/**/)
			{
#ifdef CECS_MASTER_LOG_ENABLE
				ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_01, P_WARNING, "spic rsp type[0x%x]", spicInitPtr->type);
#endif
				switch(spicInitPtr->type)
				{
					case 0x8402/*SPIC_NET_INITIAL_CMPLT*/:
						linkChgStatus = LINK_CHG_LINK_UP;
					break;
					case 0x8401/*SPIC_NET_RESET_CMPLT*/:
						linkChgStatus = LINK_CHG_LINK_DOWN;
					break;
					default:
						break;
				}
				spiDevFreeTxBuf(CECS_DEVICE_INDEX, txBufDesc.buf);
            	txNum = 0;
				goto CHECK_TX; 
			}

#ifdef CECS_MASTER_LOG_ENABLE
            ECPLAT_DUMP(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_2, P_WARNING, "master tx dump:", txNum, txPtr);
#endif
        }

        spiMasterDrv->Control(ARM_SPI_PURGE_COMM, 0);

		if(txNum != 0)
		{
			cecs_data_send(txPtr, (uint16_t)txNum);
			spiDevFreeTxBuf(CECS_DEVICE_INDEX, txBufDesc.buf);
            txNum = 0;
		}

    	int_status = ch390_get_int_status();
#ifdef CECS_MASTER_LOG_ENABLE
		ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_3, P_WARNING, "int_status [0x%x]", int_status);
#endif
		if(int_status & ISR_LNKCHG)
		{
			osDelay(100);
			ch390_write_phy(0x1f,0);
			while(ch390_read_phy(0x1e));
			if(ch390_get_link_status())
			{
#ifdef CECS_MASTER_LOG_ENABLE
				ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_4, P_WARNING, "Link up");
#endif
				linkChgStatus = (linkChgStatus >= LINK_CHG_RESET ? LINK_CHG_IDLE : linkChgStatus);
				ch390_write_reg(CH390_ISR,ISR_LNKCHG);
				eventTimeout = osWaitForever;
			}
			else
			{
				ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_5, P_WARNING, "Link failed");
				eventTimeout = CECSTIMEOUT_AFTER_LINK_CHG;
				linkChgStatus = LINK_CHG_RESET;
			}
		}
		if(int_status & ISR_ROS)
		{
#ifdef CECS_MASTER_LOG_ENABLE
			ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_6, P_WARNING, "receive overflow");
#endif
		}
		if(int_status & ISR_ROO)
		{
#ifdef CECS_MASTER_LOG_ENABLE
			ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_7, P_WARNING, "Overflow counter overflow");
#endif
		}
		if(int_status & ISR_PR)
		{
			do
			{
				rxPayLoadLen = cecs_data_reveive(rxBuffer);
				if(rxPayLoadLen)
				{
#ifdef CECS_MASTER_LOG_ENABLE
					ECPLAT_DUMP(UNILOG_PLA_APP, CECS_masterTransferTaskEntry_8, P_WARNING, "master rx dump:", rxPayLoadLen, rxBuffer);
#endif
					rxNewBuffer = spiDevTryGetRxBuf(CECS_DEVICE_INDEX, rxPayLoadLen);
					if(rxNewBuffer)
					{
						spiDevInput(CECS_DEVICE_INDEX, rxPayLoadLen, 0);
						rxBuffer = rxNewBuffer;
					}
				}
			}while(rxPayLoadLen != 0);
		}          

    }
}


/*----------------------------------------------------------------------------*
 *                      GLOBAL FUNCTIONS                                      *
 *----------------------------------------------------------------------------*/

CECS_MASTER_INT_IRQ_ISR
{
#ifdef CECS_MASTER_LOG_ENABLE
    ECPLAT_PRINTF(UNILOG_PLA_APP, CECS_masterIntIsr, P_WARNING, "master got INT");
#endif
    if(gMasterTransferTriggerFlag)
    {
        osEventFlagsSet(gMasterTransferTriggerFlag, MASTER_TRANSFER_RX_FLAG);
    }
}

void CECS_masterSendTrigger(void)
{
	if(gMasterTransferTriggerFlag)
	{
		osEventFlagsSet(gMasterTransferTriggerFlag, MASTER_TRANSFER_TX_FLAG);
	}

}

int32_t CECS_MasterInit(void)
{
    int32_t ret = CECS_masterHwInit();
	
    if(ret != ARM_DRIVER_OK)
        return ret;
	ch390_interface_register(ch390_interface);
#ifdef FIBOCOM_BASE
    ch390_hardware_reset();
#endif
	osDelay(10);
	ch390_default_config();
    CECS_createMasterTransferTask();

    return 0;
}

void CECS_MasterGpioInit(void)
{
    slpManAONIOPowerOn();
#ifdef FIBOCOM_BASE
    slpManNormalIOVoltSet(IOVOLT_3_30V);
    slpManAONIOVoltSet(IOVOLT_3_30V);
#endif
    PadConfig_t padConfig;
    PAD_getDefaultConfig(&padConfig);

    padConfig.mux = CECS_MASTER_RST_PAD_ALT_FUNC;
    PAD_setPinConfig(CECS_MASTER_RST_PAD_INDEX, &padConfig);

    GpioPinConfig_t config;
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    config.misc.initOutput = 1; // Pull down SRDY for init state

    GPIO_pinConfig(CECS_MASTER_RST_GPIO_INSTANCE, CECS_MASTER_RST_GPIO_INDEX, &config);

}

