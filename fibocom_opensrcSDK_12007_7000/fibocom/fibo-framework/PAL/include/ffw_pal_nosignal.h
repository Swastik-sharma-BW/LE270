#include <stdio.h>
#include <stdint.h>

#ifndef _FFW_PAL_NONSIGNAL_H_
#define _FFW_PAL_NONSIGNAL_H_


typedef unsigned long ffw_uint32;
typedef unsigned short ffw_uint16;
typedef unsigned char ffw_uint8;
typedef signed long ffw_int32;

#define FFW_REQ_MODE_LTE_RX_TEST                                                3
#define FFW_REQ_MODE_LTE_TX_TEST_ON                                             4
#define FFW_REQ_MODE_LTE_TX_TEST_OFF                                            5


/*************STRUCT*****************/
typedef struct {
    ffw_int32 rat;
    ffw_int32 band;
    ffw_int32 bandwith;
    ffw_int32 dl_channnel;
    ffw_int32 ul_channnel;
    ffw_int32 antnum;
    ffw_int32 scs;
    ffw_int32 expect_lvl;
    ffw_int32 msc;
    ffw_int32 rb_start;
    ffw_int32 rb_num;
    ffw_int32 tx_pwr;
} ffw_pal_nonsignal_req_param_t;


int ffw_pal_nonsignal_nstmode_set(void *engine,uint8_t simid,int32_t mode);
int ffw_pal_nonsignal_nstmode_get(void *engine,uint8_t simid,int32_t *mode);
int32_t ffw_pal_nonsignal_mode_set(void *engine,ffw_int32 mode);
int ffw_pal_nonsignal_param_set(ffw_pal_nonsignal_req_param_t *req);
int32_t ffw_nonsignal_rsp(uint8_t chanId);


#endif
