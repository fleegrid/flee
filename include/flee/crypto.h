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
 * allocate key, salt, nonce for given cipher
 */
void fl_crypto_init(fl_crypto *crypto);

/**
 * dealloc key, salt, nonce for crypto
 */
void fl_crypto_deinit(fl_crypto *crypto);

/**
 * set crypto key from a plain password, using BLAKE2b
 */
fl_err fl_crypto_set_key(fl_crypto *crypto, char *passwd);

/**
 * create a new salt and derive the subkey, using BLAKE2b
 */
fl_err fl_crypto_new_subkey(fl_crypto *crypto);

/*
 * resume a subkey from a given salt, using BLAKE2b
 */
fl_err fl_crypto_resume_subkey(fl_crypto *crypto, unsigned char *salt);

#endif /* _FLEE_CRYPTO_H_ */
