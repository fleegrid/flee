//
//  common.c
//  flee
//
//  Created by Unit.2B on 2017/7/22.
//

#define __FL_LIB_SOURCE__
#include <flee/common.h>
#include <flee/internal.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sodium.h>

fl_err fl_init() {
  // initialize sodium, pick best implementation
  if (sodium_init() != 0) {
    return fl_esodium;
  }

  return fl_ok;
}

char *fl_strerr(fl_err err) {
  switch (err) {
  case fl_ok:
    return "ok";
  case fl_eplatform:
    return "platform not supported";
  case fl_esodium:
    return "sodium failed to initialize";
  case fl_esyscall:
    return strerror(errno);
  case fl_edecrypt:
    return "failed to decrypt";
  case fl_ecommand:
    return "bad command";
  default:
    return "unknown";
  }
}
