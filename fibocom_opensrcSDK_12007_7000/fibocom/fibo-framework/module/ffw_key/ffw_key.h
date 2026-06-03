/**
 * @file ffw_key.h
 * @author Ethan Guo (Guo Junhuang) (ethan.guo@fibocom.com)
 * @brief Head file of key function service, including GPIO key and keypad.
 * @version 0.1
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _FFW_KEY_H_
#define _FFW_KEY_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    KEY_MAP_POWER = 0, ///< power key
    KEY_MAP_0,         ///< '0'
    KEY_MAP_1,         ///< '1'
    KEY_MAP_2,         ///< '2'
    KEY_MAP_3,         ///< '3'
    KEY_MAP_4,         ///< '4'
    KEY_MAP_5,         ///< '5'
    KEY_MAP_6,         ///< '6'
    KEY_MAP_7,         ///< '7'
    KEY_MAP_8,         ///< '8'
    KEY_MAP_9,         ///< '9'
    KEY_MAP_STAR,      ///< '*'
    KEY_MAP_SHARP,     ///< '#'
    KEY_MAP_LEFT,      ///< left
    KEY_MAP_RIGHT,     ///< right
    KEY_MAP_UP,        ///< up
    KEY_MAP_DOWN,      ///< down
    KEY_MAP_OK,        ///< ok
    KEY_MAP_CANCLE,    ///< cancel
    KEY_MAP_SOFT_L,    ///< left soft key
    KEY_MAP_SOFT_R,    ///< rigfht soft key
    KEY_MAP_SIM1,      ///< customized
    KEY_MAP_SIM2,      ///< customized
    KEY_MAP_EXT1,
    KEY_MAP_EXT2,
    KEY_MAP_EXT3,
    KEY_MAP_EXT4,
    KEY_MAP_EXT5,
    KEY_MAP_EXT6,
    KEY_MAP_EXT7,
    KEY_MAP_EXT8,
    KEY_MAP_EXT9,
    KEY_MAP_EXT10,
    KEY_MAP_EXT11,
    KEY_MAP_EXT12,
    KEY_MAP_EXT13,
    KEY_MAP_SOFT_M,    /// soft key

    GPIO_KEY_MAP_1,     // GPIO KEY
    GPIO_KEY_MAP_2,
    GPIO_KEY_MAP_3,
    GPIO_KEY_MAP_4,

    KEY_MAP_MAX_COUNT  ///< total count
} ffw_key_map_t __attribute__((unused));

typedef enum
{
    KEY_STATE_UNKNOWN = 0,
    KEY_STATE_PRESS = (1 << 0),
    KEY_STATE_RELEASE = (1 << 1),
    KEY_STATE_SHORT_PRESS = 3,
    KEY_STATE_SHORT_RELEASE,
    KEY_STATE_LONG_PRESS,
    KEY_STATE_LONG_RELEASE,
} ffw_key_state_t __attribute__((unused));


typedef void (*ffw_key_cb_t)(int32_t key, int32_t state, void *ctx);

int32_t ffw_srv_key_init(void);

int32_t ffw_gpio_key_tab_register(void);
int32_t ffw_gpio_key_tab_release(void);

int32_t ffw_key_register_callback(ffw_key_cb_t cb, void *ctx, uint32_t long_press_time);
int32_t ffw_key_release_callback(void);
bool ffw_key_get_has_registered(void);
int32_t ffw_key_pwrkey_cb(int32_t state);

#ifdef __cplusplus
}
#endif
#endif
