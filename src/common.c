//
//  common.c
//  flee
//
//  Created by Unit.2B on 2017/7/22.
//

#include <flee/common.h>

#include <sodium.h>

fl_err fl_init() {
  if (sodium_init() != 0) {
    return err_crypto;
  }
  return err_ok;
}
