/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description:
*
*  History: initiated by xxxx
*
*  Notes:
*
******************************************************************************/

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"
#include "ex_flash.h"
#include "sctdef.h"
#ifdef FIBOCOM_BASE
#include DEBUG_LOG_HEADER_FILE
#include "cmsis_os2.h"
#endif

/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/
#define TRANSFER_DATA_WIDTH    (8)
#define SPI_SSN_GPIO_INSTANCE   RTE_SPI0_SSN_GPIO_INSTANCE
#define SPI_SSN_GPIO_INDEX      RTE_SPI0_SSN_GPIO_INDEX
#ifdef FIBOCOM_BASE
#define SPI1_SSN_GPIO_INSTANCE   RTE_SPI1_SSN_GPIO_INSTANCE
#define SPI1_SSN_GPIO_INDEX      RTE_SPI1_SSN_GPIO_INDEX
static osMutexId_t  gFlashexMutex    = NULL;
static osMutexId_t  gFlashexWriteMutex    = NULL;

#define EXT_FLASH_UNIQUEID      16
#define EXT_FLASH_OTP_SIZE      1024
#define EXT_FLASH_OTP_NUM       3
#endif


/*----------------------------------------------------------------------------*
 *                    DATA TYPE DEFINITION                                    *
 *----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*
 *                      GLOBAL VARIABLES                                      *
 *----------------------------------------------------------------------------*/

/** \brief driver instance declare */
extern ARM_DRIVER_SPI Driver_SPI0;
static ARM_DRIVER_SPI *spiMasterDrv = &CREATE_SYMBOL(Driver_SPI, 0);
#ifdef FIBOCOM_BASE
static osEventFlagsId_t gSpiFlags = NULL;
#ifdef CHIP_EC718
extern ARM_DRIVER_SPI Driver_SPI1;
static ARM_DRIVER_SPI *spiMasterDrv1 = &CREATE_SYMBOL(Driver_SPI, 1);
#endif
#endif
//volatile uint16_t gFid = 0;
#ifdef FIBOCOM_BASE
struct flash_info_t
{
    uint32_t id;
    uint32_t size;
};
//flash model
struct flash_info_t sg_flash_list[] =
{
    {0x8513,0x100000},//P25Q80H
    {0x8514,0x200000},//P25Q16SH
    {0x8515,0x400000},//P25Q32SH
    {0x8516,0x800000},//P25Q64SL
    {0x8517,0x1000000},//PY25Q128LA
    {0x8518,0x2000000},//P25Q256L
    {0x2515,0x400000},//SK25LE032
    {0x2516,0x800000},//SK25LE064
    {0x2015,0x400000},//XM25QH32CHIG
    {0xc816,0x800000},//GD25LQ64CWIGR
    {0x6816,0x800000},//BY25FQ64E
    {0x5e16,0x800000} //ZB25LQ64A
};
const size_t sg_flash_list_length = sizeof(sg_flash_list) / sizeof(sg_flash_list[0]);
#endif


/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTION DECLEARATION                         *
 *----------------------------------------------------------------------------*/

#ifdef FIBOCOM_BASE
/**
  \fn          void SPI_Callback()
  \brief       SPI callback
  \return
*/
static void SPI_Callback(uint32_t event)
{
    if (event & ARM_SPI_EVENT_TRANSFER_COMPLETE)
    {
        osEventFlagsSet(gSpiFlags, 0x01);
    }
    else
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, SPI_Callback, P_ERROR, "event=0x%X", event);
    }
}
#endif


/**
  \fn        exSpiFlashIoIint(void)
  \brief     init SPI interface
  \param[in]
  \note
 */
static void exSpiFlashIoIint(void)
{
#ifdef FIBOCOM_BASE
    spiMasterDrv->SetMode(DMA_MODE);
#endif
    // Initialize master spi
#ifdef FIBOCOM_BASE
    if (gSpiFlags == NULL)
    {
        gSpiFlags = osEventFlagsNew(NULL);
    }
    spiMasterDrv->Initialize(SPI_Callback);
#else
    spiMasterDrv->Initialize(NULL);
#endif

    // Power on
    spiMasterDrv->PowerControl(ARM_POWER_FULL);

    // Configure master spi bus
    spiMasterDrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_DATA_BITS(TRANSFER_DATA_WIDTH) |
                          ARM_SPI_MSB_LSB     | ARM_SPI_SS_MASTER_SW, 26000000U);
}


/**
  \fn        exSpiFlashIoDeIint(void)
  \brief     deinit SPI interface
  \param[in]
  \note
 */
static void exSpiFlashIoDeIint(void)
{
    // Initialize master spi
    spiMasterDrv->Uninitialize();
    // Power on
    spiMasterDrv->PowerControl(ARM_POWER_OFF);

}



/**
  \fn        exFlashSetSpiCsHigh(void)
  \brief     manual control CS PIN
  \param[in]
  \note
 */
static void exFlashSetSpiCsHigh(void)
{
    GPIO_pinWrite(SPI_SSN_GPIO_INSTANCE, 1 << SPI_SSN_GPIO_INDEX, 1 << SPI_SSN_GPIO_INDEX);
}



