//
//  flee.c
//  flee
//
//  Created by Unit.2B on 2017/7/25.
//

#include <flee.h>
#include <flee/internal.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

static int sock;

static fl_crypto rxc;
static fl_sem_t rxs;
static pthread_t rxt;
static unsigned char rxbuf[64 * 1024];

static fl_crypto txc;
static fl_sem_t txs;
static pthread_t txt;
static unsigned char txbuf[64 * 1024];

static void *rx_thread(void *ctx __unused) {
  ssize_t n;
  // RX
  while ((n = read(sock, rxbuf, sizeof rxbuf - 1)) > 0) {
    rxbuf[n] = '\0';
    DLOG("%s", rxbuf);
  }
  LOG("RX stream closed");
  fl_sem_post(&rxs);
  pthread_exit(NULL);
  return NULL;
}

static void *tx_thread(void *ctx __unused) {
  txbuf[0] = '\0';
  LOG("TX stream closed");
  fl_sem_post(&txs);
  pthread_exit(NULL);
  return NULL;
}

int main(int argc, char **argv) {
  char *server, *port, *passwd;
  // BOOT
  if (argc != 3) {
    printf("Usage: %s SERVER_ADDRESS PORT\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  server = argv[1];
  port = argv[2];
  passwd = getenv("FLEE_PASSWD");
  if (passwd == NULL || strlen(passwd) == 0) {
    printf("$FLEE_PASSWD not set\n");
    exit(EXIT_FAILURE);
  }
  LOG("flee client v%s", fl_version);

  // init rx crypto, tx crypto
  fl_crypto_init(&rxc, passwd);
  fl_crypto_init(&txc, passwd);
  fl_crypto_new_subkey(&txc);

  // DNS
  struct addrinfo hints;
  struct addrinfo *answer = NULL;
  int ret;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_ADDRCONFIG;
  ret = getaddrinfo(server, port, &hints, &answer);
  insist_syscall(ret);
  // SOCK
  sock = socket(answer->ai_family, answer->ai_socktype, answer->ai_protocol);
  insist_syscall(sock);
  // CONNECT
  ret = connect(sock, answer->ai_addr, answer->ai_addrlen);
  insist_syscall(ret);
  // RX semaphore
  fl_sem_init(&rxs, 0);
  fl_sem_init(&txs, 0);
  pthread_create(&rxt, NULL, rx_thread, NULL);
  pthread_create(&txt, NULL, tx_thread, NULL);
  // TX semaphore
  fl_sem_wait(&rxs);
  fl_sem_wait(&txs);
  LOG("flee client exiting");
  return 0;
}
