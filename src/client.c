//
//  client.c
//  flee
//
//  Created by Unit.2B on 2017/7/23.
//

#define __FL_LIB_SOURCE__
#include <flee/client.h>

fl_err fl_client_init(fl_client *client, char *passwd, char *host, int port) {
  fl_err err = fl_ok;
  client->host = malloc(strlen(host) + 1);
  strcpy(client->host, host);
  client->port = port;
  client->base = NULL;
  fl_crypto_init(&client->crypto, passwd);
  require_ok(err, exit);
exit:
  return err;
}

fl_err fl_client_start(fl_client *client, struct event_base *base) {
  client->base = base;
  return fl_ok;
}

void fl_client_deinit(fl_client *client) {
  if (client->host)
    free(client->host);
}