/**
  \fn        exFlashSetSpiCsLow(void)
  \brief     manual control CS PIN
  \param[in]
  \note
 */
static void exFlashSetSpiCsLow(void)
{
    GPIO_pinWrite(SPI_SSN_GPIO_INSTANCE, 1 << SPI_SSN_GPIO_INDEX, 0);
}

#ifdef FIBOCOM_BASE
void spiFlashWriteCmdAndAddr(uint8_t cmd, uint32_t addr)
{
    uint8_t dataOut[4] = {cmd, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};
    uint8_t dataIn[4]  = {0};

    spiMasterDrv->Transfer(dataOut, dataIn, sizeof(dataOut));
    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 10) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, spiFlashWriteCmdAndAddr, P_ERROR, "Write CMD and address timeout.\r\n");
    }

}
#endif

/**
  \fn        exflashWrByte(void)
  \brief     Software SPI_Flash bus driver basic function, send a single byte to MOSI,
    *        and accept MISO data at the same time. used for both cmd/data send
  \param[in] u8Data:Data sent on the MOSI data line
  \note
*/
static uint8_t exflashWrByte(uint8_t u8Data)
{
    uint8_t u8Out = 0;

    spiMasterDrv->Transfer(&u8Data, &u8Out, 1);
#ifdef FIBOCOM_BASE
    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 10) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exflashWrByte, P_ERROR,"Write byte timeout.\r\n");
    }
#endif

    return u8Out;
}



/**
  \fn        exFlashWrEn(void)
  \brief     send wr en cmd to flash
  \param[in]
  \note
 */
static void exFlashWrEn(void)
{
    exFlashSetSpiCsLow();
    exflashWrByte(WRITE_ENABLE_CMD);
    exFlashSetSpiCsHigh();
}


/**
  \fn        exFlashWrDisen(void)
  \brief     send wr disen cmd to flash
  \param[in]
  \note
 */
static void exFlashWrDisen(void)
{
    exFlashSetSpiCsLow();
    exflashWrByte(WRITE_DISABLE_CMD);
    exFlashSetSpiCsHigh();
}



/**
  \fn        exFlashPollingBusyFlag(void)
  \brief     read the BUSY field in flash reg and loop until done
  \param[in]
  \note
 */
static void exFlashPollingBusyFlag(void)
{
    uint8_t u8test;
    exFlashSetSpiCsLow();
#ifdef FIBOCOM_BASE
    for(uint32_t i = 0; i <30000; i++)
    {
        osDelay(1);
        exflashWrByte(READ_STATUS_REG1_CMD);
        u8test = exflashWrByte(Dummy_Byte1);
        if((u8test & 0x01) == 0x00)break;
    }
#else
    do
    {
        exflashWrByte(READ_STATUS_REG1_CMD);
        u8test = exflashWrByte(Dummy_Byte1);
    } while ((u8test & 0x01) == 0x01);
#endif
    exFlashSetSpiCsHigh();
}







/*----------------------------------------------------------------------------*
 *                      GLOBAL FUNCTIONS                                      *
 *----------------------------------------------------------------------------*/


/**
  \fn        exFlashChipErase(void)
  \brief     perform chip level erase, use with caution!!
  \param[in]
  \note
*/
void exFlashChipErase(void)
{
#ifdef FIBOCOM_BASE
    osMutexAcquire(gFlashexMutex, osWaitForever);
#endif
    exFlashWrEn();
    exFlashPollingBusyFlag();
    exFlashSetSpiCsLow();
    exflashWrByte(CHIP_ERASE_CMD);
    exFlashSetSpiCsHigh();
    exFlashPollingBusyFlag();
    exFlashWrDisen();
#ifdef FIBOCOM_BASE
    osMutexRelease(gFlashexMutex);
#endif
}



/**
  \fn        exFlashBlockErase(uint32_t u32Erase_Addr, uint8_t u8mode)
  \brief     perform block level erase
  \param[in] u32Data_Addr  :Block first address to start erasing
             u8mode        :Erase mode 1=32K other=64K
  \note
*/
void exFlashBlockErase(uint32_t u32Erase_Addr, uint8_t u8mode)
{
#ifdef FIBOCOM_BASE
    osMutexAcquire(gFlashexMutex, osWaitForever);
#endif
    exFlashWrEn();
    exFlashPollingBusyFlag();
    exFlashSetSpiCsLow();
#ifdef FIBOCOM_BASE
    if (u8mode == 1)
    {
        spiFlashWriteCmdAndAddr(BLOCK_ERASE_32K_CMD, u32Erase_Addr);
    }
    else
    {
        spiFlashWriteCmdAndAddr(BLOCK_ERASE_64K_CMD, u32Erase_Addr);
    }
#else
    if (u8mode == 1)
    {
        exflashWrByte(BLOCK_ERASE_32K_CMD);
    }
    else
    {
        exflashWrByte(BLOCK_ERASE_64K_CMD);
    }

    exflashWrByte(u32Erase_Addr >> 16);
    exflashWrByte(u32Erase_Addr >> 8);
    exflashWrByte(u32Erase_Addr);
#endif
    exFlashSetSpiCsHigh();
    exFlashPollingBusyFlag();
    exFlashWrDisen();
#ifdef FIBOCOM_BASE
    osMutexRelease(gFlashexMutex);
#endif
}



