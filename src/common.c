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
    return fl_ecrypto;
  }
  // set libevent log callback
  event_set_log_callback(&fl_libevent_logger);
  return fl_ok;
}
