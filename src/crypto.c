//
//  crypto.c
//  flee
//
//  Created by Unit.2B on 2017/7/22.
//

#define __FL_LIB_SOURCE__
#include <flee/crypto.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sodium.h>

int const fl_crypto_key_bytes = crypto_aead_chacha20poly1305_ietf_KEYBYTES;

int const fl_crypto_salt_bytes = crypto_generichash_blake2b_KEYBYTES_MAX;

int const fl_crypto_nonce_bytes = 16;

int const fl_crypto_overhead_bytes = crypto_aead_chacha20poly1305_ietf_ABYTES;

void fl_crypto_init(fl_crypto *crypto) {
  crypto->key = malloc(fl_crypto_key_bytes);
  crypto->salt = malloc(fl_crypto_salt_bytes);
  crypto->subkey = malloc(fl_crypto_key_bytes);
  crypto->nonce = malloc(fl_crypto_nonce_bytes);
}

fl_err fl_crypto_set_key(fl_crypto *crypto, char *passwd) {
  if (crypto_generichash_blake2b_BYTES_MAX < fl_crypto_key_bytes) {
    printf("FATAL: BLAKE2b could not provide AEAD key");
    exit(1);
  }
  fl_err err = err_ok;
  int ret;
  unsigned char hash[crypto_generichash_blake2b_BYTES_MAX];
  ret = crypto_generichash_blake2b(hash, sizeof hash, (unsigned char *)passwd,
                                   strlen(passwd), NULL, 0);
  require_crypto(ret, err, exit);
  memcpy(crypto->key, hash, fl_crypto_key_bytes);
exit:
  return err;
}

fl_err fl_crypto_calculate_subkey(fl_crypto *crypto) {
  fl_err err = err_ok;
  int ret;
  // digest master key with salt
  unsigned char hash[crypto_generichash_blake2b_BYTES_MAX];
  ret = crypto_generichash_blake2b(hash, sizeof hash, crypto->key,
                                   fl_crypto_key_bytes, crypto->salt,
                                   fl_crypto_salt_bytes);
  require_crypto(ret, err, exit);
exit:
  return err;
}

fl_err fl_crypto_new_subkey(fl_crypto *crypto) {
  // new salt
  randombytes_buf(crypto->salt, fl_crypto_salt_bytes);
  return fl_crypto_calculate_subkey(crypto);
}

fl_err fl_crypto_resume_subkey(fl_crypto *crypto, unsigned char *salt) {
  // copy salt
  memcpy(crypto->salt, salt, fl_crypto_salt_bytes);
  return fl_crypto_calculate_subkey(crypto);
}

void fl_crypto_deinit(fl_crypto *crypto) {
  if (crypto->key != NULL)
    free(crypto->key);
  if (crypto->subkey != NULL)
    free(crypto->subkey);
  if (crypto->salt != NULL)
    free(crypto->salt);
  if (crypto->nonce != NULL)
    free(crypto->nonce);
  *crypto = fl_crypto_empty;
}