/**
  \fn        exFlashSectorErase(uint32_t u32Erase_Addr)
  \brief     perform sector level erase, normally sector is 4K
  \param[in] u32Data_Addr  :Block first address to start erasing
  \note
*/
void exFlashSectorErase(uint32_t u32Erase_Addr)
{
#ifdef FIBOCOM_BASE
    osMutexAcquire(gFlashexMutex, osWaitForever);
#endif
    exFlashWrEn();
    exFlashPollingBusyFlag();
    exFlashSetSpiCsLow();
#ifdef FIBOCOM_BASE
    spiFlashWriteCmdAndAddr(SECTOR_ERASE_CMD, u32Erase_Addr);
#else
    exflashWrByte(SECTOR_ERASE_CMD);

    exflashWrByte(u32Erase_Addr >> 16);
    exflashWrByte(u32Erase_Addr >> 8);
    exflashWrByte(u32Erase_Addr);
#endif
    exFlashSetSpiCsHigh();
    exFlashPollingBusyFlag();
    exFlashWrDisen();
#ifdef FIBOCOM_BASE
    osMutexRelease(gFlashexMutex);
#endif
}






/**
  \fn        exFlashPagePro(void)
  \brief     starts writing data of up to 256 bytes at a specified address on one page (0~65535)
  \param[in] u32ReadAddr       Start reading address(24bit)
             pu8Buffer:Data storage buffer
             u16NumByteToWrite:The number of bytes to write (maximum 256),
  *          the number should not exceed the number of remaining bytes on the page!!!
  \note
*/
void exFlashPagePro(uint32_t u32WriteAddr, uint8_t *pu8Buffer, uint16_t u16NumByteToWrite)
{
#ifdef FIBOCOM_BASE
    osMutexAcquire(gFlashexMutex, osWaitForever);
#else
    uint16_t i;
#endif
    exFlashWrEn();
    exFlashSetSpiCsLow();
#ifdef FIBOCOM_BASE
    spiFlashWriteCmdAndAddr(PAGE_PROG_CMD, u32WriteAddr);
    uint8_t pu8RdBuffer[u16NumByteToWrite];
    CLOCK_clockEnable(CLK_FLASH);
    spiMasterDrv->Transfer(pu8Buffer, pu8RdBuffer, u16NumByteToWrite);
    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashPagePro, P_ERROR,"Write page timeout.\r\n");
    }
    CLOCK_clockDisable(CLK_FLASH);
#else
    exflashWrByte(PAGE_PROG_CMD);
    exflashWrByte((uint8_t)((u32WriteAddr) >> 16));
    exflashWrByte((uint8_t)((u32WriteAddr) >> 8));
    exflashWrByte((uint8_t)u32WriteAddr);

    for (i = 0; i < u16NumByteToWrite; i++)exflashWrByte(pu8Buffer[i]);
#endif
    exFlashSetSpiCsHigh();
    exFlashPollingBusyFlag();
    exFlashWrDisen();
#ifdef FIBOCOM_BASE
    osMutexRelease(gFlashexMutex);
#endif
}


/**
  \fn        exFlashReadMDID(void)
  \brief     reading device ID from flash
  \param[in]
  \note
*/
uint16_t exFlashReadMDID(void)
{
#ifdef FIBOCOM_BASE
    osMutexAcquire(gFlashexMutex, osWaitForever);
#endif
    uint16_t        u16Temp = 0;
    /* Enable chip select */
    exFlashSetSpiCsLow();
    /* Send "RDID " instruction */
#ifdef FIBOCOM_BASE
    spiFlashWriteCmdAndAddr(READ_ID_CMD, 0);
#else
    exflashWrByte(READ_ID_CMD);
    exflashWrByte(0x00);
    exflashWrByte(0x00);
    exflashWrByte(0x00);
#endif
    /* Read a byte from the FLASH */
    u16Temp |= exflashWrByte(Dummy_Byte1) << 8;
    u16Temp |= exflashWrByte(Dummy_Byte1);
    /* Disable chip select */
    exFlashSetSpiCsHigh();
#ifdef FIBOCOM_BASE
    osMutexRelease(gFlashexMutex);
#endif
    return u16Temp;
}




/**
  \fn        uint8_t exFlashRead(uint8_t *pu8Buffer, uint32_t u32ReadAddr, uint16_t u16NumByteToRead)
  \brief     reading data of the specified length at the specified address
  \param[in] u32ReadAddr       Start reading address(24bit)
             pu8Buffer         Data storage buffer
             u16NumByteToRead  The number of bytes to read(max 65535)
  \note
*/
uint8_t exFlashRead(uint8_t *pu8Buffer, uint32_t u32ReadAddr, uint16_t u16NumByteToRead)
{
#ifdef FIBOCOM_BASE
    osMutexAcquire(gFlashexMutex, osWaitForever);
#else
    uint16_t i;
#endif
    exFlashSetSpiCsLow();/* Enable chip select */
#ifdef FIBOCOM_BASE
    spiFlashWriteCmdAndAddr(READ_CMD, u32ReadAddr);
    uint32_t index  = 0;
    uint32_t size   = 8191;
    uint32_t count  = u16NumByteToRead / size;
    uint32_t length = u16NumByteToRead % size;

    CLOCK_clockEnable(CLK_FLASH);
    for (index=0; index<count; index++)
    {
        spiMasterDrv->Transfer(pu8Buffer, &pu8Buffer[index * size], size);
        if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashRead_1, P_ERROR,"Read timeout 1.\r\n");
        }
    }
    if (length > 0)
    {
        spiMasterDrv->Transfer(pu8Buffer, &pu8Buffer[index * size], length);
        if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashRead_2, P_ERROR,"Read timeout 2.\r\n");
        }
    }
    CLOCK_clockDisable(CLK_FLASH);
