#ifndef B9F875E1_12B8_49E4_A889_52B67E47A94E
#define B9F875E1_12B8_49E4_A889_52B67E47A94E

#include <stdint.h>

//int fibo_app_start(void);
void *fibo_app_start(uint32_t appimg_flash_offset,uint32_t appimg_flash_size,uint32_t appimg_ram_offset,uint32_t appimg_ram_size,void *parm);
int app_header_check(char *header, uint32_t app_size, unsigned int *len, unsigned int *crc_32);
uint32_t crc32_calc_update(const void *p, int len, uint32_t crc);
int fibo_get_old_image_size();

#endif /* B9F875E1_12B8_49E4_A889_52B67E47A94E */
