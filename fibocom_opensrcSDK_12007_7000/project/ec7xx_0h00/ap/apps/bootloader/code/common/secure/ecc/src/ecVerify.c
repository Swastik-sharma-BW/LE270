/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#include "uECC.h"

#include <stdio.h>
#ifdef FEATURE_BOOTLOADER_PROJECT_ENABLE
#include "common.h"
#else
#include DEBUG_LOG_HEADER_FILE
#endif
#define SHA224_SIZE 28
#define SHA256_SIZE 32

uint32_t VerifySignature(uint8_t is224, uint8_t *hash, uint8_t *ecsda, uint8_t *pubKey)
{
    const struct uECC_Curve_t *curve = uECC_secp224r1();
    if(is224 == 0){
        ECPLAT_PRINTF(UNILOG_FOTA, ecVerify_1, P_ERROR, "not support 256\n");
    }

    if (NULL == pubKey)
    {
        ECPLAT_PRINTF(UNILOG_FOTA, ecVerify_2, P_ERROR, "no key\n");
        return 0x6c;
    }

    if (0 == uECC_valid_public_key(pubKey, curve))
    {
        ECPLAT_PRINTF(UNILOG_FOTA, ecVerify_3, P_ERROR, "key invalid\n");
        return 0x6c;
    }

    if (1 != uECC_verify(pubKey, hash, (is224 ? SHA224_SIZE : SHA256_SIZE), ecsda, curve)) //0:fail, 1:success
    {
        ECPLAT_PRINTF(UNILOG_FOTA, ecVerify_4, P_ERROR, "sign invalid\n");
        return 0x6c;
    }

    ECPLAT_PRINTF(UNILOG_FOTA, ecVerify_5, P_SIG, "sign ok!\n");

    return 0;
}


