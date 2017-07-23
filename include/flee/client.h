//
//  client.h
//  flee
//
//  Created by Unit.2B on 2017/7/23.
//

#ifndef _FLEE_CLIENT_H_
#define _FLEE_CLIENT_H_

#include <flee/common.h>
#include <flee/crypto.h>

typedef struct {
  char *host;
  int port;
  fl_crypto crypto;
  struct event_base *base;
} fl_client;

/**
 * copy .address, init crypto
 */
fl_err fl_client_init(fl_client *client, char *passwd, char *host, int port);

/**
 * start the client on the specified event_base
 */
fl_err fl_client_start(fl_client *client, struct event_base *base);

/**
 * stop the client
 */
fl_err fl_client_stop(fl_client *client);

/**
 * free .address, deinit crypto
 */
void fl_client_deinit(fl_client *client);

#endif /* _FLEE_CLIENT_H_ */
