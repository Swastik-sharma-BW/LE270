#ifndef E4EB28A8_2795_4C8D_9064_E46B4AA674D6
#define E4EB28A8_2795_4C8D_9064_E46B4AA674D6

#include <stdint.h>
#include <stdbool.h>

#define OSI_MAKE_TAG(a, b, c, d) ((unsigned)(a) | ((unsigned)(b) << 8) | ((unsigned)(c) << 16) | ((unsigned)(d) << 24))

#define DRV_NAME_SPI1 OSI_MAKE_TAG('S', 'P', 'I', '1')          // SPI1
#define DRV_NAME_SPI2 OSI_MAKE_TAG('S', 'P', 'I', '2')          // SPI2
#define DRV_NAME_SPI3 OSI_MAKE_TAG('S', 'P', 'I', '3')          // SPI3

typedef enum
{
    SPI_WORK_STANDARD,
    SPI_WORK_SYNC,
    SPI_WORK_S8,
    SPI_WORK_3_WIRE,
    SPI_WORK_SLAVE,
} ffw_spi_work_mode_t;

typedef enum
{
    CLK_SOURCE_RTC32,
    CLK_SOURCE_XTAL26,
    CLK_SOURCE_RC26_26,
    CLK_SOURCE_GNSSPLL_133m,
    CLK_SOURCE_APLL_167,
} ffw_spi_clk_source_t;

typedef enum
{
    SPI_CS_ACTIVE_HIGH,
    SPI_CS_ACTIVE_LOW,
} ffw_spi_cspol_t;

typedef enum
{
    SPI_CPHA_1Edge,
    SPI_CPHA_2Edge,
} ffw_spi_cpha_pol_t;

typedef enum
{
    SPI_DI_0 = 0,
    SPI_DI_1,
    SPI_DI_2,
} ffw_spi_input_sel_t;

typedef enum
{
    SPI_DIRECT_POLLING = 0,
    SPI_DIRECT_IRQ,
    SPI_DMA_POLLING,
    SPI_DMA_IRQ,
} ffw_spi_transfer_mode_t;

typedef enum
{
    SPI_CPOL_LOW = 0,
    SPI_CPOL_HIGH,
} ffw_spi_work_cpolpol_t;

typedef enum
{
    SPI_I2C_CS0 = 0,
    SPI_I2C_CS1,
    SPI_I2C_CS2,
    SPI_I2C_CS3,
    SPI_GPIO_CS,
} ffw_spi_cssel_t;

typedef struct
{
    bool inputEn;
    bool lsb;
    uint8_t oe_delay;
    uint32_t name;
    uint32_t framesize;
    uint32_t baud;
    ffw_spi_work_mode_t work_mode;
    ffw_spi_clk_source_t clk_source;
    ffw_spi_cspol_t cs_polarity0;
    ffw_spi_cspol_t cs_polarity1;
    ffw_spi_work_cpolpol_t cpol;
    ffw_spi_cpha_pol_t cpha;
    ffw_spi_input_sel_t input_sel;
    ffw_spi_transfer_mode_t transmode;
    uint8_t rxf_full_thrhld;
    uint8_t rxf_empty_thrhld;
    uint8_t txf_full_thrhld;
    uint8_t txf_empty_thrhld;
    uint32_t spi_tx_data_len;
    uint32_t spi_rx_data_len;
} ffw_spi_config_t;

typedef struct
{
    uint32_t bits_per_word;
    uint32_t speed_hz;
    ffw_spi_cssel_t cs;
    ffw_spi_cspol_t cs_polarity;
    bool lsb;
}ffw_spi_soc_t;

/**
* @brief 
* 
* @param cfg 
* @param spiFd 
* @return int 
 */
int ffw_spi_open(ffw_spi_config_t cfg, uint32_t *spiFd);

/**
* @brief 
* 
* @param spiFd 
* @return int 
 */
int ffw_spi_close(uint32_t spiFd);

/**
* @brief 
* 
* @param spiFd 
* @param spiIoc 
* @param sendaddr 
* @param size 
* @return int 
 */
int ffw_spi_send(uint32_t spiFd, ffw_spi_soc_t spiIoc, void *sendaddr, uint32_t size);

/**
* @brief 
* 
* @param spiFd 
* @param spiIoc 
* @param readaddr 
* @param size 
* @return int 
 */
int ffw_spi_recv(uint32_t spiFd, ffw_spi_soc_t spiIoc, void *readaddr, uint32_t size);

/**
* @brief 
* 
* @param spiFd 
* @param spiIoc 
* @param sendaddr 
* @param readaddr 
* @param size 
* @return int 
 */
int ffw_spi_send_rcv(uint32_t spiFd, ffw_spi_soc_t spiIoc, void *sendaddr, void *readaddr, uint32_t size);

#endif /* E4EB28A8_2795_4C8D_9064_E46B4AA674D6 */
