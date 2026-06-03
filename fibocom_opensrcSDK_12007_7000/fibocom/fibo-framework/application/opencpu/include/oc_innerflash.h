#ifndef __OC_INNERFLASH_H__
#define __OC_INNERFLASH_H__

INT32 fibo_inner_flash_init(void);

INT32 fibo_inner_flash_info(UINT32 *pulId, UINT32 *pulCapacity);

INT32 fibo_inner_flash_read(UINT32 faddr, UINT8 *data,UINT32 size);

INT32 fibo_inner_flash_write(UINT32 faddr, UINT8 *data,UINT32 size);

INT32 fibo_inner_flash_erase(UINT32 faddr, UINT32 size);

#endif

