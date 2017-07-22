//
//  crypto.h
//  flee
//
//  Created by Unit.2B on 2017/7/22.
//

#ifndef _FLEE_CRYPTO_H_
#define _FLEE_CRYPTO_H_

#include <flee/common.h>

// Chapo key/subkey bytes length
FL_EXTERN int const fl_crypto_key_bytes;

// BLAKE2b key derivation salt bytes length
FL_EXTERN int const fl_crypto_salt_bytes;

// Chapo per encryption nonce bytes length
FL_EXTERN int const fl_crypto_nonce_bytes;

// Chapo max encrypted text bytes length - plain text bytes length
FL_EXTERN int const fl_crypto_overhead_bytes;

/**
 * fl_crypto
 *
 * BLAKE2b for key derivation
 *
 * CHACHA20-POLY1305 for encryption
 *
 */
typedef struct {
  // master key
  unsigned char *key;
  // salt for subkey derivation
  unsigned char *salt;
  // subkey derived from master key
  unsigned char *subkey;
  // nonce
  unsigned char *nonce;
} fl_crypto;

#define fl_crypto_empty                                                        \
  (fl_crypto) { .key = NULL, .subkey = NULL, .salt = NULL, .nonce = NULL }

/**
 * allocate key, salt, nonce for given cipher, set masterkey with BLAKE2b
 */
fl_err fl_crypto_init(fl_crypto *crypto, char *passwd);

/**
 * dealloc key, salt, nonce for crypto
 */
void fl_crypto_deinit(fl_crypto *crypto);

/**
 * create a new salt and derive the subkey, using BLAKE2b
 */
fl_err fl_crypto_new_subkey(fl_crypto *crypto);

/**
 * resume a subkey from a given salt, using BLAKE2b
 */
fl_err fl_crypto_resume_subkey(fl_crypto *crypto, unsigned char *salt);

/**
 * increase nonce value by 1, (little-endian, auto overflowed)
 */
void fl_crypto_increase_nonce(fl_crypto *crypto);

/**
 * encrypt 'dataout' is at least 'in_len' + 'fl_crypto_overhead_bytes' length
 */
fl_err fl_crypto_encrypt(fl_crypto *crypto, unsigned char *datain,
                         unsigned long long inlen, unsigned char *dataout,
                         unsigned long long *outlen);
/**
 * decrypt 'dataout' is at least 'in_len' length
 */
fl_err fl_crypto_decrypt(fl_crypto *crypto, unsigned char *datain,
                         unsigned long long inlen, unsigned char *dataout,
                         unsigned long long *outlen);

#endif /* _FLEE_CRYPTO_H_ */
