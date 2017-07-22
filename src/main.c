//
//  main.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc __unused, char **argv __unused) {
  fl_init();

  fl_tun tun = fl_tun_empty;
  fl_ip_set(tun.ip, "10.10.10.1");
  fl_ip_set(tun.dst_ip, "10.10.10.2");
  fl_ip_set(tun.netmask, "255.255.255.0");
  printf("uint32: %d\n", fl_ip_to_u(tun.ip));
  tun.mtu = 1500;
  int ret = fl_tun_init(&tun);
  printf("device: %s(%d) %d.%d.%d.%d -> %d.%d.%d.%d (%d.%d.%d.%d)\n", tun.name,
         tun.fd, tun.ip[0], tun.ip[1], tun.ip[2], tun.ip[3], tun.dst_ip[0],
         tun.dst_ip[1], tun.dst_ip[2], tun.dst_ip[3], tun.netmask[0],
         tun.netmask[1], tun.netmask[2], tun.netmask[3]);
  printf("result: %d\n", ret);
  fl_tun_deinit(&tun);

  fl_crypto crypto = fl_crypto_empty;
  fl_crypto_init(&crypto, "hello");
  fl_crypto_new_subkey(&crypto);
  for (int i = 0; i < fl_crypto_key_bytes; i++) {
    printf("%02x", crypto.key[i]);
  }
  printf("\n");
  for (int i = 0; i < fl_crypto_salt_bytes; i++) {
    printf("%02x", crypto.salt[i]);
  }
  printf("\n");
  for (int i = 0; i < fl_crypto_key_bytes; i++) {
    printf("%02x", crypto.subkey[i]);
  }
  printf("\n");
  char *plain = "hello, world";
  unsigned char cipher[strlen(plain) + fl_crypto_overhead_bytes];
  unsigned long long cl = 0;
  fl_crypto_encrypt(&crypto, (unsigned char *)plain, strlen(plain), cipher,
                    &cl);
  printf("Crlen: %llu", cl);
  printf("\n");
  for (int i = 0; i < (int)cl; i++) {
    printf("%02x", cipher[i]);
  }
  printf("\n");
  unsigned char p[strlen(plain)];
  unsigned long long pl = 0;
  fl_crypto_decrypt(&crypto, cipher, cl, p, &pl);
  for (int i = 0; i < (int)pl; i++) {
    printf("%c", p[i]);
  }
  printf("\n");
  fl_crypto_deinit(&crypto);

  return 0;
}
