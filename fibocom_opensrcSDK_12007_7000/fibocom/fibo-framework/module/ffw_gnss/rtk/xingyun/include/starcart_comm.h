/**
*@added by fibocom.inc. handle sanity check
*@service of RTK process.
*/
#ifndef _STARCART_COMM_H_
#define _STARCART_COMM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "osi_api.h"
#include "osi_log.h"
#include "at_response.h"
#include "at_command.h"
#include "at_engine.h"

#include "starcart_rtk.h"
#include "starcart_export_err.h"

typedef struct
{
	char uartdata[4096]; //uart data
	int len; //relen
} fdx_data;


#define DEVICEID_MAX_LEN 64
#define APPKEY_MAX_LEN 64

#define STARCART_DATA "+GTGPSXYNEMADATA: %s\n%s"

static atCmdEngine_t *ght_starcart_cmdEngine = NULL;

#define ght_starcart_cmd_come(params)                                \
    do                                                               \
    {                                                                \
        ght_starcart_cmdEngine = params->engine;                     \
        starcart_logPrint("cmdEngine=%p", ght_starcart_cmdEngine);   \
    } while (0)

#define ght_starcart_get_str(cmd, index, len)                 	   \
    ({                                                             \
        char *ret;                                                 \
        atCmdParam_t *s = cmd->params[index++];                    \
        bool paramRet = true;                                      \
        ret = (char *)atParamStr(s, &paramRet);                    \
        if (!paramRet || strlen(ret) > (len))            		   \
        {                                                          \
            starcart_logPrint("get str params fail:%s", s->value); \
			RETURN_CME_ERR(cmd->engine, ERR_AT_CME_PARAM_INVALID); \
            return;                                                \
        }                                                          \
        ret;                                                       \
    })

#define ght_starcart_get_int(cmd, index, min, max)                     \
	({																   \
		int ret;													   \
		atCmdParam_t *s = cmd->params[index++]; 					   \
		bool paramRet = true;										   \
		ret = atParamUintInRange(s, (min), (max), &paramRet);		   \
		if (!paramRet)												   \
		{															   \
			starcart_logPrint("get rang int params fail:%s", s->value);\
			RETURN_CME_ERR(cmd->engine, ERR_AT_CME_PARAM_INVALID);	   \
			return; 												   \
		}															   \
		ret;														   \
	})

static inline int starcart_cmd_return_ok()
{
	int ret = STARCART_RET_SUCCESS;
	atCmdRespOK(ght_starcart_cmdEngine);
	return ret;
}

static inline int starcart_cmd_return_err(int code)
{
	int ret = STARCART_RET_SUCCESS;
	atCmdRespCmeError(ght_starcart_cmdEngine, code);
	return ret;
}
static inline int starcart_cmd_report(const char *text)
{
    int ret = STARCART_RET_SUCCESS;
    atCmdRespInfoText(ght_starcart_cmdEngine, text);
    return ret;
}



#ifdef __cplusplus
}
#endif
#endif
