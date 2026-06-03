#ifndef __OC_SPI_H__
#define __OC_SPI_H__

#include "ffw_spi.h"

typedef uint32_t SPIHANDLE;
typedef ffw_spi_soc_t SPI_IOC;
typedef ffw_spi_config_t drvSpiConfig_t;

INT32 fibo_spi_open(drvSpiConfig_t cfg, SPIHANDLE *spiFd);

INT32 fibo_spi_close(SPIHANDLE *spiFd);

INT32 fibo_spi_send(SPIHANDLE spiFd, SPI_IOC spiIoc, void *sendaddr, uint32_t size);

INT32 fibo_spi_recv(SPIHANDLE spiFd, SPI_IOC spiIoc, void *readaddr, uint32_t size);

INT32 fibo_spi_send_rcv(SPIHANDLE spiFd, SPI_IOC spiIoc, void *sendaddr, void *readaddr, uint32_t size);

INT32 fibo_spi_pinctrl(void);

#endif

