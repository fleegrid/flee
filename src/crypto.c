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

const int fl_crypto_key_bytes = crypto_aead_chacha20poly1305_ietf_KEYBYTES;

const int fl_crypto_salt_bytes = crypto_generichash_blake2b_KEYBYTES_MAX;

const int fl_crypto_nonce_bytes = crypto_aead_chacha20poly1305_ietf_NPUBBYTES;

const int fl_crypto_overhead_bytes = crypto_aead_chacha20poly1305_ietf_ABYTES;

fl_err fl_crypto_set_key(fl_crypto *crypto, char *passwd) {
  fl_err err = err_ok;
  int ret;
  unsigned char hash[fl_crypto_key_bytes];
  // PASSWD (VARLEN) - BLAKE2b (0) -> MASTER_KEY (32)
  ret = crypto_generichash_blake2b(hash, sizeof hash, (unsigned char *)passwd,
                                   strlen(passwd), NULL, 0);
  require_crypto(ret, err, exit);
  memcpy(crypto->key, hash, fl_crypto_key_bytes);
exit:
  return err;
}

fl_err fl_crypto_init(fl_crypto *crypto, char *passwd) {
  if (crypto->key == NULL)
    crypto->key = malloc(fl_crypto_key_bytes);
  if (crypto->salt == NULL)
    crypto->salt = malloc(fl_crypto_salt_bytes);
  if (crypto->subkey == NULL)
    crypto->subkey = malloc(fl_crypto_key_bytes);
  if (crypto->nonce == NULL)
    crypto->nonce = malloc(fl_crypto_nonce_bytes);
  memset(crypto->key, 0, fl_crypto_key_bytes);
  memset(crypto->subkey, 0, fl_crypto_key_bytes);
  memset(crypto->salt, 0, fl_crypto_salt_bytes);
  memset(crypto->nonce, 0, fl_crypto_nonce_bytes);
  return fl_crypto_set_key(crypto, passwd);
}

fl_err fl_crypto_calculate_subkey(fl_crypto *crypto) {
  fl_err err = err_ok;
  int ret;
  // digest master key with salt
  unsigned char hash[fl_crypto_key_bytes];
  // MASTER_KEY (32) - BLAKE2b (64) -> SUB_KEY (32)
  ret = crypto_generichash_blake2b(hash, sizeof hash, crypto->key,
                                   fl_crypto_key_bytes, crypto->salt,
                                   fl_crypto_salt_bytes);
  require_crypto(ret, err, exit);
  memcpy(crypto->subkey, hash, fl_crypto_key_bytes);
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

void fl_crypto_increase_nonce(fl_crypto *crypto) {
  for (int i = 0; i < fl_crypto_nonce_bytes; i++) {
    crypto->nonce[i]++;
    if (crypto->nonce[i] != 0) {
      return;
    }
  }
}

fl_err fl_crypto_encrypt(fl_crypto *crypto, unsigned char *datain,
                         unsigned char inlen, unsigned char *dataout,
                         unsigned long long *outlen) {
  int ret;
  fl_err err = err_ok;
  ret = crypto_aead_chacha20poly1305_ietf_encrypt(
      dataout, outlen, datain, inlen, NULL, 0, NULL, crypto->nonce,
      crypto->subkey);
  require_crypto(ret, err, exit);
exit:
  return err;
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