#else
    exflashWrByte(READ_CMD);
    exflashWrByte(u32ReadAddr >> 16);
    exflashWrByte(u32ReadAddr >> 8);
    exflashWrByte(u32ReadAddr);

    for (i = 0; i < u16NumByteToRead; i++)
    {
        pu8Buffer[i] = exflashWrByte(Dummy_Byte1); //Read one byte
    }
#endif
    exFlashSetSpiCsHigh();/* Disable chip select */
#ifdef FIBOCOM_BASE
    osMutexRelease(gFlashexMutex);
#endif
    return EXFLASH_OK;
}



/**
  \fn        exFlashErase(uint32_t eAddr, uint32_t size)
  \brief     Erases the flash region. use different erase cmd according to size
  \param[in] eAddr addr to erase
             size  erase len
  \note
*/
uint8_t exFlashErase(uint32_t eAddr, uint32_t size)
{
    uint32_t offsetAddress;
    uint32_t remainLen;
    uint32_t currEraseSize;


    if ((eAddr&0xfff) !=0)
    {
        return EXFLASH_ERROR;
    }

    offsetAddress = eAddr;
    remainLen = size;

    while(remainLen > 0)
    {
        if (((offsetAddress&0xffff) ==0) && (remainLen >=0x10000))
        {
            exFlashBlockErase(offsetAddress,0);
            remainLen -= 0x10000;
            currEraseSize = 0x10000;
        }
        else if (((offsetAddress&0x7fff) ==0)&& (remainLen >=0x8000))
        {
            exFlashBlockErase(offsetAddress,1);
            remainLen-=0x8000;
            currEraseSize = 0x8000;
        }
        else
        {
            exFlashSectorErase(offsetAddress);
            currEraseSize = (remainLen >= 0x1000) ? 0x1000 : remainLen;
            remainLen -= currEraseSize;
        }

        offsetAddress += currEraseSize;
    }

    return EXFLASH_OK;
}




/**
  \fn        exFlashWrite(uint8_t* pData, uint32_t WriteAddr, uint32_t size)
  \brief     Writes an amount of data to the QSPI flash.
  \param[in] pData:  Data Pointer to write
             WriteAddr:  Write start address
             Size:  Size of data to write
  \note
*/
uint8_t exFlashWrite(uint8_t* pData, uint32_t WriteAddr, uint32_t size)
{
#ifdef FIBOCOM_BASE
    osMutexAcquire(gFlashexWriteMutex, osWaitForever);
#endif
    uint32_t end_addr, current_size, current_addr;
    //uint32_t irqMask;

    // Calculation of the size between the write address and the end of the page
    //write non-aligned bytes first
    current_size = PAGE_SIZE - (WriteAddr%PAGE_SIZE);

    // Check if the size of the data is less than the remaining place in the page
    if (current_size > size)
    {
        current_size = size;
    }

    // Initialize the adress variables
    current_addr = WriteAddr;
    end_addr = WriteAddr + size;

    // Perform the write page by page
    do
    {
        //irqMask = SaveAndSetIRQMask();

        exFlashPagePro(current_addr,pData,current_size);

        // Update the address and size variables for next page programming
        current_addr += current_size;
        pData += current_size;
        current_size = ((current_addr + PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : PAGE_SIZE;

        //RestoreIRQMask(irqMask);
    } while (current_addr < end_addr);
#ifdef FIBOCOM_BASE
    osMutexRelease(gFlashexWriteMutex);
#endif
    return EXFLASH_OK;
}




/**
  \fn        exFlashInit(void)
  \brief     init the external spi flash
  \param[in]
  \note
*/
uint8_t exFlashInit(void)
{
#ifdef FIBOCOM_BASE
    gFlashexMutex = osMutexNew(NULL);
    if(gFlashexMutex == NULL)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashInit, P_ERROR,"Failed to create mutex for gFlashexMutex\r\n");
        return EXFLASH_ERROR;
    }
#endif
    exSpiFlashIoIint();
    //gFid = exFlashReadMDID();

    return EXFLASH_OK;
}


/**
  \fn        exFlashDeinit(void)
  \brief     deinit the external spi flash
  \param[in]
  \note
*/
uint8_t exFlashDeinit(void)
{
    exSpiFlashIoDeIint();
#ifdef FIBOCOM_BASE
    osMutexDelete(gFlashexMutex);
#endif

    return EXFLASH_OK;
}

