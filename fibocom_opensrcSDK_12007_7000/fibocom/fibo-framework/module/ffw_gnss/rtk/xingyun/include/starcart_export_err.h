/**
*@ added by fibocom. handle service starcart rtk
*@ at command and rtk process errr cb
*
*/
#ifndef _STARCART_EXPORT_ERROR_H_
#define _STARCART_EXPORT_ERROR_H_
#ifdef __cplusplus
extern "C"
{
#endif

typedef enum{

	STARCART_RET_SUCCESS = 0,
	STARCART_RET_FAILURE = -1,

	STARCART_SDK_GET_INTERFACE_ERR = -101,
	STARCART_SDK_INIT_ERROR = -102,
	STARCART_SDK_MUTEX_ERROR = -103,
	
} Starcart_Return_ErrorCode;

#ifdef __cplusplus
}
#endif
#endif
