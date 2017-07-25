//
//  internal.h
//  flee
//
//  Created by Unit.2B on 2017/7/24.
//

#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#include <flee/common.h>

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
#define __FL_APPLE
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
  printf("%s:%d:%s(): [DEBUG] " fmt "\n", __FILENAME__, __LINE__, __func__,    \
         ##__VA_ARGS__);
#else
#define DLOG(fmt, ...)
#endif

#define LOG(fmt, ...)                                                          \
  printf("%s:%d: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__);

#define ELOG(fmt, ...)                                                         \
  fprintf(stderr, "%s:%d: [ERROR] " fmt "\n", __FILENAME__, __LINE__,          \
          ##__VA_ARGS__);

/*
 * error handling
 */

// if COND is not satisfied, print STR to stderr and exit(EXIT_FAILURE)
#define insist(COND, STR)                                                      \
  if (!(COND)) {                                                               \
    ELOG("FATAL: %s, %s", #COND, STR);                                         \
    exit(EXIT_FAILURE);                                                        \
  }

// if ERR is not fl_ok, goto LABEL
#define validate_ok(ERR, LABEL)                                                \
  if (ERR != fl_ok) {                                                          \
    ELOG("%s", fl_strerr(ERR));                                                \
    goto LABEL;                                                                \
  }

// if ERR is not fl_ok, print error and exit(EXIT_FAILURE)
#define insist_ok(ERR)                                                         \
  {                                                                            \
    fl_err _e = (ERR);                                                         \
    if (_e != fl_ok) {                                                         \
      ELOG("FATAL: %s", fl_strerr(_e));                                        \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  }

// if RET < 0, then assign ERR to fl_esyscall and goto LABEL
#define validate_syscall(RET, ERR, LABEL)                                      \
  if ((RET) < 0) {                                                             \
    ERR = fl_esyscall;                                                         \
    ELOG("%s", strerror(errno));                                               \
    goto LABEL;                                                                \
  }

// if RET < 0, then print strerror(errno) then exit(EXIT_FAILURE)
#define insist_syscall(RET)                                                    \
  {                                                                            \
    int _ret = (RET);                                                          \
    if (_ret < 0) {                                                            \
      ELOG("FATAL: %s", strerror(_ret));                                       \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  }

/*
 * Apple semaphore
 */
#ifdef __FL_APPLE
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

typedef struct {
#ifdef __APPLE__
  dispatch_semaphore_t sem;
#else
  sem_t sem;
#endif
} fl_sem_t;

static inline void fl_sem_init(fl_sem_t *s, uint32_t value) {
#ifdef __FL_APPLE
  dispatch_semaphore_t *sem = &s->sem;
  *sem = dispatch_semaphore_create(value);
#else
  sem_init(&s->sem, 0, value);
#endif
}

static inline void fl_sem_wait(fl_sem_t *s) {
#ifdef __FL_APPLE
  dispatch_semaphore_wait(s->sem, DISPATCH_TIME_FOREVER);
#else
  int r;
  do {
    r = sem_wait(&s->sem);
  } while (r == -1 && errno == EINTR);
#endif
}

static inline void fl_sem_post(fl_sem_t *s) {
#ifdef __FL_APPLE
  dispatch_semaphore_signal(s->sem);
#else
  sem_post(&s->sem);
#endif
}

#endif /* _INTERNAL_H_ */