#ifdef FIBOCOM_BASE
#ifdef CHIP_EC718
static void exFlashSetSpiCsHighSpi1(void)
{
    GPIO_pinWrite(SPI1_SSN_GPIO_INSTANCE, 1 << SPI1_SSN_GPIO_INDEX, 1 << SPI1_SSN_GPIO_INDEX);
}

static void exFlashSetSpiCsLowSpi1(void)
{
    GPIO_pinWrite(SPI1_SSN_GPIO_INSTANCE, 1 << SPI1_SSN_GPIO_INDEX, 0);
}

void spiFlashWriteCmdAndAddrSpi1(uint8_t cmd, uint32_t addr)
{
    uint8_t dataOut[4] = {cmd, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};
    uint8_t dataIn[4]  = {0};

    spiMasterDrv1->Transfer(dataOut, dataIn, sizeof(dataOut));
    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 10) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, spiFlashWriteCmdAndAddrSpi1, P_ERROR, "Write CMD and address timeout.\r\n");
    }

}

static uint8_t exflashWrByteSpi1(uint8_t u8Data)
{
    uint8_t u8Out = 0;

    spiMasterDrv1->Transfer(&u8Data, &u8Out, 1);
    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 10) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exflashWrByteSpi1, P_ERROR,"Write byte timeout.\r\n");
    }

    return u8Out;
}

static void exFlashWrEnSpi1(void)
{
    exFlashSetSpiCsLowSpi1();
    exflashWrByteSpi1(WRITE_ENABLE_CMD);
    exFlashSetSpiCsHighSpi1();
}

static void exFlashWrDisenSpi1(void)
{
    exFlashSetSpiCsLowSpi1();
    exflashWrByteSpi1(WRITE_DISABLE_CMD);
    exFlashSetSpiCsHighSpi1();
}

static void exFlashPollingBusyFlagSpi1(void)
{
    uint8_t u8test;
    exFlashSetSpiCsLowSpi1();

    for(uint32_t i = 0; i <30000; i++)
    {
        osDelay(1);
        exflashWrByteSpi1(READ_STATUS_REG1_CMD);
        u8test = exflashWrByteSpi1(Dummy_Byte1);
        if((u8test & 0x01) == 0x00)break;
    }

    exFlashSetSpiCsHighSpi1();
}

void exFlashSectorEraseSpi1(uint32_t u32Erase_Addr)
{
    osMutexAcquire(gFlashexMutex, osWaitForever);

    exFlashWrEnSpi1();
    exFlashPollingBusyFlagSpi1();
    exFlashSetSpiCsLowSpi1();

    spiFlashWriteCmdAndAddrSpi1(SECTOR_ERASE_CMD, u32Erase_Addr);

    exFlashSetSpiCsHighSpi1();
    exFlashPollingBusyFlagSpi1();
    exFlashWrDisenSpi1();
    osMutexRelease(gFlashexMutex);
}

void exFlashPageProSpi1(uint32_t u32WriteAddr, uint8_t *pu8Buffer, uint16_t u16NumByteToWrite)
{
    osMutexAcquire(gFlashexMutex, osWaitForever);

    exFlashWrEnSpi1();
    exFlashSetSpiCsLowSpi1();
    spiFlashWriteCmdAndAddrSpi1(PAGE_PROG_CMD, u32WriteAddr);
    uint8_t pu8RdBuffer[u16NumByteToWrite];
    CLOCK_clockEnable(CLK_FLASH);
    spiMasterDrv1->Transfer(pu8Buffer, pu8RdBuffer, u16NumByteToWrite);
    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashPageProSpi1, P_ERROR,"Write page timeout.\r\n");
    }
    CLOCK_clockDisable(CLK_FLASH);


    exFlashSetSpiCsHighSpi1();
    exFlashPollingBusyFlagSpi1();
    exFlashWrDisenSpi1();
    osMutexRelease(gFlashexMutex);
}

uint16_t exFlashReadMDIDSpi1(void)
{
    osMutexAcquire(gFlashexMutex, osWaitForever);

    uint16_t        u16Temp = 0;
    /* Enable chip select */
    exFlashSetSpiCsLowSpi1();
    /* Send "RDID " instruction */
    spiFlashWriteCmdAndAddrSpi1(READ_ID_CMD, 0);

    /* Read a byte from the FLASH */
    u16Temp |= exflashWrByteSpi1(Dummy_Byte1) << 8;
    u16Temp |= exflashWrByteSpi1(Dummy_Byte1);
    /* Disable chip select */
    exFlashSetSpiCsHighSpi1();

    osMutexRelease(gFlashexMutex);
    return u16Temp;
}

uint8_t exFlashReadSpi1(uint8_t *pu8Buffer, uint32_t u32ReadAddr, uint16_t u16NumByteToRead)
{
    osMutexAcquire(gFlashexMutex, osWaitForever);

    exFlashSetSpiCsLowSpi1();/* Enable chip select */
    spiFlashWriteCmdAndAddrSpi1(READ_CMD, u32ReadAddr);
    uint32_t index  = 0;
    uint32_t size   = 8191;
    uint32_t count  = u16NumByteToRead / size;
    uint32_t length = u16NumByteToRead % size;

    CLOCK_clockEnable(CLK_FLASH);
    for (index=0; index<count; index++)
    {
        spiMasterDrv1->Transfer(pu8Buffer, &pu8Buffer[index * size], size);
        if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashReadSpi1, P_ERROR,"Read timeout 1.\r\n");
        }
    }
    if (length > 0)
    {
        spiMasterDrv1->Transfer(pu8Buffer, &pu8Buffer[index * size], length);
        if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashReadSpi1_01, P_ERROR,"Read timeout 2.\r\n");
        }
    }
    CLOCK_clockDisable(CLK_FLASH);

    exFlashSetSpiCsHighSpi1();/* Disable chip select */
    osMutexRelease(gFlashexMutex);

    return EXFLASH_OK;
}

