//
//  common.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_COMMON_H_
#define _FLEE_COMMON_H_

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  // validated function is not implemented in current platform
  fl_eplatform = 1,
  // syscall err, check errno for details
  fl_esyscall = 2,
  // sodium failed to initialize
  fl_esodium = 3,
  // failed to decrypt
  fl_edecrypt = 4,
  // bad url
  fl_eurl = 5,
} fl_err;

#define fl_version "1.0"

/**
 * internal initialization
 */
fl_err fl_init();

/**
 * fl_err to string
 */
char *fl_strerr(fl_err err);

#endif /* _FLEE_COMMON_H_ */
