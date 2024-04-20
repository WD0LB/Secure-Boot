#include "platform.h"
#include "mbedtls_config.h"
#include "ctr_drbg.h"
#include "entropy.h"
#include "pk.h"

mbedtls_pk_context* pk_context;

mbedtls_pk_init(pk_context);
// Load the public key into pk_context

int verify_signature(unsigned *hash, size_t hash_len, unsigned char *signature, size_t sig_len, mbedtls_pk_context *pk_context) {
    int result = mbedtls_pk_verify(pk_context, MBEDTLS_MD_SHA256, hash, hash_len, signature, sig_len);
    return result;
}