uint8_t exFlashWriteSpi1(uint8_t* pData, uint32_t WriteAddr, uint32_t size)
{
    osMutexAcquire(gFlashexWriteMutex, osWaitForever);

    uint32_t end_addr, current_size, current_addr;

    // Calculation of the size between the write address and the end of the page
    //write non-aligned bytes first
    current_size = PAGE_SIZE - (WriteAddr%PAGE_SIZE);

    // Check if the size of the data is less than the remaining place in the page
    if (current_size > size)
    {
        current_size = size;
    }

    // Initialize the adress variables
    current_addr = WriteAddr;
    end_addr = WriteAddr + size;

    // Perform the write page by page
    do
    {
        exFlashPageProSpi1(current_addr,pData,current_size);

        // Update the address and size variables for next page programming
        current_addr += current_size;
        pData += current_size;
        current_size = ((current_addr + PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : PAGE_SIZE;

    } while (current_addr < end_addr);

    osMutexRelease(gFlashexWriteMutex);
    return EXFLASH_OK;
}


uint8_t exSpi1FlashIoIint(uint32_t clk)
{
    gFlashexMutex = osMutexNew(NULL);
    if(gFlashexMutex == NULL)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exSpi1FlashIoIint, P_ERROR,"Failed to create mutex for gFlashexMutex\r\n");
        return EXFLASH_ERROR;
    }
    gFlashexWriteMutex = osMutexNew(NULL);
    if(gFlashexWriteMutex == NULL)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exSpi1FlashIoIint_01, P_ERROR,"Failed to create mutex for gFlashexWriteMutex\r\n");
        return EXFLASH_ERROR;
    }

    spiMasterDrv1->SetMode(DMA_MODE);
    // Initialize master spi
    if (gSpiFlags == NULL)
    {
        gSpiFlags = osEventFlagsNew(NULL);
    }
    spiMasterDrv1->Initialize(SPI_Callback);

    spiMasterDrv1->PowerControl(ARM_POWER_FULL);

    // Configure master spi bus
    spiMasterDrv1->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_DATA_BITS(TRANSFER_DATA_WIDTH) |
                          ARM_SPI_MSB_LSB     | ARM_SPI_SS_MASTER_SW, clk);
    return EXFLASH_OK;
}

uint8_t exFlashReadUniqueId(uint8_t spi_num, uint8_t *uid, uint8_t *uid_len)
{
    osMutexAcquire(gFlashexMutex, osWaitForever);
    uint8_t u8Out[EXT_FLASH_UNIQUEID];

    if (uid == NULL || uid_len == NULL)
    {
        osMutexRelease(gFlashexMutex);
        return EXFLASH_ERROR;
    }

    *uid_len = EXT_FLASH_UNIQUEID;
    CLOCK_clockEnable(CLK_FLASH);
    if (spi_num)
    {
        exFlashSetSpiCsLowSpi1();
        spiFlashWriteCmdAndAddrSpi1(0x4b, 0);
        exflashWrByteSpi1(0x00);
        spiMasterDrv1->Transfer(u8Out, uid, EXT_FLASH_UNIQUEID);
    }
    else
    {
        exFlashSetSpiCsLow();
        spiFlashWriteCmdAndAddr(0x4b, 0);
        exflashWrByte(0x00);
        spiMasterDrv->Transfer(u8Out, uid, EXT_FLASH_UNIQUEID);
    }

    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashReadUniqueId, P_INFO,"ReadUniqueId timeout.");
    }

    if (spi_num)
        exFlashSetSpiCsHighSpi1();
    else
        exFlashSetSpiCsHigh();
    CLOCK_clockDisable(CLK_FLASH);

    osMutexRelease(gFlashexMutex);
    return EXFLASH_OK;
}

static uint8_t exFlashEraseSecurityRegister(uint8_t spi_num, uint8_t num)
{
    /*
     * Erase Security Registers (ERSCUR 44H)
     * Address                   A23-16    A15-12   A11-10   A9-0
     * Security Register #1      00H       0001     00       Don't care
     */
    uint32_t security_addr = 0;

    security_addr = (security_addr & ~(0xF << 12)) | ((uint32_t)(num & 0xF) << 12);

    CLOCK_clockEnable(CLK_FLASH);
    if (spi_num)
    {
        exFlashWrEnSpi1();
        exFlashPollingBusyFlagSpi1();
        exFlashSetSpiCsLowSpi1();
        spiFlashWriteCmdAndAddrSpi1(0x44, security_addr);
    }
    else
    {
        exFlashWrEn();
        exFlashPollingBusyFlag();
        exFlashSetSpiCsLow();
        spiFlashWriteCmdAndAddr(0x44, security_addr);
    }

    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashEraseSecurityRegister, P_INFO,"WriteProgramSecurity timeout.");
    }

    if (spi_num)
    {
        exFlashSetSpiCsHighSpi1();
        exFlashPollingBusyFlagSpi1();
        exFlashWrDisenSpi1();
    }
    else
    {
        exFlashSetSpiCsHigh();
        exFlashPollingBusyFlag();
        exFlashWrDisen();
    }
    CLOCK_clockDisable(CLK_FLASH);

    return EXFLASH_OK;
}

