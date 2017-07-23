//
//  common.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_COMMON_H_
#define _FLEE_COMMON_H_

#include <flee/error.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/event.h>
#include <sodium.h>

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

/**
 * internal initialization
 */
fl_err fl_init();

/**
 * fl_ip is a 32-bit IPv4 address
 */
typedef unsigned char fl_ip[4];

/**
 * parse a A.B.C.D IP string
 */
fl_err fl_ip_set(fl_ip ip, char *s);

/*
 * convert IP to unsigned int 32
 */
uint32_t fl_ip_to_u(fl_ip ip);

#endif /* _FLEE_COMMON_H_ */
