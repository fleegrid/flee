//
//  common.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_COMMON_H_
#define _FLEE_COMMON_H_

#include <flee/error.h>
#include <flee/util.h>
#include <sys/types.h>

/*
 * unused parameter
 */
#define _unused __attribute__((unused))

/*
 * platform determination
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
 * extern
 */
#ifdef __FL_LIB_SOURCE__
#define FL_EXTERN
#else
#define FL_EXTERN extern
#endif

#endif /* _FLEE_COMMON_H_ */
