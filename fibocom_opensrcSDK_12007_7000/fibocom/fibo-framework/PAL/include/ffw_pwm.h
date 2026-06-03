#ifndef F16C4E33_FB46_48F8_A0D8_858A1CA1C2ED
#define F16C4E33_FB46_48F8_A0D8_858A1CA1C2ED

#include <stdint.h>
#include <stdbool.h>

/**
* @brief 
* 
* @param index 
* @return int 
 */
int ffw_pwm_open(int32_t index);

/**
* @brief 
* 
* @return int 
 */
int ffw_pwm_close(int32_t index);

/**
* @brief 
* 
* @param index 
* @param duty 
* @param arr 
* @param psc 
* @return int 
 */
int ffw_pwm_config(int32_t index, uint16_t duty, uint16_t arr, uint16_t psc);

/**
* @brief
*
* @return int
 */
int ffw_pwm_close_all(void);

#endif /* F16C4E33_FB46_48F8_A0D8_858A1CA1C2ED */
