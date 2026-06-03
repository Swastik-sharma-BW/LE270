#ifndef DD2FE4FA_C94C_4E6D_A335_F986CC0385FA
#define DD2FE4FA_C94C_4E6D_A335_F986CC0385FA

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    int32_t key_id;
    int8_t long_or_short_press;
    int8_t press_or_release;
} ffw_keypad_info_t;

typedef void (*key_callback)(void* param);


/**
* @brief 
* 
* @param timeout 
* @return true 
* @return false 
 */
bool ffw_keypad_queue_init(uint32_t timeout);

/**
* @brief 
* 
* @param key 
* @return true 
* @return false 
 */
bool ffw_keypad_queue_output(ffw_keypad_info_t *key);

/**
* @brief 
* 
* @return ffw_keypad_info_t 
 */
ffw_keypad_info_t *ffw_pwrkeypad_get_status(void);

/**
* @brief 
* 
* @param timeout 
* @param Ispwroff 
* @return true 
* @return false 
 */
bool ffw_pwrkeypad_config(uint32_t timeout, bool Ispwroff);

/**
* @brief 
* 
* @param en 
* @return true 
* @return false 
 */
bool ffw_keypad_msg_mod(bool en);

/**
* @brief 
* 
* @param pwr_press_cb 
* @param pwr_release_cb 
* @param long_press 
* @return int32_t 
 */
int32_t ffw_pwrkeypad_set_callback(key_callback pwr_press_cb, key_callback pwr_release_cb, uint16_t long_press, void *arg);


/**
* @brief 
* 
* @param timeout 
* @param Ispwroff 
* @return true 
* @return false 
 */
bool ffw_pwrkey_config(uint32_t timeout, bool Ispwroff);


/**
* @brief 
* 
* @param timeout 
* @param Ispwroff 
* @return true 
* @return false 
 */
bool ffw_pwrkeypad_config(uint32_t timeout, bool Ispwroff);

/**
* @brief 
* 
* @param en 
* @return true 
* @return false 
 */
bool ffw_keypad_msg_mod(bool en);

/**
* @brief 
* 
* @param void 
* @return pwrkeystatus 
* @return 255 
 */
uint8_t ffw_get_pwrkeystatus(void);

/**
* @brief
*
* @param pwr_cb
* @param long_press
* @param arg
* @return 1: success
* @return 0: fail
 */
int32_t ffw_set_pwr_callback_ex(key_callback pwr_cb,uint16_t long_press,void *arg);
#endif /* DD2FE4FA_C94C_4E6D_A335_F986CC0385FA */
