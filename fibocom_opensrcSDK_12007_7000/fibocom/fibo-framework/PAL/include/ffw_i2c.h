#ifndef C22BBCD2_7AD8_4205_822E_86AD48492B19
#define C22BBCD2_7AD8_4205_822E_86AD48492B19

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
   int i2c_name;
   int value;
}ffw_i2c_bps_t;

typedef struct
{
    uint8_t addr_device;   ///< device address
    uint8_t addr_data;     ///< data high 8 bits
    uint8_t addr_data_low; ///< data low 8 bits
    bool reg_16bit;        ///< true:16bit reg address;false:8bit reg address
    bool is_use;            // reg address or not
    bool restart_no_stop;   // restart without stop condition after sending reg address to read data, default as false
} ffw_i2c_slave_t;

int ffw_i2c_open(ffw_i2c_bps_t bps, uint32_t *i2c_handle);
int ffw_i2c_close(uint32_t i2c_handle);
int ffw_i2c_write(uint32_t i2c_handle, ffw_i2c_slave_t slave_addr, uint8_t *buffer, uint8_t bufferlen);
int ffw_i2c_read(uint32_t i2c_handle, ffw_i2c_slave_t slave_addr, uint8_t *buffer, uint8_t bufferlen);
int ffw_i2c_xfer(uint32_t i2c_handle, ffw_i2c_slave_t slave_addr, uint8_t *send_buffer, uint8_t send_bufferlen, uint8_t *recv_buffer, uint8_t recv_bufferlen);

#endif /* C22BBCD2_7AD8_4205_822E_86AD48492B19 */
