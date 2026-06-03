
#ifdef CONFIG_FFW_FLIGHT_MODE_SUPPORT
#ifndef _FFW_PAL_FLIGHT_H_
#define _FFW_PAL_FLIGHT_H_

bool flight_pal_get_info(int *irq_n,int *gpio,int *prio);
bool flight_pal_get_pin_level(int gpio);
bool flight_pal_get_pin_level_hotfix(int irq,int gpio);
int flight_pal_get_cfun(uint8_t *p_cfun);
int flight_pal_set_cfun(uint8_t fun);
int flight_pal_wait_ready(void);
int flight_pal_config_irq(int irq_n,int gpio,int on,int irq_level);
void flight_pal_enable_irq(int irq);
void flight_pal_disable_irq(int irq);
void flight_pal_clear_pending_irq(int irq);
int flight_pal_register_irq_cb(int (*cb)(uint32_t));

#endif
#endif

