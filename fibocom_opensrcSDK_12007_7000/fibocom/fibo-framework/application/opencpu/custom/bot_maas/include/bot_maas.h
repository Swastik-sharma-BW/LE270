
#include "stdint.h"
#include "ffw_buffer.h"

typedef struct bypass_botpub
{
    void *engine;
    unsigned size;
    ffw_buffer_t data;
} bypass_botpub_t;

void cbb_maasregstatus(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_maasconnstate(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_maasdevreg(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_maaspub(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_maasconfig(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_maassdkversion(int32_t cmd_type, void *params, int32_t params_count, void *engine);
void cbb_maaschannel(int32_t cmd_type, void *params, int32_t params_count, void *engine);
//void cbb_maasdebug(int32_t cmd_type, void *params, int32_t params_count, void *engine);
