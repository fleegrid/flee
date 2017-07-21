//
//  common.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_COMMON_H_
#define _FLEE_COMMON_H_

#include <inttypes.h>

#include <flee/error.h>

#if defined(__APPLE__) && defined(__MACH__)
/* Darwin system, including macOS, iOS */
#define _FL_DARWIN
#elif defined(__linux__)
/* Linux system */
#define _FL_LINUX
#else
#error "platform not supported"
#endif

/**
 * fl_ip is a 32-bit IPv4 address
 */
typedef uint8_t fl_ip[4];

#endif /* _FLEE_COMMON_H_ */
