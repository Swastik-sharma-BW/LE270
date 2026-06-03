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
#ifndef __CECS_H__
#define __CECS_H__


/*
  CECS connection:

  +----------------+                              +----------------+
  |                |                              |                |
  |    +-----------|                              |-----------+    |
  |    |SPI master |                              |  SPI slave|    |
  |    |           |                              |           |    |
  |    |       	RST|----------------------------->|RST        |    |
  |    |           |                              |           |    |
  |    |       	INT|<-----------------------------|INT        |    |
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

//master part
#define CECS_MASTER_INSTANCE             0

#define CECS_MASTER_INT_PAD             WAKEUP_PAD_4
#define CECS_MASTER_INT_IRQ             PadWakeup4_IRQn

#define CECS_MASTER_INT_IRQ_ISR         void Pad4_WakeupIntHook(void) // override the weak function in bsp_custom.c

#if defined CHIP_EC718
#ifdef FIBOCOM_BASE
#define CECS_MASTER_RST_PAD_INDEX       18      // PIN54 (GPIO3)
#define CECS_MASTER_RST_PAD_ALT_FUNC    PAD_MUX_ALT0
#define CECS_MASTER_RST_GPIO_INSTANCE   0       // PIN54 (GPIO3)
#define CECS_MASTER_RST_GPIO_INDEX      3       // PIN54 (GPIO3)
#else
#define CECS_MASTER_RST_PAD_INDEX       48      // AGPIO3 (GPIO23)
#define CECS_MASTER_RST_PAD_ALT_FUNC    PAD_MUX_ALT0
#define CECS_MASTER_RST_GPIO_INSTANCE   1       // AGPIO3 (GPIO23)
#define CECS_MASTER_RST_GPIO_INDEX      7       // AGPIO3 (GPIO23)
#endif
#elif defined CHIP_EC716
#define CECS_MASTER_RST_PAD_INDEX       25      // AGPIO3 (GPIO13)
#define CECS_MASTER_RST_PAD_ALT_FUNC    PAD_MUX_ALT0
#define CECS_MASTER_RST_GPIO_INSTANCE   0       // AGPIO3 (GPIO13)
#define CECS_MASTER_RST_GPIO_INDEX      13      // AGPIO3 (GPIO13)
#endif

#define CECS_SPI_FRAME_FORMAT            ARM_SPI_CPOL1_CPHA1
#define CECS_BUS_SPEED                   40000000 //25600000 //1000000

#define CECS_FRAME_MTU                   1600
#define CECS_MIN_TRANSFER_DUMMY_BYTES    256

#define CECSTIMEOUT_AFTER_LINK_CHG		 (3000) //3000MS
/*----------------------------------------------------------------------------*
 *                   DATA TYPE DEFINITION                                     *
 *----------------------------------------------------------------------------*/

/** \brief List of CECS flow control options */
typedef enum
{
    CECS_FLOW_CONTROL_XON,      /**< permit to send */
    CECS_FLOW_CONTROL_XOFF      /**< shall not send */
} CECSFlowControl_e;



/*----------------------------------------------------------------------------*
 *                    GLOBAL FUNCTIONS DECLEARATION                           *
 *----------------------------------------------------------------------------*/
int32_t CECS_MasterInit(void);
void CECS_MasterGpioInit(void);


#ifdef __cplusplus
}
#endif
#endif/*__CECS_H__*/


