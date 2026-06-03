

#include "mem_map.h"
#include <stdlib.h>

typedef unsigned int uint32_t;

#define FOTA_PATCH_BLOCK_SIZE          (44*1024)

uint32_t fibo_get_data_info_addr(uint32_t offset)
{
	return ((FLASH_FOTA_REGION_START + offset) & (~AP_FLASH_XIP_ADDR));	
}

uint32_t fibo_get_data_info_len()
{
	return (FLASH_FOTA_REGION_LEN - FOTA_PATCH_BLOCK_SIZE);	
}

uint32_t fibo_get_flash_ap_xip_addr()
{
	return AP_FLASH_XIP_ADDR;
}

/*
|<-AP->|<-FS->|<-RESERVE(20KB)->|<-HIB 96KB->|<-FOTA(|<-fibo info used  4KB->|<-fota upgrade used 44KB->|)->|
*/

uint32_t fibo_get_ap_load_addr()
{
	return (AP_FLASH_LOAD_ADDR & (~AP_FLASH_XIP_ADDR));
}

uint32_t fibo_get_ap_load_size()
{
	return AP_FLASH_LOAD_SIZE;
}

uint32_t fibo_get_app_load_addr()
{
#ifdef APP_IMAGE_FLASH_LOAD_ADDR
	return (APP_IMAGE_FLASH_LOAD_ADDR & (~AP_FLASH_XIP_ADDR));
#else
    return 0;
#endif
}

uint32_t fibo_get_app_load_size()
{
#ifdef APP_IMAGE_FLASH_LOAD_SIZE
	return APP_IMAGE_FLASH_LOAD_SIZE;
#else
    return 0;
#endif

}

uint32_t fibo_get_fs_addr()
{
	return FLASH_FS_REGION_START;
}

uint32_t fibo_get_fs_size()
{
	return FLASH_FS_REGION_SIZE;
}

uint32_t  fibo_get_hibbackup_addr()
{
    return FLASH_MEM_BACKUP_ADDR;
}

uint32_t  fibo_get_hibbackup_size()
{
    return FLASH_MEM_BACKUP_SIZE;
}

uint32_t fibo_get_fota_region_start()
{
#ifdef FEATURE_FOTAPAR_ENABLE
	return FLASH_FOTA_REGION_START;
#else
	return 0;
#endif
}
uint32_t fibo_get_fota_region_size()
{
#ifdef FEATURE_FOTAPAR_ENABLE
	return FLASH_FOTA_REGION_LEN;
#else
	return 0;
#endif
}


uint32_t fibo_get_reserve_region_start()
{
    return ((FLASH_FS_REGION_END)&(~AP_FLASH_XIP_ADDR));
}
uint32_t fibo_get_reserve_region_size()
{

    return (FLASH_MEM_BACKUP_NONXIP_ADDR > 0? (FLASH_MEM_BACKUP_NONXIP_ADDR-FLASH_FS_REGION_END):0);
}

uint32_t fibo_get_tts_res_addr()
{

    if(((AP_FLASH_LOAD_ADDR & (~AP_FLASH_XIP_ADDR))+AP_FLASH_LOAD_SIZE) 
        < ((FLASH_FS_REGION_START & (~AP_FLASH_XIP_ADDR))))
    {
        return ((AP_FLASH_LOAD_ADDR & (~AP_FLASH_XIP_ADDR))+AP_FLASH_LOAD_SIZE);
    }
    else
    {
        return 0;
    }


}

void * fibo_get_tts_malloc_addr()
{
    //Clients apply for TTS space on their own here
    return NULL;
}
void fibo_get_tts_free_addr()
{
    //Clients free up TTS space on their own here
}



uint32_t fibo_app_ram_size()
{
#ifdef APP_RAM_DATA_SIZE
    return APP_RAM_DATA_SIZE;
#else
    return 0;
#endif 
}

#ifdef FFW_WENCHAO_CONFIG
uint32_t fibo_get_phonebook_data_addr()
{
    if((FLASH_MEM_PLAT_INFO_NONXIP_ADDR+FLASH_MEM_PLAT_INFO_SIZE)  >= 0x00200000)
    {
        return ((FLASH_MEM_PLAT_INFO_NONXIP_ADDR & (~AP_FLASH_XIP_ADDR))+FLASH_MEM_PLAT_INFO_SIZE);
    }
    else
    {
        return 0;
    }
}
#endif

int fibo_get_app_info(uint32_t *app_flash_addr, uint32_t *app_flash_size, uint32_t *app_ram_size)
{
    if (app_flash_addr)
    {
#ifdef APP_IMAGE_FLASH_LOAD_ADDR
        *app_flash_addr = APP_IMAGE_FLASH_LOAD_ADDR;
#else
        *app_flash_addr = 0;
#endif
    }

    if (app_flash_size)
    {
#ifdef APP_IMAGE_FLASH_LOAD_SIZE
        *app_flash_size = APP_IMAGE_FLASH_LOAD_SIZE;
#else
        *app_flash_size = 0;
#endif
    }

    if (app_ram_size)
    {
#ifdef APP_RAM_DATA_SIZE
        *app_ram_size = APP_RAM_DATA_SIZE;
#else
        *app_ram_size = 0;
#endif
    }

    return 0;
}

//fibocom scboot func en by fibocom liuben
int ffw_secboot_fun_en()
{
#ifdef CONFIG_FFW_FEATURE_SECBOOT
    return 1;
#else
    return 0;
#endif
}

