#ifndef  BMS_ONEWIRE_H
#define  BMS_ONEWIRE_H
#include <stdint.h>
#include <stdbool.h>

typedef void (*one_wire_recv_callback_t)(uint8_t *data, int len);
typedef void (*one_wire_send_callback_t)(void);

typedef struct
{
    uint32_t syn_low_std;                    //Synchronization signal low-level pulse width
    uint32_t syn_low_range;                  //Error in receiving low-level pulse width of synchronous signal
    uint32_t syn_high_std;                   //Synchronous signal high-level pulse width
    uint32_t syn_high_range;                 //Error in receiving high-level pulse width of synchronous signal

    uint32_t stop_low_std;                   //Stop signal low level pulse width
    uint32_t stop_low_range;                 //Allow error in low-level pulse width of stop signal
    uint32_t stop_high_min;                  //Minimum high-level pulse width of stop signal

    uint32_t bit_period;                     //Cycle of data bits
    uint32_t bit1_low_std;                   //Logic 1 low level pulse width
    uint32_t bit1_low_range;                 //Error in low-level pulse width of bit1 signal
    uint32_t bit1_high_range;                //Error in high-level pulse width of bit1 signal

    uint32_t bit0_low_std;                   //Logic 0 low level pulse width
    uint32_t bit0_low_range;                 //Error in low-level pulse width of bit0 signal
    uint32_t bit0_high_range;                //Error in high-level pulse width of bit0 signal
} ffw_one_wire_cycle_t;

typedef struct
{
    uint8_t rx_pin;                           //recv pin
    uint8_t tx_pin;                           //send pin
    ffw_one_wire_cycle_t cycle_config;        //Cycle configuration
    uint32_t recv_max_size;                   //Maximum length for receiving buf
    uint8_t tx_timer_num;                     //tx's timer, support 0~5 channel.if pwm_enable true, tx_timer_num must equals pwm num.
    uint8_t tx_pwm_function_num;
    bool pwm_enable;                          //TX PWM transmission,tx requires pins that support PWM functionality
    bool transfer_msb;                        //msb enable
    one_wire_recv_callback_t rx_callback;     //recv callback
    one_wire_send_callback_t tx_callback;     //send callback
    void *reserve;                            //Reserved, single line communication may be added in the future
} ffw_one_wire_config_t;

int ffw_one_wire_communication_deinit(void);

int ffw_one_wire_communication_send(uint8_t *data, uint16_t len);

int ffw_one_wire_communication_init(ffw_one_wire_config_t *config);
#endif /* BMS_ONEWIRE_H */

