/******************************************************************************
Copyright:      - 2017, All rights reserved by Eigencomm Ltd.
File name:      - ecuiccapi.c
Description:    - UICC open API.
Function List:  -
History:        - 09/20/2022, Originated by xlhu
******************************************************************************/


/******************************************************************************
 * Include Files
*******************************************************************************/
#include "cmsis_os2.h"
#if (defined CHIP_EC718) || (defined CHIP_EC716)
#include "ec7xx.h"
#elif defined CHIP_EC618
#include "ec618.h"
#endif


#include "os_common.h"
#include DEBUG_LOG_HEADER_FILE
#include "debug_trace.h"
#include "ecuiccapi.h"

/*********************************************************************************
 * Macros
*********************************************************************************/

/******************************************************************************
 * Extern global variables
*******************************************************************************/

/******************************************************************************
 * Extern functions
*******************************************************************************/
extern void UsimInitReadAddiFiles(UINT16 fileId);
extern void UiccCtrlTryGsmApp(void);
extern void ReadGsmAppFiles(UINT16 fileId);
/******************************************************************************
 * Global variables
*******************************************************************************/
/*
 * bSoftSIMTaskCreate
 * Whether the SoftSIM task created.
*/
AP_PLAT_COMMON_BSS BOOL    bSoftSIMTaskCreate = FALSE;
/******************************************************************************
 * Types
*******************************************************************************/

/******************************************************************************
 * Local variables
*******************************************************************************/


/******************************************************************************
 * Function Prototypes
*******************************************************************************/

/******************************************************************************
 * Function definition
*******************************************************************************/
#define __DEFINE_SOFTSIM_ADAPT_FUNCTION__ //just for easy to find this position in SS
/******************************************************************************
 * SoftSIM interface adapted note
 * 1> The SoftSIM feature is disable as default, and enabled by AT+ECSIMCFG="softsim",1
 * 2> SoftSIM task shall be created in SoftSimInit func or appinit depended by softsim vender.
 * 3> SoftSIM vender shall adapt  functions SoftSimReset/SoftSimApduReq.
*******************************************************************************/

/******************************************************************************
 * SoftSimReset
 * Description: This API called by modem/uiccdrv task to reset softsim and get ATR parameter from softsim,
 *         as same as code/warm reset with physical SIM card.
 * param[in]   null
 * param[out]   UINT16 *atrLen, the pointer to the length of ATR, this memory don't need to be free.
 * param[out]   UINT8 *atrData, the pointer to the ATR data,  this memory don't need to be free.
 *               atrData buffer size is 33, fill atrData shall not exceed 33 bytes.
 * Comment: This API will be called only if softsim feature is enabled by AT CMD.
 *        Shall send signal/msg to softsim task and block to wait response .
 *        Softsim internal process running in this func is not allowed.
******************************************************************************/
void SoftSimReset(UINT16 *atrLen, UINT8 *atrData)
{
    osSemaphoreId_t sem = osSemaphoreNew(1U, 0, PNULL);
    osStatus_t osState = osOK;

    /*
    * softsim vender shall implement 3 steps as below
    */

    /*
    * 1> create signal/msg
    */

    /*
    * 2> send signal/msg to softsim task with sem/atrLen/atrData
    */

    /*
    * 3> process signal/msg on softsim task, then retrun paramters and release sem on softsim task
    */

    /*
     * wait for sem 2sec
    */
    if ((osState = osSemaphoreAcquire(sem, 2000)) != osOK)
    {
        OsaDebugBegin(FALSE, osState, 0, 0);
        OsaDebugEnd();
    }

    /*
     * Semaphore delete
    */
    osSemaphoreDelete(sem);

}