static uint8_t exFlashReadSR(uint8_t spi_num, uint8_t sr_num, void *data)
{
    /*
     * sr_num:1~2
     * Read Status Register(S0~S15) (RDSR1 05H/RDSR2 35H)
     *  05H: S0~S7
     *  35H: S8~S15
     */

    uint8_t sr_command = (sr_num == 1) ? 0x05 : 0x35;
    CLOCK_clockEnable(CLK_FLASH);
    if (spi_num)
    {
        exFlashSetSpiCsLowSpi1();
        exflashWrByteSpi1(sr_command);
        *(uint8_t *)data = exflashWrByteSpi1(0x00);
        exFlashSetSpiCsHighSpi1();
    }
    else
    {
        exFlashSetSpiCsLow();
        exflashWrByte(sr_command);
        *(uint8_t *)data = exflashWrByte(0x00);
        exFlashSetSpiCsHigh();
    }

    CLOCK_clockDisable(CLK_FLASH);

    return EXFLASH_OK;
}

static uint8_t exFlashWriteSR(uint8_t spi_num, uint8_t sr_num, uint8_t data)
{
    /*
     * sr_num:1~2
     * Write Status Register(S0~S15) (RDSR1 01H/RDSR2 31H)
     *  01H: S0~S7
     *  31H: S8~S15
     */
    uint8_t sr_command = (sr_num == 1) ? 0x01 : 0x31;
    CLOCK_clockEnable(CLK_FLASH);
    if (spi_num)
    {
        exFlashWrEnSpi1();
        exFlashPollingBusyFlagSpi1();
        exFlashSetSpiCsLowSpi1();
        exflashWrByteSpi1(sr_command);
        exflashWrByteSpi1(data);
        exFlashSetSpiCsHighSpi1();
        exFlashPollingBusyFlagSpi1();
        exFlashWrDisenSpi1();
    }
    else
    {
        exFlashWrEn();
        exFlashPollingBusyFlag();
        exFlashSetSpiCsLow();
        exflashWrByte(sr_command);
        exflashWrByte(data);
        exFlashSetSpiCsHigh();
        exFlashPollingBusyFlag();
        exFlashWrDisen();
    }
    CLOCK_clockDisable(CLK_FLASH);

    return EXFLASH_OK;
}

static uint8_t exFlashLockSecurityRegister(uint8_t spi_num, uint8_t num)
{
    /*
     * NUM 1~3: LB1 ~ LB3(Status Register S11~S13)
     * sr2_status: S3~S5
     */

    uint8_t sr2_status = 0;
    exFlashReadSR(spi_num, 2, &sr2_status);
    ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashLockSecurityRegister, P_INFO,"sr2_status:0x%x.", sr2_status);

    sr2_status |= (1 << (num + 2));
    exFlashWriteSR(spi_num, 2, sr2_status);

    return EXFLASH_OK;
}

static bool exFlashIsSecurityRegisterLocked(uint8_t spi_num, uint8_t num)
{
    /*
     * NUM 1~3: LB1 ~ LB3(Status Register S11~S13)
     * sr2_status: S3~S5
     */

    uint8_t sr2_status = 0;
    exFlashReadSR(spi_num, 2, &sr2_status);

    if (((sr2_status >> (num + 2)) & 0x01) == 1)
        return true;
    else
        return false;
}

