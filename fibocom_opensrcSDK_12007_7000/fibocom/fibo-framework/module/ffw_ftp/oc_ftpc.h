#ifndef FFW_FTP_OC_FTPC
#define FFW_FTP_OC_FTPC

#include <stdint.h>
#include <stdbool.h>
//#include "fibo_opencpu_comm.h"

typedef void (*on_ftp_progress_t)(uint32_t total, uint32_t recv_len, void *arg);

/**
* @brief 
* 
* @param url 
* @param size 
* @return int 
 */
int ffw_ftpc_get_file_size(const char *url, uint32_t *size);

/**
* @brief 
* 
* @param url 
* @param buf 
* @param len 
* @param data_len 
* @return int 
 */
int ffw_ftpc_download_buffer(const char *url, uint8_t *buf, uint32_t len, int *data_len, uint32_t timeout, on_ftp_progress_t on_progress, void *arg);

/**
* @brief 
* 
* @param url 
* @param filename 
* @param timeout 
* @param on_progress 
* @param arg 
* @return int 
 */
int ffw_ftpc_download_filename(const char *url, const char *filename, uint32_t timeout, on_ftp_progress_t on_progress, void *arg);

/**
* @brief 
* 
* @param url 
* @param filepath 
* @return int 
 */
int ffw_ftpc_upload(const char *url, const char *filepath);

#endif /* FFW_FTP_OC_FTPC */
