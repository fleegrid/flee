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

#include <uv.h>

uv_loop_t *loop;
uv_tcp_t server;

void alloc_buffer(uv_handle_t *handle __unused, size_t size, uv_buf_t *buf) {
  buf->base = malloc(size);
  memset(buf->base, 0, size);
  buf->len = size;
}

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
  uv_write_t *req = (uv_write_t *)malloc(sizeof(uv_write_t));

  if (nread == -1) {
    /* if (uv_last_error(loop).code != UV_EOF) { */
    /* } */

    uv_close((uv_handle_t *)stream, NULL);
  }

  int r = uv_write(req, stream, buf, 1, NULL);

  if (r) {
    /* error */
  }

  free(buf->base);
}

void on_connection(uv_stream_t *server, int status) {
  uv_tcp_t *client = malloc(sizeof(uv_tcp_t));

  if (status == -1) {
    /* error */
  }

  uv_tcp_init(loop, client);

  if (uv_accept(server, (uv_stream_t *)client) == 0) {
    int r = uv_read_start((uv_stream_t *)client, alloc_buffer, on_read);

    if (r) {
      /* error */
    }
  } else {
    uv_close((uv_handle_t *)client, NULL);
  }
}

int main(int argc __unused, char **argv __unused) {
  loop = uv_default_loop();

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

  struct sockaddr_in addr;
  uv_ip4_addr("0.0.0.0", 3000, &addr);

  uv_tcp_init(loop, &server);
  uv_tcp_bind(&server, (struct sockaddr *)&addr, 0);

  int r = uv_listen((uv_stream_t *)&server, 128, on_connection);

  if (r) {
    exit(0);
  }

  return uv_run(loop, UV_RUN_DEFAULT);
}
