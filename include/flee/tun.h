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
#include <sys/socket.h>

typedef struct {
  // local ip
  fl_ip ip;
  // point to point destination ip
  fl_ip dst_ip;
  // netmask
  fl_ip netmask;
  // mtu
  int mtu;
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
