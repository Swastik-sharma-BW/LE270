#ifndef AE554DFA_20FC_497A_849B_E787CB572297
#define AE554DFA_20FC_497A_849B_E787CB572297

#include <stdint.h>
#include <stdbool.h>
#include <ffw_timer.h>

typedef void (*ffw_isr_callback_t)(void* param);

typedef enum
{
    FFW_HAL_GPIO_DATA_LOW,
    FFW_HAL_GPIO_DATA_HIGH,
    FFW_HAL_GPIO_DATA_MAX
} ffw_gpio_data_t;

typedef enum
{
    FFW_DRV_GPIO_INPUT,
    FFW_DRV_GPIO_OUTPUT
} ffw_gpio_mode_t;

typedef struct
{
    uint32_t id;

    ffw_gpio_mode_t mode;
    bool intr_enabled;
    bool intr_level;
    bool debounce;
    bool rising;
    bool falling;

    ffw_isr_callback_t cb;
    void *cb_ctx;
}ffw_gpio_t;

typedef struct
{
    ffw_gpio_t *drv_gpio;
    ffw_gpio_mode_t *drv_isr;
    ffw_gpio_data_t level;
    ffw_isr_callback_t isr_callback;
} hal_gpioisr_t;

typedef struct
{
    bool debounce;   // debounce enabled
    bool intr_enable;   //interrupt enabled, only for GPIO input
    bool intr_level;    // true for level interrupt, false for edge interrupt
    bool falling;    //falling edge or level low interrupt enabled
    bool rising;   //rising edge or level high interrupt enabled
    bool pullen;    //pull enable or disable
    bool pullup;    //pull enable up or down
    ffw_isr_callback_t cb; //interrupt callback
    void *cb_ctx;    //callback parameters
    bool is_wakeup;  //wakeup from sleepmode enable or disable
}ffw_gpioisr_t;

typedef struct
{
    uint16_t  pin_id;
    uint16_t  gpio;
    uint16_t  port;
    uint16_t  pin;
    uint16_t  portIrqMask;
    uint16_t  level;
    ffw_isr_callback_t cb; //interrupt callback
    void *cb_ctx;
    ffw_timerp_t ptimer;
    bool is_wakeup;
    bool debounce;
}ffw_isr_cb_table;

/**
 * @brief 初始化gpio mode
 * 
 * @param pin_id pin id
 * @return int
 * @retval FFW_R_SUCCESS: 成功
 * @retval FFW_R_FAILED: 失败
 */
int ffw_gpiomode_init(uint32_t pin_id);

/**
 * @brief 设置gpio的模式
 *
 * @param pin_id pin id
 * @param mode 模式
 * @return int
 * @retval FFW_R_SUCCESS: 成功
 * @retval FFW_R_FAILED: 失败
 */
int ffw_gpiomode_set(uint32_t pin_id, uint32_t mode);

/**
 * @brief set gpio as gpio_mode
 *
 * @param pin_id pin id
 * @return int
 * @retval FFW_R_SUCCESS: 成功
 * @retval FFW_R_FAILED: 失败
 */
int ffw_gpiomode_set_gpiomode(uint32_t pin_id);

/**
* @brief
*
* @param pin_id
* @return int
 */
int ffw_getgpioindex(uint32_t pin_id);

/**
* @brief
*
* @param pin_id
* @return int
 */
int ffw_is_vaild_pin(uint32_t pin_id);

/**
* @brief
*
* @param pin_id
* @return true
* @return false
 */
bool ffw_gpio_moderelease(uint32_t pin_id);

/**
* @brief
*
* @param pin_id
* @param inout
* @return true
* @return false
 */
bool ffw_gpio_setDir(uint32_t pin_id, ffw_gpio_mode_t inout);

/**
* @brief
* 
* @param pin_id
* @param level
* @return true
* @return false
 */
bool ffw_gpio_getLevel(uint32_t pin_id, ffw_gpio_data_t *level);

/**
* @brief
* 
* @param pin_id
* @param out_level
* @return true
* @return false
 */
bool ffw_gpio_setLevel(uint32_t pin_id, ffw_gpio_data_t out_level);

/**
* @brief
*
* @param pin_id
* @return true
* @return false
 */
bool ffw_gpio_isr_deinit(uint32_t pin_id);

/**
* @brief
*
* @param pin_id
* @param gpio_Isr
* @return true
* @return false
 */
bool ffw_gpio_isr_init(uint32_t  pin_id, ffw_gpioisr_t *gpio_Isr);

/**
* @brief
*
* @param pin_id
* @param is_pull_up
* @return true
* @return false
 */
bool ffw_gpio_pull_up_or_down(uint32_t pin_id,bool is_pull_up);


/**
* @brief
*
* @param pin_id
* @return true
* @return false
 */
bool ffw_gpio_pull_disable(uint32_t pin_id);

/**
* @brief
*
* @param pin_id
* @param is_resistance
* @return true
* @return false
 */
bool ffw_gpio_pull_high_resistance(uint16_t pin_id, bool is_resistance);

typedef enum
{
    NORMAL_GPIO = 4,
    AON_GPIO
}ffw_pal_pmu_type;

/**
* @brief
*
* @param id : ffw_pal_pmu_type
* @param enabled
* @param lp_enabled : Is it enabled under low power consumption
* @return true
* @return false
* @note NORMAL_GPIO and AON_GPIO default enabled.
* @note NORMAL_GPIO will power down at low power consumption and cannot be maintained
 */
bool ffw_pal_pmu_ctrl(uint32_t id, bool enabled, bool lp_enabled);

/**
* @brief
*
* @param pmu_type : 0 is NORMAL_GPIO, 1 is AON_GPIO
* @param level : IOVoltageSel_t
* @return int
* @note NORMAL_GPIO and AON_GPIO voltage range is 1.65V~3.40V..
 */
int32_t ffw_pal_pmu_setlevel(uint8_t pmu_type, uint8_t level);

#endif /* AE554DFA_20FC_497A_849B_E787CB572297 */
