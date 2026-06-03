#include <stdint.h>
#include <stdbool.h>

/**
* @brief 
* 
* @param void
* @return int 
 */
int ffw_inner_flash_init(void);

/**
* @brief 
* 
* @param pulId 
* @param pulCapacity 
* @return int 
 */
int ffw_inner_flash_info(UINT32 *pulId, UINT32 *pulCapacity);

/**
* @brief 
* 
* @param faddr 
* @param data 
* @param size 
* @return int 
 */
int ffw_inner_flash_read(UINT32 faddr, UINT8 *data,UINT32 size);

/**
* @brief 
* 
* @param faddr 
* @param data 
* @param size 
* @return int 
 */
int ffw_inner_flash_write(UINT32 faddr, UINT8 *data,UINT32 size);

/**
* @brief 
* 
* @param faddr 
* @param size 
* @return int 
 */
int ffw_inner_flash_erase(UINT32 faddr, UINT32 size);
