#ifndef CE4480C2_210B_474A_9D94_E3ADD62840E6
#define CE4480C2_210B_474A_9D94_E3ADD62840E6

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ffw_types.h"

#define FFW_ATE_RC_OK 0         ///< "OK"
#define FFW_ATE_RC_CONNECT 1    ///< "CONNECT"
#define FFW_ATE_RC_RING 2       ///< "RING/CRING"
#define FFW_ATE_RC_NOCARRIER 3  ///< "NO CARRIER"
#define FFW_ATE_RC_ERROR 4      ///< "ERROR"
#define FFW_ATE_RC_NOTSUPPORT 5 ///< "NOT SUPPORT"
#define FFW_ATE_RC_NODIALTONE 6 ///< "NO DIALTONE"
#define FFW_ATE_RC_BUSY 7       ///< "BUSY"
#define FFW_ATE_RC_NOANSWER 8   ///< "NO ANSWER"
#define FFW_ATE_RC_INVCMDLINE 9 ///< "INVALID COMMAND LINE"
#define FFW_ATE_RC_CR 10        ///< "\r\n"
#define FFW_ATE_RC_SIMDROP 11   ///< "Sim drop"

/**
 * @brief 
 * 
 * @param engine 
 */
void ffw_cmd_resp_result_code(ffw_handle_t engine, int code);

/**
 * @brief 
 * 
 * @param engine 
 * @param data 
 * @param len 
 */
void ffw_cmd_resp_result_text(ffw_handle_t engine, const void *data, uint32_t len);

/**
 * @brief 
 * 
 * @param engine 
 * @param code 
 */
void ffw_cmd_resp_result_cmecode(ffw_handle_t engine, int code);

/**
 * @brief 
 * 
 * @param engine 
 * @param text 
 * @param length 
 */
void ffw_cmd_resp_info_text(ffw_handle_t engine, const char *text, uint32_t length);

/**
 * @brief 
 * 
 * @param engine 
 * @param code 
 */
void ffw_cmd_resp_info_code(ffw_handle_t engine, int code);

/**
 * @brief 
 * 
 * @param engine 
 * @param text 
 * @param length 
 */
void ffw_cmd_resp_info_data(ffw_handle_t engine, const char *text, uint32_t length);

/**
 * @brief 
 * 
 * @param engine 
 * @param text 
 * @param length 
 */
void ffw_cmd_resp_urc_text(ffw_handle_t engine, const char *text, uint32_t length);

/**
 * @brief 
 * 
 * @param engine 
 * @param code 
 */
void ffw_cmd_resp_urc_code(ffw_handle_t engine, int code);

/**
 * @brief 
 * 
 * @param engine 
 * @param data 
 * @param len 
 */
void ffw_cmd_resp_urc_data(ffw_handle_t engine, const void *data, uint32_t len);


#endif /* CE4480C2_210B_474A_9D94_E3ADD62840E6 */
