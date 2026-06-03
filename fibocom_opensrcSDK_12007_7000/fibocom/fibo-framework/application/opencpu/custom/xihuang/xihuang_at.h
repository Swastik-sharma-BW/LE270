#ifndef _XIHUANG_AT_H_
#define _XIHUANG_AT_H_

#include <stdbool.h>
#include <stdint.h>

void cbb_XHCGMM(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_XHCGMI(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_XHATI(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_XHEN(int32_t cmd_type, void *params, int32_t params_count, void *engine);

#endif
