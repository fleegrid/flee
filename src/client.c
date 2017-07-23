//
//  client.c
//  flee
//
//  Created by Unit.2B on 2017/7/23.
//

#define __FL_LIB_SOURCE__
#include <flee/client.h>

fl_err fl_client_init(fl_client *client, char *passwd, char *host, int port) {
  fl_err err = err_ok;
  err = fl_crypto_init(&client->crypto, passwd);
  require_ok(err, exit);
  client->host = malloc(strlen(host) + 1);
  strcpy(client->host, host);
exit:
  return err;
}

fl_err fl_client_start(fl_client *client, struct event_base *base) {
  client->base = base;
  return err_ok;
}

void fl_client_deinit(fl_client *client) {
  fl_crypto_deinit(&client->crypto);
  if (client->host)
    free(client->host);
}
