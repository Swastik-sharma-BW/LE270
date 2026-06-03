#ifndef  __MEM_ZONE_H__
#define  __MEM_ZONE_H__

#if !defined(TYPE_EC716)
#define FOTA_PATH   "/FBFOTA"
#define FOTA_BIN    "/FBFOTA/delta.par"
#define FOTA_FLAG   "/fibocom/ffw_ota_log.bin"

#define FULL_APP_PATH   "/FBFOTA"
#define FULL_APP_BIN    "/FBFOTA/app.bin"

#else
#define FOTA_PATH   "/"
#define FOTA_BIN    "/delta.par"
#define FOTA_FLAG   "/ffw_ota_log.bin"

#define FULL_APP_PATH   "/"
#define FULL_APP_BIN    "/app.bin"

#endif

#if defined(TYPE_EC718S) || defined(TYPE_EC716S)
#define FIBOCOM_MAX_DELTA_FILE_SIZE     (100*1024)   //MAX Delta file 100KB, FFS 104KB  
#else
#define FIBOCOM_MAX_DELTA_FILE_SIZE     (700*1024)   //MAX Delta file 700KB, FFS 844KB  
#endif

//---------------The above code cannot be modified


typedef enum
{
    OPTION_MAIN_FM_DIFF_PATCH_DL,    //���̼��������
    OPTION_CUSTOM_APP_WHOLE_PATCH_DL,//APP��������
    OPTION_CUSTOM_APP_DIFF_PATCH_DL,  //APP�������    
    OPTION_MAIN_FM_APP_DIFF_PATCH_DL,//���̼���APP�����һ�𣬲������
} oc_fota_option_e;

typedef  enum  back_offsize
{
	E_BACK_BASE,
    E_BACK_FOTA  = E_BACK_BASE + 40,  // 40
    E_BACK_IMEI2 = E_BACK_FOTA + 18,
    E_BACK_APP_FULL_UPDATE = E_BACK_IMEI2 + 40,
    E_BACK_APP_DIFF_UPDATE = E_BACK_APP_FULL_UPDATE + 40,
    E_BACK_FM_APP_DIFF_UPDATE = E_BACK_APP_DIFF_UPDATE + 40,
    E_BACK_OPTION = E_BACK_FM_APP_DIFF_UPDATE + 40,
    E_BACK_END
}back_offsize;

uint32_t fibo_get_flash_ap_xip_addr();
uint32_t fibo_get_ap_load_addr();
uint32_t fibo_get_ap_load_size();
uint32_t fibo_get_app_load_addr();
uint32_t fibo_get_app_load_size();
uint32_t fibo_get_reserve_region_start();
uint32_t fibo_get_reserve_region_size();
uint32_t fibo_get_fota_region_start();
uint32_t fibo_get_fota_region_size();
uint32_t fibo_get_fs_addr();
uint32_t fibo_get_fs_size();
uint32_t fibo_get_data_info_addr(uint32_t offset);
uint32_t fibo_get_data_info_len();
uint32_t fibo_get_tts_res_addr();
uint32_t fibo_get_hibbackup_addr();
uint32_t fibo_get_hibbackup_size();
uint32_t fibo_app_ram_size();
void * fibo_get_tts_malloc_addr();
void fibo_get_tts_free_addr();

int fibo_get_app_info(uint32_t *app_flash_addr, uint32_t *app_flash_size, uint32_t *app_ram_size);
int ffw_secboot_fun_en();


#endif
