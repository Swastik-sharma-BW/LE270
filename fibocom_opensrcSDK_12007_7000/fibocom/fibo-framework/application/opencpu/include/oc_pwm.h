#ifndef __OC_PWM_H__
#define __OC_PWM_H__

/**
 * @brief
 *
 * @param index: pwm index
 * @return true
 * @return false
 */
bool fibo_pwm_open(int32_t index);

/**
 * @brief
 *
 * @param index : pwm index
 * @param duty
 * @param arr
 * @param psc
 * @return void
 * @note:
 *    Duty cycle = duty / (arr + 1):High level as a percentage of one PWM cycle
 *    PWM frequency = 26MHz / (psc + 1) / (arr + 1)
 */
void fibo_pwm_config(int32_t index, uint16_t duty, uint16_t arr, uint16_t psc);

/**
 * @brief
 *
 * @param index : void
 * @return void
 */
void fibo_pwm_close(void);

/**
 * @brief
 *
 * @param index : pwm index
 * @return void
 */
bool fibo_pwm_close_v2(int32_t index);

#endif

