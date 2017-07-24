//
//  internal.h
//  flee
//
//  Created by Unit.2B on 2017/7/24.
//

#ifndef _INTERNAL_H_
#define _INTERNAL_H_

/*
 * __unsued fallback
 */
#ifndef __unused
#define __unused __attribute__((unused))
#endif

/*
 * platform detection
 */
#if defined(__APPLE__) && defined(__MACH__)
#define __FL_DARWIN
#elif defined(__linux__)
#define __FL_LINUX
#else
#error "platform not supported"
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
 * error handling
 */
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

#endif /* _INTERNAL_H_ */
