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

/**
 * __unsued macro
 */
#ifndef __unused
#define __unused __attribute__((unused))
#endif

/*
 * platform detection
 */
#if defined(__APPLE__) && defined(__MACH__)
/* Darwin system, including macOS, iOS */
#define _FL_DARWIN
#elif defined(__linux__)
/* Linux system */
#define _FL_LINUX
#else
#error "platform not supported"
#endif

/*
 * FL_EXTERN
 */
#ifdef __FL_LIB_SOURCE__
#define FL_EXTERN
#else
#define FL_EXTERN extern
#endif

/*
 * logging
 */
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifdef DEBUG
#define DLOG(fmt, ...)                                                         \
  printf("[DEBUG] %s:%d:%s(): " fmt "\n", __FILENAME__, __LINE__, __func__,    \
         ##__VA_ARGS__);
#else
#define DLOG(fmt, ...)
#endif

#define LOG(fmt, ...)                                                          \
  printf("%s:%d:%s(): " fmt "\n", __FILENAME__, __LINE__, __func__,            \
         ##__VA_ARGS__);

#define ELOG(fmt, ...)                                                         \
  fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILENAME__, __LINE__, __func__,   \
          ##__VA_ARGS__);

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

// if ERR is not fl_ok, goto LABEL
#define require_ok(ERR, LABEL)                                                 \
  if (ERR != fl_ok) {                                                          \
    goto LABEL;                                                                \
  }

// if COND is not satisfied, then ACT and goto LABEL
#define require(COND, ACT, LABEL)                                              \
  if (!(COND)) {                                                               \
    ACT;                                                                       \
    goto LABEL;                                                                \
  }

// if COND is not satisfied, print STR to stderr and abort()
#define require_fatal(COND, STR)                                               \
  if (!(COND)) {                                                               \
    ELOG("fatal error: %s not match, %s", #COND, STR);                         \
    abort();                                                                   \
  }

// if RET is < 0, then assign ERR to fl_esyscall and goto LABEL
#define require_syscall(RET, ERR, LABEL)                                       \
  require(RET >= 0, ERR = fl_esyscall, LABEL)

/**
 * internal initialization
 */
fl_err fl_init();

#endif /* _FLEE_COMMON_H_ */
