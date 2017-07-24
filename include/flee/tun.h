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
  { .fd = -1 }

/**
 * initialize a fl_tun structure, setup IP and dstIP, returns name and fd
 * correctly
 */
fl_err fl_tun_init(fl_tun *tun);

/**
 * close the underlaying fd
 */
void fl_tun_deinit(fl_tun *tun);

#endif /* _FLEE_TUN_H_ */
