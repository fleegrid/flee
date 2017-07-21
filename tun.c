//
//  tun.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>

#ifdef _FL_DARWIN
#include <net/if_utun.h>
#include <sys/kern_control.h>
#include <sys/kern_event.h>
#endif

fl_err fl_tun_init(fl_tun *tun) {
  fl_err err = err_ok;
  int ret;
#ifdef _FL_DARWIN
  struct ctl_info info;
  struct sockaddr_ctl ctl;
  unsigned int len;

  // create system control socket
  tun->fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
  check_syscall(tun->fd, err);
  require_ok(err, exit);
  // get ctl_id
  strcpy(info.ctl_name, UTUN_CONTROL_NAME);
  ret = ioctl(tun->fd, CTLIOCGINFO, &info);
  check_syscall(ret, err);
  require_ok(err, exit);
  // connect kernel
  ctl = (struct sockaddr_ctl){.sc_len = sizeof(struct sockaddr_ctl),
                              .sc_family = AF_SYSTEM,
                              .ss_sysaddr = AF_SYS_CONTROL,
                              .sc_id = info.ctl_id,
                              .sc_unit = 0,
                              .sc_reserved = {0, 0, 0, 0, 0}};
  ret = connect(tun->fd, (struct sockaddr *)&ctl, sizeof(struct sockaddr_ctl));
  check_syscall(ret, err);
  require_ok(err, exit);
  // get name
  len = sizeof(tun->name);
  ret = getsockopt(tun->fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, tun->name, &len);
  check_syscall(ret, err);
  require_ok(err, exit);
#endif
exit:
  return err;
}

fl_err fl_tun_deinit(fl_tun *tun) {
  fl_err err = err_ok;
  int ret;
  if (tun->fd > 0) {
    ret = close(tun->fd);
    tun->fd = -1;
    check_syscall(ret, err);
    require_ok(err, exit);
  }
exit:
  return err;
}
