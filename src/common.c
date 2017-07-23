//
//  common.c
//  flee
//
//  Created by Unit.2B on 2017/7/22.
//

#define __FL_LIB_SOURCE__
#include <flee/common.h>

#include <stdio.h>
#include <string.h>

#include <event2/event.h>
#include <sodium.h>

void fl_libevent_logger(int severity, const char *msg) {
  const char *s;
  switch (severity) {
  case EVENT_LOG_DEBUG:
    s = "DEBUG";
    break;
  case EVENT_LOG_MSG:
    s = "MESSG";
    break;
  case EVENT_LOG_WARN:
    s = " WARN";
    break;
  case EVENT_LOG_ERR:
    s = "ERROR";
    break;
  default:
    s = "?????";
    break; /* never reached */
  }
  printf("[%s] %s\n", s, msg);
}

fl_err fl_init() {
  // initialize sodium, pick best implementation
  if (sodium_init() != 0) {
    return err_crypto;
  }
  // set libevent log callback
  event_set_log_callback(&fl_libevent_logger);
  return err_ok;
}

fl_err fl_ip_set(fl_ip ip, char *s) {
  fl_err err = err_ok;
  int ret = 0;
  ret = sscanf(s, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
  require(ret == 4, err = err_ip_invalid, exit);
exit:
  return err;
}

uint32_t fl_ip_to_u(fl_ip ip) {
  uint32_t ret = 0;
  memcpy(&ret, ip, sizeof(ret));
  return ret;
}
