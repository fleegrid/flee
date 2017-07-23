//
//  main.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee.h>
#include <sys/signal.h>
#include <unistd.h>

struct event_base *base;

void on_signal(evutil_socket_t fd, short flags, void *args) {
  LOG("exiting: SIGNAL(%d)", fd);
  event_base_loopbreak(base);
}

int main(int argc __unused, char **argv __unused) {
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

  fl_client client;
  fl_client_init(&client, "hello", "yorha.army", 9777);

  DLOG("Address: %s", client.host);

  base = event_base_new();

  struct event *ev = evsignal_new(base, SIGINT, on_signal, NULL);
  event_add(ev, NULL);
  event_base_loop(base, EVLOOP_NO_EXIT_ON_EMPTY);
  event_base_free(base);
  return 0;
}
