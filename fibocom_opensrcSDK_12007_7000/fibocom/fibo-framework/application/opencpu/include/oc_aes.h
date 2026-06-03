
#ifndef MBEDTLS_AES_H
#define MBEDTLS_AES_H

#include <stddef.h>
#include <stdint.h>

int fibo_mbedtls_aes_cbc_128(int mode, uint8_t* data, uint32_t data_len, uint8_t* output);
int fibo_mbedtls_aes_cbc_key_128(int mode, unsigned char iv[16], const unsigned char* key, uint8_t* data, uint32_t data_len, uint8_t* output);

#endif /* aes.h */

