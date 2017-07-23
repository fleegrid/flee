//
//  main.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee.h>
#include <unistd.h>

struct event_base *base;

void on_signal(evutil_socket_t fd, short flags, void *args) {
  LOG("exiting: SIGNAL(%d)", fd);
  event_base_loopbreak(base);
}

int main(int argc __unused, char **argv __unused) {
  fl_init();

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
