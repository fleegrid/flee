//
//  tun.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#define __FL_LIB_SOURCE__
#include <flee/tun.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>

#ifdef _FL_DARWIN
#include <net/if_utun.h>
#include <sys/kern_control.h>
#include <sys/kern_event.h>
#include <sys/sysctl.h>
#endif

fl_err fl_ip_set(fl_ip ip, char *s) {
  fl_err err = fl_ok;
  int ret = 0;
  ret = sscanf(s, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
  require(ret == 4, err = fl_eip, exit);
exit:
  return err;
}

uint32_t fl_ip_to_u(fl_ip ip) {
  uint32_t ret = 0;
  memcpy(&ret, ip, sizeof(ret));
  return ret;
}

fl_err fl_tun_init(fl_tun *tun) {
  fl_err err = fl_ok;
  int ret;
#ifdef _FL_DARWIN
  printf("WARN: Use NetworkExtension for TUN implementation on macOS/iOS\n");

  struct ctl_info info;
  struct sockaddr_ctl ctl;
  unsigned int len;

  // create system control socket
  tun->fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
  require_syscall(tun->fd, err, exit);
  // get ctl_id
  strcpy(info.ctl_name, UTUN_CONTROL_NAME);
  ret = ioctl(tun->fd, CTLIOCGINFO, &info);
  require_syscall(ret, err, exit);
  // connect kernel
  ctl = (struct sockaddr_ctl){.sc_len = sizeof(struct sockaddr_ctl),
                              .sc_family = AF_SYSTEM,
                              .ss_sysaddr = AF_SYS_CONTROL,
                              .sc_id = info.ctl_id,
                              .sc_unit = 0,
                              .sc_reserved = {0, 0, 0, 0, 0}};
  ret = connect(tun->fd, (struct sockaddr *)&ctl, sizeof(struct sockaddr_ctl));
  require_syscall(tun->fd, err, exit);
  // get name
  len = sizeof(tun->name);
  ret = getsockopt(tun->fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, tun->name, &len);
  require_syscall(ret, err, exit);
  // set ip address
  int fd;
  struct ifreq req;
  struct sockaddr_in *addr;
  // connect NET kernel
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  require_syscall(ret, err, exit);
  // prepare iface name
  strncpy(req.ifr_name, tun->name, IF_NAMESIZE);
  // set if ADDR
  addr = (struct sockaddr_in *)&req.ifr_addr;
  addr->sin_family = AF_INET;
  addr->sin_port = 0;
  addr->sin_addr.s_addr = fl_ip_to_u(tun->ip);
  ret = ioctl(fd, SIOCSIFADDR, &req);
  require_syscall(ret, err, exit);
  // set if DSTADDR
  addr = (struct sockaddr_in *)&req.ifr_dstaddr;
  addr->sin_family = AF_INET;
  addr->sin_port = 0;
  addr->sin_addr.s_addr = fl_ip_to_u(tun->dst_ip);
  ret = ioctl(fd, SIOCSIFDSTADDR, &req);
  require_syscall(ret, err, exit);
  // set netmask
  addr = (struct sockaddr_in *)&req.ifr_addr;
  addr->sin_family = AF_INET;
  addr->sin_port = 0;
  addr->sin_addr.s_addr = fl_ip_to_u(tun->netmask);
  ret = ioctl(fd, SIOCSIFNETMASK, &req);
  require_syscall(ret, err, exit);
  // set MTU
  req.ifr_mtu = tun->mtu;
  ret = ioctl(fd, SIOCSIFMTU, &req);
  require_syscall(ret, err, exit);
  // get flags
  ret = ioctl(fd, SIOCGIFFLAGS, &req);
  require_syscall(ret, err, exit);
  // set UP flag
  req.ifr_flags |= IFF_UP;
  ret = ioctl(fd, SIOCSIFFLAGS, &req);
  require_syscall(ret, err, exit);
#endif
exit:
  return err;
}

fl_err fl_tun_deinit(fl_tun *tun) {
  fl_err err = fl_ok;
  int ret;
  if (tun->fd > 0) {
    ret = close(tun->fd);
    tun->fd = -1;
    require_syscall(ret, err, exit);
  }
exit:
  return err;
}