/******************************************************************************
 * SoftSimApduReq
 * Description: This API will be called by modem/uiccdrv task to send APDU(TPDU) request and get response from softsim,
 *         support case 1/2/3/4 command/response process.
 * param[in]   UINT16 txDataLen, the length of tx data
 * param[in]   UINT8 *txData, the pointer to the tx data, this memory don't need to be free.
 * param[out]   UINT16 *rxDataLen, the pointer to the length of rx data, this memory don't need to be free.
 * param[out]   UINT8 *rxData, the pointer to the rx data, this memory don't need to be free.
 *               rxData buffer size is 258, fill rxData shall not exceed 258 bytes.
 * Comment: This API will be called only if softsim feature is enabled by AT CMD.
 *        Shall send signal/msg to softsim task and block to wait response.
 *        Softsim internal process running in this func is not allowed.
******************************************************************************/
void SoftSimApduReq(UINT16 txDataLen, UINT8 *txData, UINT16 *rxDataLen, UINT8 *rxData)
{
    osSemaphoreId_t sem = osSemaphoreNew(1U, 0, PNULL);
    osStatus_t osState = osOK;

    /*
    * softsim vender shall implement 3 steps as below
    */

    /*
    * 1> create signal/msg
    */

    /*
    * 2> send signal/msg to softsim task with sem/txDataLen/txData/rxDataLen/rxData
    */

    /*
    * 3> process signal/msg on softsim task, then retrun paramters and release sem on softsim task
    */

    /*
     * wait for sem 2sec
    */
    if ((osState = osSemaphoreAcquire(sem, 2000)) != osOK)
    {
        OsaDebugBegin(FALSE, osState, 0, 0);
        OsaDebugEnd();
    }

    /*
     * Semaphore delete
    */
    osSemaphoreDelete(sem);

}



/******************************************************************************
 * SoftSimInit
 * Description: This api called by modem/uiccdrv task to start softsim task if softsim feature is enabled.
 * input: void
 * output: void
 * Comment:
******************************************************************************/
void SoftSimInit(void)
{
    if (bSoftSIMTaskCreate == TRUE)
    {
        ECPLAT_PRINTF(UNILOG_PLA_DRIVER, SoftSimInit_0, P_INFO, "Softsim task has already been created");
        return;
    }
    /*
    * start softsim task
    */
    ECPLAT_PRINTF(UNILOG_PLA_DRIVER, SoftSimInit_1, P_INFO, "Start fibo softsim task");
    /*
    * softsim vender implement softsim task created
    */



    //delay 100ms for softsim task init
    osDelay(100);

    //set flag after task created
    bSoftSIMTaskCreate = TRUE;

}

/******************************************************************************
 * SoftSimIsSimplyApduTrans
 * Description: return value indicated whether SoftSIM only support simply APDU transmission without TPDU.
 *        e.g. terminal -> SoftSIM: 00 A4 08 04 02 2F E2
 *           SoftSIM -> teriminal: 62 17 82 02 41 21 83 02 2F E2 8A 01 05 8B 03 2F 06 03 80 02 00 0A 88 01 10 90 00
 * input: void
 * output: BOOL
 * Comment: This api called by modem/uiccdrv task.
******************************************************************************/
BOOL SoftSimIsSimplyApduTrans(void)
{
    return FALSE;//softsim vender changed it here if required
}

#define __DEFINE_OPER_REQ_READ_ADDI_FILE_FUNCTION__ //just for easy to find this position in SS
/******************************************************************************
 * EcUiccReadAddiFiles
 * Description: Read additional file during usim init for an operator's requirement.
 *        e.g.
 * input: UINT16 fileId, filed Id
 * output: void
 * Comment: This api called by modem/uiccctrl task.
******************************************************************************/
void EcUiccReadAddiFiles(UINT16 fileId)
{
#ifdef USIM_INIT_READ_ADDI_FILE
    UsimInitReadAddiFiles(fileId);
#endif
}

/******************************************************************************
 * EcUiccTryGsmApp
 * Description: try GSM application (2G SIM) for an operator's requirement.
 *        e.g.
 * input: void
 * output: void
 * Comment: This api called by modem/uiccctrl task.
******************************************************************************/
BOOL EcUiccTryGsmApp(void)
{
#ifdef USIM_INIT_READ_ADDI_FILE
    UiccCtrlTryGsmApp();
    return TRUE;
#else
    return FALSE;
#endif
}

