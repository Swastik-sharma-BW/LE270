#include "app_image.h"
#include "fibo_opencpu.h"
#include <time.h>
#include "oc_at.h"

extern void *appimg_enter(void *param);


static AtValueAttr attrFIBOTESTDEMO[] = {AT_PARAM_ATTR_DEF(AT_DEC_VAL, AT_OPT_VAL),
                                         AT_PARAM_ATTR_DEF(AT_STR_VAL, AT_OPT_VAL) };

CmsRetId demoTESTDEMO(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId ret = CMS_FAIL;
    AtParaRet   atRet =  0;
    UINT32  reqHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_DEFAULT_SUB_AT_ID, pAtCmdReq->chanId);
    UINT32  operaType = (UINT32)pAtCmdReq->operaType;
    CHAR    rspBuf[ATEC_IND_RESP_256_STR_LEN] = {0};

    UINT16  paraLen = 0;
    INT32   param0 = 0;
    UINT8    param1[ATEC_IND_RESP_64_STR_LEN] = {0};
    AtParamValueCP  pParamList = pAtCmdReq->pParamList;

    switch (operaType)
    {
        case AT_TEST_REQ:         /* AT+TESTDEMO=? */
        {
            sprintf(rspBuf, "+TESTDEMO: <param0>,<param1>");
            ret = atcReply(reqHandle, AT_RC_OK, ATC_SUCC_CODE, rspBuf);
            break;
        }
        case AT_EXEC_REQ:         /* AT+TESTDEMO */
        {
            ret = atcReply(reqHandle, AT_RC_OK, ATC_SUCC_CODE, NULL);
            break;
        }
        case AT_READ_REQ:         /* AT+TESTDEMO? */
        {
            sprintf(rspBuf, "+TESTDEMO: %d,%s",1,"demo");
            ret = atcReply(reqHandle, AT_RC_OK, ATC_SUCC_CODE, rspBuf);
            break;
        }

        case AT_SET_REQ:         /* AT+TESTDEMO=<param1>,<param2> */
        {
            atRet = atGetNumValue(pParamList, 0, &param0, 0, 2048, 0);
            if (AT_PARA_OK == atRet)
            {
                atRet = atGetStrValue(pAtCmdReq->pParamList, 1, param1, ATEC_IND_RESP_64_STR_LEN, &paraLen, (CHAR *)NULL);
            }

            if(AT_PARA_OK == atRet)
            {
                sprintf(rspBuf, "+TESTDEMO: %d, %s",param0, param1);
                ret = atcReply(reqHandle, AT_RC_OK, ATC_SUCC_CODE, rspBuf);
                break;
            }
        }

        default:
        {
            ret = atcReply(reqHandle, AT_RC_CME_ERROR, (AT_RC_CUSTOM_ERROR), NULL);
            break;
        }
    }

    return ret;
}

CmsRetId demoGTFSINFO(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId ret = CMS_FAIL;
    UINT32  reqHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_DEFAULT_SUB_AT_ID, pAtCmdReq->chanId);
    CHAR    rspBuf[ATEC_IND_RESP_256_STR_LEN] = {0};

    sprintf(rspBuf, "+GTFSINFO: total %d, free %d", fibo_file_get_total_size(), fibo_file_getFreeSize());
    ret = atcReply(reqHandle, AT_RC_OK, ATC_SUCC_CODE, rspBuf);

    return ret;
}


const AtCmdPreDefInfo  CustAtTable[] =
{
    AT_CMD_PRE_DEFINE("+TESTDEMO",          demoTESTDEMO,         attrFIBOTESTDEMO,       AT_EXT_PARAM_CMD,   AT_DEFAULT_TIMEOUT_SEC),
    AT_CMD_PRE_DEFINE("+GTFSINFO",          demoGTFSINFO,                     NULL,       AT_EXT_PARAM_CMD,   AT_DEFAULT_TIMEOUT_SEC),
};

static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    int mode = va_arg(arg, int); //
    int num  = va_arg(arg, int);
    va_end(arg); 
    
    fibo_textTrace("sig_res_callback  sig = %d (200 is opencpu test.) mode =%d num =%d", sig, mode, num);
  

    return;
}


static void at_res_callback(UINT8 *buf, UINT16 len)
{
    fibo_textTrace("FIBO <--%s", buf);
}

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};





void *appimg_enter(void *param)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_enter_1, P_INFO, "application appimg_enter");
    fibo_textTrace("app image enter");
    fibo_atCmd_Register(CustAtTable,sizeof(CustAtTable)/sizeof(AtCmdPreDefInfo));
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, appimg_exit_1, P_INFO, "application image exit");
    return;
}
