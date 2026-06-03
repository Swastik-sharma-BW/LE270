#ifndef EFA0B99F_6D94_4DB3_A0FB_FDD5B0D3E4BD
#define EFA0B99F_6D94_4DB3_A0FB_FDD5B0D3E4BD

#include <stdint.h>
#include <stdbool.h>

/**
* @brief 
* 
* @param pinsel 
* @param val 
* @param div 
* @return true 
* @return false 
 */
bool ffw_ext_flash_poweron(uint8_t pinsel, uint8_t val, uint8_t div);

/**
* @brief 
* 
* @param Pinsel 
* @return int 
 */
int ffw_ext_flash_init(uint8_t Pinsel);

/**
* @brief 
* 
* @param pulId 
* @param pulCapacity 
* @return int 
 */
int ffw_ext_flash_info(uint32_t *pulId, uint32_t *pulCapacity);

/**
* @brief 
* 
* @param faddr 
* @param data 
* @param size 
* @return int 
 */
int ffw_ext_flash_read(uint32_t faddr, uint8_t *data, uint32_t size);

/**
* @brief 
* 
* @param faddr 
* @param data 
* @param size 
* @return int 
 */
int ffw_ext_flash_write(uint32_t faddr, uint8_t *data, uint32_t size);

/**
* @brief 
* 
* @param faddr 
* @param size 
* @return int 
 */
int ffw_ext_flash_erase(uint32_t faddr, uint32_t size);

/**
* @brief 
* 
* @param mode 
* @return int 
 */
int ffw_ext_flash_mode_set(uint8_t mode);

/**
* @brief 
* 
* @param uladdr_start 
* @param ulsize 
* @param dir 
* @param spi_pin_sel 
* @param format_on_fail 
* @param force_format 
* @return int 
 */
int ffw_ffsmountExtflash(uint32_t uladdr_start, uint32_t ulsize, char *dir, uint8_t spi_pin_sel, bool format_on_fail, bool force_earse);


/**
* @brief 
* 
* @param spi_pin_sel 
* @return int 
 */
int ffw_ext_flash_format(uint8_t spi_pin_sel);

int ffw_extflash_read_uniqueid(uint8_t *uid, uint8_t *uid_len);

int ffw_extflash_write_otp(uint32_t otp_addr, const void *data, uint32_t size);

int ffw_extflash_read_otp(uint32_t otp_addr, void *data, uint32_t size);

#endif /* EFA0B99F_6D94_4DB3_A0FB_FDD5B0D3E4BD */
