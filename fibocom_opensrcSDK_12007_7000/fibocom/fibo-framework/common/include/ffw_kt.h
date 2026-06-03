#ifndef LIB_INCLUDE_FFW_KT
#define LIB_INCLUDE_FFW_KT

#ifdef CONFIG_CUST_KR_FEATURE
#include <stdint.h> // for int32_t, uint8_t, int64_t
typedef unsigned int size_t;



typedef struct
{
    uint16_t wms_fibo_ksc5601_value;
    uint16_t wms_fibo_ucs2_value;
}ffw_convert_ksc5601_to_ucs2_map;


typedef struct
{
    uint8_t ip_type;
    char apn_name[100];
    char username[64];
    char password[64];
    uint8_t auth_type;
}ffw_kt_apn;

typedef struct
{

    uint8_t            count;
    uint8_t            refNum;
    uint8_t            maxNum;
    uint8_t            seqNum;
    char            data[176 *2];
}ffw_kt_sms_gtfota;

extern void ffw_KT_defaultpdninit(int simid, int cid,int change_flag);
extern void ffw_KT_PDNset();
extern void ffw_KT_pdnsave();
extern void ffw_KT_defaultPDNsave();
extern void ffw_KT_set_apn_remove_flag(int flag);
extern int ffw_KT_get_apn_remove_flag();
extern void ffw_KT_set_apn_remove_cid(int cid);
extern int ffw_KT_get_apn_remove_cid();







int ffw_KT_gsm_decode7bit(char* pSrc, char* pDst, int nSrcLength);
void ffw_KT_convert_ucs2_to_8bit(uint16_t *in_ucs2, uint8_t in_length, char *out_oct, uint8_t *out_size);

void ffw_KT_handle_ktsms_remote_control(char *tp_ud,int user_data_len,uint8_t simid);
void ffw_KT_handle_ktsms_fota_control(char *srcAddr, char* date, char *message,int message_len);

uint16_t ffw_KT_convert_ksc5601_to_ucs2
(
const uint8_t *ksc5601_data,
uint16_t ksc5601_data_len,
uint16_t *ucs2_buf,
size_t ucs2_buf_sz
);

#endif
#endif
