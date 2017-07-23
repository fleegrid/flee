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
#define fl_crypto_key_bytes 32 // crypto_aead_chacha20poly1305_ietf_KEYBYTES

// BLAKE2b key derivation salt bytes length
#define fl_crypto_salt_bytes 64 // crypto_generichash_blake2b_KEYBYTES_MAX

// Chapo per encryption nonce bytes length
#define fl_crypto_nonce_bytes 12 // crypto_aead_chacha20poly1305_ietf_NPUBBYTES

// Chapo max encrypted text bytes length - plain text bytes length
#define fl_crypto_overhead_bytes 16 // crypto_aead_chacha20poly1305_ietf_ABYTES

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
  unsigned char key[fl_crypto_key_bytes];
  // salt for subkey derivation
  unsigned char salt[fl_crypto_salt_bytes];
  // subkey derived from master key
  unsigned char subkey[fl_crypto_key_bytes];
  // nonce
  unsigned char nonce[fl_crypto_nonce_bytes];
} fl_crypto;

/**
 * load master key from passwd, using BLAKE2b
 *
 * this will also clear subkey, salt and nonce
 */
void fl_crypto_init(fl_crypto *crypto, char *passwd);

/**
 * create a new salt and derivate a subkey
 */
void fl_crypto_new_subkey(fl_crypto *crypto);

/**
 * load salt, and derivate a subkey
 */
void fl_crypto_load_subkey(fl_crypto *crypto, unsigned char *salt);

/**
 * reset nonce to full zero
 */
void fl_crypto_reset_nonce(fl_crypto *crypto);

/**
 * increase nonce value by 1, (little-endian, auto overflowed)
 */
void fl_crypto_increase_nonce(fl_crypto *crypto);

/**
 * encrypt 'dataout' is at least 'in_len' + 'fl_crypto_overhead_bytes' length
 */
void fl_crypto_encrypt(fl_crypto *crypto, unsigned char *datain,
                       unsigned long long inlen, unsigned char *dataout,
                       unsigned long long *outlen);
/**
 * decrypt 'dataout' is at least 'in_len' length
 */
bool fl_crypto_decrypt(fl_crypto *crypto, unsigned char *datain,
                       unsigned long long inlen, unsigned char *dataout,
                       unsigned long long *outlen);

#endif /* _FLEE_CRYPTO_H_ */
