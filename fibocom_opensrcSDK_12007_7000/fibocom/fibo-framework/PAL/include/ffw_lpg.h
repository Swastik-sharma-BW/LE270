#ifndef __FFW_LPG_H__
#define __FFW_LPG_H__

#include <stdint.h>
#include <stdbool.h>


typedef enum {  
    LPG_CLOSE=0,   // Close LPG  
    LPG_OPEN,      //Open LPG    
}LPG_status;


/**
 * @brief LPG initialize
 * 
 * @param NONE
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_lpg_init(void);

#ifndef CONFIG_FFW_CUSTOM_LPG_SUPPORT
/**
 * @brief Set the net LPG status
 * 
 * @param lpg_status: open/close LPG
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_set_lpgstatus(LPG_status lpg_status);

/**
 * @brief Set the system LPG status
 * 
 * @param lpg_status: open/close LPG
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_set_syslpgstatus(LPG_status lpg_status);

/**
 * @brief Set Net mode LPG status
 * 
 * @param lpg_status: open/close LPG
 * @return int 
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_set_netmodelpgstatus(LPG_status lpg_status);
#else

//2024-07-03, wangzhen, MTC0708-990. Modify custom light interface.
#if defined(FFW_FLYMODEM_CONFIG) || defined (FFW_UVR_CONFIG) || defined(FFW_XINYUAN_CONFIG)|| defined(CONFIG_FFW_MIFIDATA)
/**
 * @brief Set Red LPG status
 *
 * @param lpg_status: open/close LPG
 * @return int
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_set_redlpgstatus(LPG_status lpg_status);

/**
 * @brief Set Green LPG status
 *
 * @param lpg_status: open/close LPG
 * @return int
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_set_greenlpgstatus(LPG_status lpg_status);
#endif

#if defined(FFW_FLYMODEM_CONFIG)
/**
 * @brief Set blue LPG status
 *
 * @param lpg_status: open/close LPG
 * @return int
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_set_bluelpgstatus(LPG_status lpg_status);
#endif

#if defined (FFW_UVR_CONFIG)
/**
 * @brief Set yellow LPG status
 *
 * @param lpg_status: open/close LPG
 * @return int
 * @retval FFW_R_SUCCESS
 * @retval FFW_R_FAILED
 */
int ffw_set_yellowlpgstatus(LPG_status lpg_status);
#endif
//2024-07-03, wangzhen, MTC0708-990, End.
#endif

#endif /* __FFW_LPG_H__ */
