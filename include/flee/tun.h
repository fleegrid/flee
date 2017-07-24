//
//  tun.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_TUN_H_
#define _FLEE_TUN_H_

#include <flee/common.h>

#include <net/if.h>

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

typedef struct {
  /*
   * input
   */
  // local ip
  struct in_addr ip;
  // point to point destination ip
  struct in_addr dst_ip;
  // netmask
  struct in_addr netmask;
  // mtu
  int mtu;
  /*
   * output
   */
  // name of device
  char name[IF_NAMESIZE];
  // underlaying file descriptor
  int fd;
} fl_tun;

#define fl_tun_empty                                                           \
  (fl_tun) { .fd = -1 }

/**
 * initialize a fl_tun structure, setup IP and dstIP, returns name and fd
 * correctly
 */
fl_err fl_tun_init(fl_tun *tun);

/**
 * close the underlaying fd
 */
fl_err fl_tun_deinit(fl_tun *tun);

#endif /* _FLEE_TUN_H_ */