/******************************************************************************
 * EcGsmAppReadAddiFiles
 * Description: Read additional file during GSM app init for an operator's requirement.
 *        e.g.
 * input: UINT16 fileId, filed Id
 * output: void
 * Comment: This api called by modem/uiccctrl task.
******************************************************************************/
void EcGsmAppReadAddiFiles(void)
{
#ifdef USIM_INIT_READ_ADDI_FILE
    /*
    * below files under DF GSM(7F20)
    */
    // read EFecc
    ReadGsmAppFiles(0x6FB7);
    // read EFlp
    ReadGsmAppFiles(0x6F05);
    // read EFad
    ReadGsmAppFiles(0x6FAD);
    // read EFsst
    ReadGsmAppFiles(0x6F38);
    // read EFimsi
    ReadGsmAppFiles(0x6F07);
    // read EFacc
    ReadGsmAppFiles(0x6F78);
    // read EFhpplmn
    ReadGsmAppFiles(0x6F31);
    // read EFplmn
    ReadGsmAppFiles(0x6F30);
    // read EFhplmnwact
    ReadGsmAppFiles(0x6F62);
    // read EFplmnwact
    ReadGsmAppFiles(0x6F60);
    // read EFoplmnwact
    ReadGsmAppFiles(0x6F61);
    // read EFloci
    ReadGsmAppFiles(0x6F7E);
    // read EFlocigprs
    ReadGsmAppFiles(0x6F53);
    // read EFkc
    ReadGsmAppFiles(0x6F20);
    // read EFkcgprs
    ReadGsmAppFiles(0x6F52);
    // read EFfplmn
    ReadGsmAppFiles(0x6F7B);
    // read EFACMmax
    ReadGsmAppFiles(0x6F37);
    // read EFACM
    ReadGsmAppFiles(0x6F39);
    //read EFGID2
    ReadGsmAppFiles(0x6F3F);
    //read EFCBMI
    ReadGsmAppFiles(0x6F45);

#if 0
    // read EFest
    ReadGsmAppFiles(0x6F56); //usim
    // read EFpsloci
    ReadGsmAppFiles(0x6F73);
    // read EFkeysPS
    ReadGsmAppFiles(0x6F09);
    // read EFkeys
    ReadGsmAppFiles(0x6F08);
    // read EFSTART-HFN
    ReadGsmAppFiles(0x6F5B);
    // read EFTHRESHOLD
    ReadGsmAppFiles(0x6F5C);
    // read EFNETPAR
    ReadGsmAppFiles(0x6FC4);
#endif
    //added files under DF gsm
    //read EFXX(6FXX)
    //ReadGsmAppFiles(0x6FXX);

    /*
    * below files under DF telecom(7F10)
    */
    //select DF telecom
    ReadGsmAppFiles(0x7F10);
    // read EFsms
    ReadGsmAppFiles(0x6F3C);
    // read EFsmsp
    ReadGsmAppFiles(0x6F42);
    // read EFsmss
    ReadGsmAppFiles(0x6F43);
    // read EFsmsr
    ReadGsmAppFiles(0x6F47);
    // read EFmsisdn
    ReadGsmAppFiles(0x6F40);
    //read EFADN
    ReadGsmAppFiles(0x6F3A);
    //read EFLND
    ReadGsmAppFiles(0x6F44);
    //read EFFDN
    ReadGsmAppFiles(0x6F3B);
    //added files under DF telecom
    //read EFXX(6FXX)
    //ReadGsmAppFiles(0x6FXX);

    //added files under other DF
    //select DF first
    //ReadGsmAppFiles(0xXFXX);
    //read EFXX(6FXX)
    //ReadGsmAppFiles(0x6FXX);

    //at last select DFgsm direction
    ReadGsmAppFiles(0x7F20);
#endif
}

