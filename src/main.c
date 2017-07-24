//
//  main.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee.h>
#include <flee/internal.h>

#include <signal.h>
#include <unistd.h>

#include <arpa/inet.h>

struct event_base *base;

fl_tun tun = fl_tun_empty;

void on_signal(evutil_socket_t fd, short flags, void *args) {
  LOG("exiting: SIGNAL(%d)", fd);
  fl_tun_deinit(&tun);
  event_base_loopbreak(base);
}

int main(int argc __unused, char **argv __unused) {
  fl_err err = fl_ok;
  fl_init();

  char *welcome = "flee v1.0";
  unsigned char encrypted[strlen(welcome) + 1 + fl_crypto_overhead_bytes];
  unsigned long long encrypted_len;
  unsigned char salt[fl_crypto_salt_bytes];
  char decrypted[strlen(welcome) + 1];

  // encryption
  fl_crypto c;
  fl_crypto_init(&c, "hello");
  fl_crypto_new_subkey(&c);
  fl_crypto_encrypt(&c, (unsigned char *)welcome, strlen(welcome) + 1,
                    encrypted, &encrypted_len);

  // save salt
  memcpy(salt, c.salt, sizeof salt);

  // decryption
  fl_crypto d;
  fl_crypto_init(&d, "hello");
  fl_crypto_load_subkey(&d, salt);
  fl_crypto_decrypt(&d, encrypted, encrypted_len, (unsigned char *)decrypted,
                    NULL);

  LOG("Welcome: %s", decrypted);

  tun.ip.s_addr = inet_addr("10.10.10.2");
  tun.dst_ip.s_addr = inet_addr("10.10.10.1");
  tun.netmask.s_addr = inet_addr("255.255.255.255");
  tun.mtu = 1500;
  err = fl_tun_init(&tun);

  LOG("TUN: %s %d", tun.name, err);
  if (err != fl_ok) {
    LOG("ERR: %s", strerror(errno));
  }

  base = event_base_new();

  struct event *ev = evsignal_new(base, SIGINT, on_signal, NULL);
  event_add(ev, NULL);
  event_base_loop(base, EVLOOP_NO_EXIT_ON_EMPTY);
  event_base_free(base);
  return 0;
}
