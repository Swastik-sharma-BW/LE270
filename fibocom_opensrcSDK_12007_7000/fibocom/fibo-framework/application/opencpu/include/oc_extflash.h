#ifndef __OC_EXTFLASH_H__
#define __OC_EXTFLASH_H__

INT32 fibo_ext_flash_init(uint8_t Pinsel);

INT32 fibo_ext_flash_read(UINT32 faddr, UINT8 *data,UINT32 size);

INT32 fibo_ext_flash_write(UINT32 faddr, UINT8 *data,UINT32 size);

INT32 fibo_ext_flash_erase(UINT32 faddr,UINT32 size);

INT32 fibo_ext_flash_info(UINT32 *pulId, UINT32 *pulCapacity);

INT32 fibo_ffsmountExtflash(UINT32 uladdr_start, UINT32 ulsize,char *dir,UINT8 spi_pin_sel,bool format_on_fail,bool force_format);

void fibo_ext_flash_mode_set(uint8_t mode);

bool fibo_ext_flash_poweron(uint8_t pinsel, uint8_t val, uint8_t div);

int32 fibo_extflash_read_uniqueid(uint8 *uid, uint8 *uid_len);

int32 fibo_extflash_write_otp(uint32 otp_addr, const uint8 *data, uint32 size);

int32 fibo_extflash_read_otp(uint32 otp_addr, uint8 *data, uint32 size);
#endif

