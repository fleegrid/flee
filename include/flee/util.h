//
//  util.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_UTIL_H_
#define _FLEE_UTIL_H_

#include <flee/error.h>

#include <stdint.h>

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

#endif /* _FLEE_UTIL_H_ */
