#ifndef __FFW_PAL_CERT_H__
#define __FFW_PAL_CERT_H__
#include "ffw_rat.h"

typedef struct _OPER_DEFAULT_APN_INFO
{
    const char *OperatorId;
    ffw_pdp_type_t pdptype;
    const char *Defaultapn;
} OPER_DEFAULT_APN_INFO;

int ffw_pal_cert_defpdp_get(char *iccid_nv, char *cur_iccid, char *imsi_nv, char *cur_imsi, ffw_pdp_type_t *pdptype, char *default_apn);

#endif

