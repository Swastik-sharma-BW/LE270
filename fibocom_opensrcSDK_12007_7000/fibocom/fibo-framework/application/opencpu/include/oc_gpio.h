#ifndef __OC_GPIO_H__
#define __OC_GPIO_H__

#include "fibo_opencpu_comm.h"

#define SAPP_IO_ID_T uint16_t
#define SAPP_GPIO_CFG_T uint16_t
typedef void (*ISR_CB)(void* param);

typedef struct
{
    bool debounce;   // debounce enabled
    bool intr_enable;   //interrupt enabled, only for GPIO input
    bool intr_level;    // true for level interrupt, false for edge interrupt
    bool falling;    //falling edge or level low interrupt enabled
    bool rising;   //rising edge or level high interrupt enabled
    bool pullen;    //pull enable or disable
    bool pullup;    //pull enable up or down
    ISR_CB cb; //interrupt callback
    void *cb_ctx; //callback parameters
    bool is_wakeup; //wakeup from sleepmode enable or disable
}oc_isr_t;

/**
* @brief set the multiplexing mode
*
* @param id: pin id
* @param mode: the function mode corresponding to the be multiplexed pin
* @return 1: success
          -1: fail
 */
INT32 fibo_gpio_mode_set(SAPP_IO_ID_T id, UINT8 mode);

/**
* @brief Set the input and output mode of the pin
*
* @param id: pin id
* @param cfg: input and output direction
              0: input; 1: output
* @return 1: success
         -1: fail
* @note :the fuction is only valid when the current pin is in GPIO mode
 */
INT32 fibo_gpio_cfg(SAPP_IO_ID_T id, SAPP_GPIO_CFG_T cfg);

/**
* @brief obtain current pin level
*
* @param id: pin id
* @param level: obtain current pin level
* @return 1: success
         -1: fail
* @note :the fuction is only valid when the current pin is in GPIO mode
 */
INT32 fibo_gpio_get(SAPP_IO_ID_T id, UINT8 *level);

/**
* @brief set current pin level
*
* @param id: pin id
* @param level: the level
               0: low level; Non-0: high level
* @return 1: success
         -1: fail
* @note :the fuction is only valid when the current pin is in GPIO mode
 */
INT32 fibo_gpio_set(SAPP_IO_ID_T id, UINT8 level);

/**
* @brief pull up and down in input mode
*
* @param id: pin id
* @param pull up or not
               true: pull up ; false: pull down
* @return 1: success
         -1: fail
* @note :pull the pin up and down internally for the current pin in input mode.
         only valid when the current pin in GPIO mode.
 */
INT32 fibo_gpio_pull_up_or_down(SAPP_IO_ID_T id,bool is_pull_up);

/**
* @brief restore the pin to default mode
*
* @param id: pin id
* @return 1: success
         -1: fail
 */
INT32 fibo_gpio_pull_disable(SAPP_IO_ID_T id);

/**
* @brief restore the pin to default mode from interrupt mode
*
* @param id: pin id
* @return 1: success
         -1: fail
* @note :restore the pin to the default mode, if the default is GPIO mode,
         set it to GPIO's output mode, and pull it high by default.
 */
INT32 fibo_gpio_isr_deinit(SAPP_IO_ID_T id);

/**
* @brief set the pin to interrupt mode
*
* @param id: pin id
* @param isr_cfg: configuration information of interrupt
* @return 1: success
         -1: fail
 */
INT32 fibo_gpio_isr_init(SAPP_IO_ID_T id, oc_isr_t* isr_cfg);

/**
* @brief set the pin to high_resistance mode
*
* @param id: pin id
* @param is_pull_high: configuration high_resistance or not
                        true: high_resistance ; false: not high_resistance
* @return 1: success
         -1: fail
 */
INT32 fibo_gpio_pull_high_resistance(SAPP_IO_ID_T id, bool is_pull_high);

/*
 * fibo_pmu_type:
 *      0-3 is UNiSOC POWER
 *      4-5 is EC POWER
 */
typedef enum
{
    FIBO_POWER_LCD = 0,
    FIBO_POWER_SD,
    FIBO_POWER_SIM1,
    FIBO_POWER_CAMA,
    FIBO_POWER_NORMAL_GPIO,
    FIBO_POWER_AON_GPIO
}fibo_pmu_type;

typedef enum
{
    FIBO_IOVOLT_1_80V = 0,
    FIBO_IOVOLT_3_20V
}fibo_pmu_voltage_t;

/**
* @brief
*
* @param id : fibo_pmu_type
* @param enabled
* @param lp_enabled : Is it enabled under low power consumption
* @return true
* @return false
 */
bool fibo_pmu_ctrl(UINT32 id, BOOL enabled, BOOL lp_enabled);

/**
* @brief
*
* @param pmu_type : fibo_pmu_type
* @param level : fibo_pmu_voltage_t
* @return int
 */
INT32 fibo_pmu_setlevel(UINT8 pmu_type,UINT8 level);

INT32 fibo_get_sim_det_level(void);
#endif
