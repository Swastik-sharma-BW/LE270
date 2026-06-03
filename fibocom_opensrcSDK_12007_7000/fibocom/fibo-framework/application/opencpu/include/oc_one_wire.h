#ifndef _OC_ONE_WIRE_
#define _OC_ONE_WIRE_
#include "ffw_one_wire.h"

typedef ffw_one_wire_cycle_t fibo_one_wire_cycle_t;

typedef ffw_one_wire_config_t fibo_one_wire_config_t;

int fibo_one_wire_communication_deinit(void);

int fibo_one_wire_communication_send(uint8_t *data, uint16_t len);

int fibo_one_wire_communication_init(fibo_one_wire_config_t *config);

#endif