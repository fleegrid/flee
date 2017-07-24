//
//  common.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_COMMON_H_
#define _FLEE_COMMON_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/event.h>

/*
 * FL_EXTERN
 */
#ifdef __FL_LIB_SOURCE__
#define FL_EXTERN
#else
#define FL_EXTERN extern
#endif

/*
 * error
 */

/**
 * fl_err is int
 */
typedef enum {
  // no error
  fl_ok = 0,
  // required function is not implemented in current platform
  fl_eplatform = 1,
  // syscall err, check errno for details
  fl_esyscall = 2,
  // ip invalid
  fl_eip = 3,
  // failed to decrypt
  fl_ecrypto = 4,
} fl_err;

/**
 * internal initialization
 */
fl_err fl_init();

#endif /* _FLEE_COMMON_H_ */
