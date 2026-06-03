#ifndef CBB_INCLUDE_CBB_CTDIAGNOSE
#define CBB_INCLUDE_CBB_CTDIAGNOSE

#include "stdint.h"


int32_t cbb_ctdiagnose(int32_t cmd_type, void *params, int32_t params_count, void *engine);
int32_t cbb_ctdiagnoseusr(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_ctmqttpub(int32_t cmd_type, void *params, int32_t params_count, void *engine);


#endif /* BD2AD4FE_B9FC_44A8_AC95_5C22C2901AC6 */
