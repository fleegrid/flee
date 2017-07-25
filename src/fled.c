//
//  fled.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee.h>
#include <flee/internal.h>

#include <event2/event.h>
#include <event2/http.h>

#include <signal.h>
#include <unistd.h>

#include <arpa/inet.h>

#define SCHEME "flee://"

struct event_base *base;

fl_err extract_url(char *url, char **passwd, char **host) {
  if (strstr(url, SCHEME) != url) {
    ELOG("url should start with 'flee://': %s", url);
    return fl_eurl;
  }
  return fl_ok;
}

void start_client(char *url) {
  LOG("flee v%s client", fl_version);
  fl_err err = fl_ok;
  char *passwd, *host;
  err = extract_url(url, &passwd, &host);
  insist_ok(err);
}

void start_server(char *url) {
  LOG("flee v%s server", fl_version);
  fl_err err = fl_ok;
  char *passwd, *host;
  err = extract_url(url, &passwd, &host);
  insist_ok(err);
}

void fl_libevent_logger(int severity, const char *msg);

int main(int argc, char **argv) {
  char *url = NULL;
  enum { MODE_NONE, MODE_HELP, MODE_SERVER, MODE_CLIENT } mode = MODE_NONE;
  for (ssize_t i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 's':
        mode = MODE_SERVER;
        break;
      case 'c':
        mode = MODE_CLIENT;
        break;
      case 'h':
        mode = MODE_HELP;
      }
    } else {
      url = argv[i];
    }
  }
  if (mode == MODE_HELP || mode == MODE_NONE || url == NULL) {
    printf("Usage:\n %s -[c|s] FLEE_URL\n", argv[0]);
    if (mode != MODE_HELP) {
      exit(EXIT_FAILURE);
    }
  }

  event_set_log_callback(&fl_libevent_logger);

  insist_ok(fl_init());
#ifdef DEBUG
  event_enable_debug_mode();
#endif

  base = event_base_new();

  if (mode == MODE_CLIENT) {
    start_client(url);
  } else if (mode == MODE_SERVER) {
    start_server(url);
  }

  event_base_dispatch(base);

  return 0;
}

void fl_libevent_logger(int severity, const char *msg) {
  switch (severity) {
  case EVENT_LOG_DEBUG:
    DLOG("libevent: %s", msg);
    break;
  case EVENT_LOG_MSG:
    LOG("libevent: %s", msg);
    break;
  case EVENT_LOG_WARN:
  case EVENT_LOG_ERR:
    ELOG("libevent: %s", msg);
    break;
  default:
    break; /* never reached */
  }
}
