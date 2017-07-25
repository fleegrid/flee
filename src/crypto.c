//
//  crypto.c
//  flee
//
//  Created by Unit.2B on 2017/7/22.
//

#define __FL_LIB_SOURCE__
#include <flee/crypto.h>
#include <flee/internal.h>

#include <sodium.h>

void fl_crypto_init(fl_crypto *crypto, char *passwd) {
  // PASSWD (VARLEN) - BLAKE2b (0) -> MASTER_KEY (32)
  int ret = crypto_generichash_blake2b(crypto->key, sizeof crypto->key,
                                       (unsigned char *)passwd, strlen(passwd),
                                       NULL, 0);
  insist(ret == 0, "failed to load key");
  fl_crypto_reset(crypto);
}

void fl_crypto_reset(fl_crypto *crypto) {
  memset(crypto->salt, 0, sizeof crypto->salt);
  memset(crypto->subkey, 0, sizeof crypto->subkey);
  memset(crypto->nonce, 0, sizeof crypto->nonce);
}

static void fl_crypto_derivate_subkey(fl_crypto *crypto) {
  // digest master key with salt
  // MASTER_KEY (32) - BLAKE2b (64) -> SUB_KEY (32)
  int ret = crypto_generichash_blake2b(crypto->subkey, sizeof crypto->subkey,
                                       crypto->key, sizeof crypto->key,
                                       crypto->salt, sizeof crypto->salt);
  insist(ret == 0, "failed to derivate subkey");
}

void fl_crypto_new_subkey(fl_crypto *crypto) {
  randombytes_buf(crypto->salt, sizeof crypto->salt);
  fl_crypto_derivate_subkey(crypto);
}

void fl_crypto_load_subkey(fl_crypto *crypto, unsigned char *salt) {
  memcpy(crypto->salt, salt, sizeof crypto->salt);
  fl_crypto_derivate_subkey(crypto);
}

void fl_crypto_reset_nonce(fl_crypto *crypto) {
  memset(crypto->nonce, 0, sizeof crypto->nonce);
}

void fl_crypto_increase_nonce(fl_crypto *crypto) {
  for (size_t i = 0; i < sizeof crypto->nonce; i++) {
    crypto->nonce[i]++;
    if (crypto->nonce[i] != 0) {
      return;
    }
  }
}

void fl_crypto_encrypt(fl_crypto *crypto, unsigned char *datain,
                       unsigned long long inlen, unsigned char *dataout,
                       unsigned long long *outlen) {
  int ret = crypto_aead_chacha20poly1305_ietf_encrypt(
      dataout, outlen, datain, inlen, NULL, 0, NULL, crypto->nonce,
      crypto->subkey);
  insist(ret == 0, "failed to encrypt");
}

bool fl_crypto_decrypt(fl_crypto *crypto, unsigned char *datain,
                       unsigned long long inlen, unsigned char *dataout,
                       unsigned long long *outlen) {
  int ret = crypto_aead_chacha20poly1305_ietf_decrypt(
      dataout, outlen, NULL, datain, inlen, NULL, 0, crypto->nonce,
      crypto->subkey);
  return ret == 0;
}
