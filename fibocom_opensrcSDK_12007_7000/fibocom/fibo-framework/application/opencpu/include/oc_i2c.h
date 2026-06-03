#ifndef __OC_I2C_H__
#define __OC_I2C_H__

#include "ffw_i2c.h"

typedef ffw_i2c_bps_t HAL_I2C_BPS_T;
typedef uint32_t i2c_Handle;
typedef ffw_i2c_slave_t drvI2cSlave_t;

INT32 fibo_i2c_open(HAL_I2C_BPS_T bps, i2c_Handle *i2c_handle);

INT32 fibo_i2c_close(i2c_Handle *i2c_handle);

INT32 fibo_i2c_Write(i2c_Handle i2c_handle, drvI2cSlave_t idAddress, UINT8 *Buffer, UINT8 bufferlen);

INT32 fibo_i2c_Read(i2c_Handle i2c_handle, drvI2cSlave_t idAddress, UINT8 *Buffer, UINT8 bufferlen);

INT32 fibo_i2c_xfer(i2c_Handle i2c_handle, drvI2cSlave_t idAddress, UINT8 *send_Buffer, UINT8 send_bufferlen, UINT8 *recv_Buffer, UINT8 recv_bufferlen);

#endif