uint8_t exFlashProgramSecurityRegister(uint8_t spi_num, uint32_t otp_addr, const void *data, uint32_t size)
{
    osMutexAcquire(gFlashexMutex, osWaitForever);

    /*
     * Program Security Registers (PRSCUR 42H)
     * Address                   A23-16    A15-12   A11-10   A9-0
     * Security Register #1      00H       0001     00       Byte Address
     */
    uint8_t num = otp_addr/EXT_FLASH_OTP_SIZE + 1;
    uint16_t address = otp_addr%EXT_FLASH_OTP_SIZE;

    uint32_t security_addr = 0;
    uint8_t security_read_buff[size];

    if (size == 0 || data == NULL || num > EXT_FLASH_OTP_NUM || size > EXT_FLASH_OTP_SIZE - address)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashProgramSecurityRegister_0, P_INFO,"Fibocom ProgramSecurity param invaild.");
        osMutexRelease(gFlashexMutex);
        return EXFLASH_ERROR;
    }

    if (exFlashIsSecurityRegisterLocked(spi_num, num))
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashProgramSecurityRegister_1, P_INFO,"Fibocom num:%d is locked.", num);
        osMutexRelease(gFlashexMutex);
        return EXFLASH_ERROR;
    }

    if (exFlashEraseSecurityRegister(spi_num, num))
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashProgramSecurityRegister_2, P_INFO,"Fibocom num:%d Erase failed.", num);
        osMutexRelease(gFlashexMutex);
        return EXFLASH_ERROR;
    }

    security_addr = ((uint32_t)num << 12) | ((uint32_t)address & 0xFFF);

    CLOCK_clockEnable(CLK_FLASH);
    if (spi_num)
    {
        exFlashWrEnSpi1();
        exFlashPollingBusyFlagSpi1();
        exFlashSetSpiCsLowSpi1();
        spiFlashWriteCmdAndAddrSpi1(0x42, security_addr);
        spiMasterDrv1->Transfer(data, security_read_buff, size);
    }
    else
    {
        exFlashWrEn();
        exFlashPollingBusyFlag();
        exFlashSetSpiCsLow();
        spiFlashWriteCmdAndAddr(0x42, security_addr);
        spiMasterDrv->Transfer(data, security_read_buff, size);
    }

    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashProgramSecurityRegister_3, P_INFO,"WriteProgramSecurity timeout.");
    }

    if (spi_num)
    {
        exFlashSetSpiCsHighSpi1();
        exFlashPollingBusyFlagSpi1();
        exFlashWrDisenSpi1();
    }
    else
    {
        exFlashSetSpiCsHigh();
        exFlashPollingBusyFlag();
        exFlashWrDisen();
    }
    CLOCK_clockDisable(CLK_FLASH);

    if (exFlashLockSecurityRegister(spi_num, num))
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashProgramSecurityRegister_4, P_INFO,"Fibocom num:%d Lock failed.", num);
        osMutexRelease(gFlashexMutex);
        return EXFLASH_ERROR;
    }

    osMutexRelease(gFlashexMutex);
    return EXFLASH_OK;
}

uint8_t exFlashReadSecurityRegister(uint8_t spi_num, uint32_t otp_addr , void *data, uint32_t size)
{
    osMutexAcquire(gFlashexMutex, osWaitForever);

    /*
     *  Read Security Registers (RDSCUR 48H)
     * Address                   A23-16    A15-12   A11-10   A9-0
     * Security Register #1      00H       0001     00       Byte Address
     */
    uint8_t num = otp_addr/EXT_FLASH_OTP_SIZE + 1;
    uint16_t address = otp_addr%EXT_FLASH_OTP_SIZE;
    uint32_t security_addr = 0;

    security_addr = ((uint32_t)num << 12) | ((uint32_t)address & 0xFFF);

    if (size == 0 || data == NULL || num > EXT_FLASH_OTP_NUM || size > EXT_FLASH_OTP_SIZE - address)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashReadSecurityRegister_0, P_INFO,"Fibocom ReadSecurity param invaild.");
        osMutexRelease(gFlashexMutex);
        return EXFLASH_ERROR;
    }

    CLOCK_clockEnable(CLK_FLASH);
    if (spi_num)
    {
        exFlashSetSpiCsLowSpi1();
        spiFlashWriteCmdAndAddrSpi1(0x48, security_addr);
        exflashWrByteSpi1(0x00);
        spiMasterDrv1->Transfer(data, data, size);
    }
    else
    {
        exFlashSetSpiCsLow();
        spiFlashWriteCmdAndAddr(0x48, security_addr);
        exflashWrByte(0x00);
        spiMasterDrv->Transfer(data, data, size);
    }

    if (osEventFlagsWait(gSpiFlags, 0x01, osFlagsWaitAll, 100) == osErrorTimeout)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exFlashReadSecurityRegister_1, P_INFO,"WriteProgramSecurity timeout.");
    }

    if (spi_num)
        exFlashSetSpiCsHighSpi1();
    else
        exFlashSetSpiCsHigh();

    CLOCK_clockDisable(CLK_FLASH);

    osMutexRelease(gFlashexMutex);
    return EXFLASH_OK;
}
#endif
uint8_t exSpi0FlashIoIint(uint32_t clk)
{
    gFlashexMutex = osMutexNew(NULL);
    if(gFlashexMutex == NULL)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exSpi0FlashIoIint, P_ERROR,"Failed to create mutex for gFlashexMutex\r\n");
        return EXFLASH_ERROR;
    }
    gFlashexWriteMutex = osMutexNew(NULL);
    if(gFlashexWriteMutex == NULL)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, exSpi0FlashIoIint_01, P_ERROR,"Failed to create mutex for gFlashexWriteMutex\r\n");
        return EXFLASH_ERROR;
    }

    spiMasterDrv->SetMode(DMA_MODE);
    // Initialize master spi
    if (gSpiFlags == NULL)
    {
        gSpiFlags = osEventFlagsNew(NULL);
    }
    spiMasterDrv->Initialize(SPI_Callback);

    // Power on
    spiMasterDrv->PowerControl(ARM_POWER_FULL);

    // Configure master spi bus
    spiMasterDrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_DATA_BITS(TRANSFER_DATA_WIDTH) |
                          ARM_SPI_MSB_LSB     | ARM_SPI_SS_MASTER_SW, clk);
    return EXFLASH_OK;
}
#endif


