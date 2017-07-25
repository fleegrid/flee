//
//  tun.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#define __FL_LIB_SOURCE__
#include <flee/internal.h>
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

#ifdef __FL_APPLE
#include <net/if_utun.h>
#include <sys/kern_control.h>
#include <sys/kern_event.h>
#endif

#ifdef __FL_LINUX
#include <fcntl.h>
#include <linux/if_tun.h>
#endif

fl_err fl_tun_init(fl_tun *tun) {
  fl_err err = fl_ok;
  int fd, fd1, ret;
  struct ifreq req;
  struct sockaddr_in *addr;
  // fix MTU
  if (tun->mtu <= 0)
    tun->mtu = 1500;
  // set all fd to -1
  fd = fd1 = ret = -1;
#ifdef __FL_APPLE
  LOG("WARN: Use NetworkExtension for TUN implementation on macOS/iOS");
  struct ctl_info info;
  struct sockaddr_ctl ctl;
  unsigned int len;

  // create system control fd
  fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
  validate_syscall(fd, err, error);
  // get ctl_id
  strcpy(info.ctl_name, UTUN_CONTROL_NAME);
  ret = ioctl(fd, CTLIOCGINFO, &info);
  validate_syscall(ret, err, error);
  // connect utun service, fd becomes available
  ctl = (struct sockaddr_ctl){.sc_len = sizeof(struct sockaddr_ctl),
                              .sc_family = AF_SYSTEM,
                              .ss_sysaddr = AF_SYS_CONTROL,
                              .sc_id = info.ctl_id,
                              .sc_unit = 0,
                              .sc_reserved = {0, 0, 0, 0, 0}};
  ret = connect(fd, (struct sockaddr *)&ctl, sizeof(struct sockaddr_ctl));
  validate_syscall(fd, err, error);
  // get device name
  len = sizeof(tun->name);
  ret = getsockopt(fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, tun->name, &len);
  validate_syscall(ret, err, error);
#endif
#ifdef __FL_LINUX
  // open tun device
  fd = open("/dev/net/tun", O_RDWR);
  validate_syscall(fd, err, error);
  // set tun IFF
  req.ifr_ifru.ifru_flags = IFF_TUN;
  strcpy(req.ifr_ifrn.ifrn_name, "");
  ret = ioctl(fd, TUNSETIFF, &req);
  validate_syscall(ret, err, error);
  // get device name
  strncpy(tun->name, req.ifr_ifrn.ifrn_name, sizeof tun->name);
  // set device persist
  ret = ioctl(fd, TUNSETPERSIST, 0);
  validate_syscall(ret, err, error);
#endif
  // connect net kernel
  fd1 = socket(AF_INET, SOCK_DGRAM, 0);
  validate_syscall(fd1, err, error);
  // set iface name
  strncpy(req.ifr_name, tun->name, IF_NAMESIZE);
  // set iface ADDR
  addr = (struct sockaddr_in *)&req.ifr_addr;
  addr->sin_family = AF_INET;
  addr->sin_port = 0;
  addr->sin_addr = tun->ip;
  ret = ioctl(fd1, SIOCSIFADDR, &req);
  validate_syscall(ret, err, error);
  // set iface DSTADDR
  addr = (struct sockaddr_in *)&req.ifr_dstaddr;
  addr->sin_family = AF_INET;
  addr->sin_port = 0;
  addr->sin_addr = tun->dst_ip;
  ret = ioctl(fd1, SIOCSIFDSTADDR, &req);
  validate_syscall(ret, err, error);
  // set iface NETMASK
  addr = (struct sockaddr_in *)&req.ifr_addr;
  addr->sin_family = AF_INET;
  addr->sin_port = 0;
  addr->sin_addr = tun->netmask;
  ret = ioctl(fd1, SIOCSIFNETMASK, &req);
  validate_syscall(ret, err, error);
  // set iface MTU
  req.ifr_mtu = tun->mtu;
  ret = ioctl(fd1, SIOCSIFMTU, &req);
  validate_syscall(ret, err, error);
  // get iface FLAGS
  ret = ioctl(fd1, SIOCGIFFLAGS, &req);
  validate_syscall(ret, err, error);
  // set UP flag
  req.ifr_flags |= (IFF_UP | IFF_RUNNING);
  ret = ioctl(fd1, SIOCSIFFLAGS, &req);
  validate_syscall(ret, err, error);
  // set fd
  tun->file = fdopen(fd, "rw");
  if (tun->file == NULL) {
    err = fl_esyscall;
    goto error;
  }
  goto exit;
error:
  // close fd
  if (fd > 0)
    close(fd);
  // reset file to NULL
  tun->file = NULL;
  // clear name
  memset(tun->name, 0, sizeof tun->name);
exit:
  // close fd1
  if (fd1 > 0)
    close(fd1);
  return err;
}

void fl_tun_deinit(fl_tun *tun) {
  if (tun->file != NULL) {
    fclose(tun->file);
  }
  tun->file = NULL;
}
