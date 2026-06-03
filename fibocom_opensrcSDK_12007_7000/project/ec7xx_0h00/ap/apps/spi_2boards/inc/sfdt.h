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
#ifndef __SFDT_H__
#define __SFDT_H__


/*
  SFDT connection:

  +----------------+                              +----------------+
  |                |                              |                |
  |    +-----------|                              |-----------+    |
  |    |SPI master |                              |  SPI slave|    |
  |    |           |                              |           |    |
  |    |       MRDY|----------------------------->|MRDY       |    |
  |    |           |                              |           |    |
  |    |       SRDY|<-----------------------------|SRDY       |    |
  |    |           |                              |           |    |
  |    |         CS|----------------------------->|CS         |    |
  |    |           |                              |           |    |
  |    |        CLK|----------------------------->|CLK        |    |
  |    |           |                              |           |    |
  |    |       MOSI|----------------------------->|MOSI       |    |
  |    |           |                              |           |    |
  |    |       MISO|<-----------------------------|MISO       |    |
  |    |           |                              |           |    |
  |    +-----------|                              |-----------+    |
  |                |                              |                |
  |             GND|<---------------------------->|GND             |
  |                |                              |                |
  +----------------+                              +----------------+
*/


/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#include "RTE_Device.h"

/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/

//undef to select slave board
//#define SFDT_MASTER_BOARD

//master part
#define SFDT_MASTER_INSTANCE             0

#define SFDT_MASTER_SRDY_PAD             WAKEUP_PAD_3
#define SFDT_MASTER_SRDY_IRQ             PadWakeup3_IRQn
#ifdef SFDT_MASTER_BOARD
#define SFDT_MASTER_SRDY_IRQ_ISR         void Pad3_WakeupIntHook(void) // override the weak function in bsp_custom.c
#else
#define SFDT_MASTER_SRDY_IRQ_ISR         void SFDT_masterSrdyIsr(void)
#endif

#if defined CHIP_EC718
#define SFDT_MASTER_MRDY_PAD_INDEX       48      // AGPIO3 (GPIO23)
#define SFDT_MASTER_MRDY_PAD_ALT_FUNC    PAD_MUX_ALT0
#define SFDT_MASTER_MRDY_GPIO_INSTANCE   1       // AGPIO3 (GPIO23)
#define SFDT_MASTER_MRDY_GPIO_INDEX      7       // AGPIO3 (GPIO23)
#elif defined CHIP_EC716
#define SFDT_MASTER_MRDY_PAD_INDEX       25      // AGPIO3 (GPIO13)
#define SFDT_MASTER_MRDY_PAD_ALT_FUNC    PAD_MUX_ALT0
#define SFDT_MASTER_MRDY_GPIO_INSTANCE   0       // AGPIO3 (GPIO13)
#define SFDT_MASTER_MRDY_GPIO_INDEX      13      // AGPIO3 (GPIO13)

#endif


//slave part
#define SFDT_SLAVE_INSTANCE              0

#define SFDT_SLAVE_MRDY_PAD              WAKEUP_PAD_4
#define SFDT_SLAVE_MRDY_IRQ              PadWakeup4_IRQn
#ifndef SFDT_MASTER_BOARD
#define SFDT_SLAVE_MRDY_IRQ_ISR          void Pad4_WakeupIntHook(void) // override the weak function in bsp_custom.c
#else
#define SFDT_SLAVE_MRDY_IRQ_ISR          void SFDT_slaveMrdyIsr(void)
#endif

#if defined CHIP_EC718

#define SFDT_SLAVE_SRDY_PAD_INDEX        48      // AGPIO3 (GPIO23)
#define SFDT_SLAVE_SRDY_PAD_ALT_FUNC     PAD_MUX_ALT0
#define SFDT_SLAVE_SRDY_GPIO_INSTANCE    1       // AGPIO3 (GPIO23)
#define SFDT_SLAVE_SRDY_GPIO_INDEX       7       // AGPIO3 (GPIO23)
#elif defined CHIP_EC716

#define SFDT_SLAVE_SRDY_PAD_INDEX        25      // AGPIO3(GPIO13)
#define SFDT_SLAVE_SRDY_PAD_ALT_FUNC     PAD_MUX_ALT0
#define SFDT_SLAVE_SRDY_GPIO_INSTANCE    0       // AGPIO3(GPIO13)
#define SFDT_SLAVE_SRDY_GPIO_INDEX       13      // AGPIO3(GPIO13)
#endif

//common part
#define SFDT_SPI_FRAME_FORMAT            ARM_SPI_CPOL0_CPHA1 // or ARM_SPI_CPOL1_CPHA1 to get rid of CS pulse between each data word transfer
#define SFDT_BUS_SPEED                   20000000 //25600000 //1000000

#define SFDT_FRAME_HEADER_SIZE           sizeof(SFDTFrameHeader_t)
#define SFDT_FRAME_MTU                   1600    // includes 4 bytes SFDT_FRAME_HEADER_SIZE
#define SFDT_MIN_TRANSFER_DUMMY_BYTES    256

/*----------------------------------------------------------------------------*
 *                   DATA TYPE DEFINITION                                     *
 *----------------------------------------------------------------------------*/

/** \brief List of SFDT flow control options */
typedef enum
{
    SFDT_FLOW_CONTROL_XON,      /**< permit to send */
    SFDT_FLOW_CONTROL_XOFF      /**< shall not send */
} SFDTFlowControl_e;

typedef union
{
    uint8_t bytes[4];
    struct
    {
        uint32_t payLoadLen:       12;
        uint32_t seqNo:             4; // for debug purpose
        uint32_t flowControl:       1; // flow control
        uint32_t bContinue:         1; // is this frame the last one?
        uint32_t resv:              6;
        uint32_t checkSum:          8; // XOR of Frame head byte0-2
    } field;
} SFDTFrameHeader_t;


/*----------------------------------------------------------------------------*
 *                    GLOBAL FUNCTIONS DECLEARATION                           *
 *----------------------------------------------------------------------------*/
__FORCEINLINE uint8_t SFDT_csum(SFDTFrameHeader_t* header)
{
    uint32_t csum = header->bytes[0] + header->bytes[1] + header->bytes[2];
    return (uint8_t)((csum >> 8) ^ csum ^ 0x3);
}

int32_t SFDT_masterInit(void);
void *SFDT_mallocTxFrameBuffer(uint32_t size);
void SFDT_freeRxFrameBuffer(uint8_t *payLoadPtr);

void SFDT_masterSend(uint8_t* payLoadPtr, uint32_t payLoadLen);
void SFDT_masterReceive(uint8_t** reportData, uint32_t* reportLen, uint32_t* flag);
void SFDT_masterFlowControl(SFDTFlowControl_e setting);

int32_t SFDT_slaveInit(void);
void SFDT_slaveSendTrigger(void);
void SFDT_slaveFlowControl(uint32_t control);

void SFDT_masterMrdyInit(void);
void SFDT_slaveSrdyInit(void);


#ifdef __cplusplus
}
#endif
#endif

