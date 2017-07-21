//
//  main.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc _unused, char **argv _unused) {
  fl_tun tun = fl_tun_empty;
  fl_ip_set(tun.ip, "10.10.10.1");
  fl_ip_set(tun.dst_ip, "10.10.10.2");
  fl_ip_set(tun.netmask, "255.255.255.0");
  printf("uint32: %d\n", fl_ip_to_u(tun.ip));
  tun.mtu = 1500;
  int ret = fl_tun_init(&tun);
  printf("device: %s(%d) %d.%d.%d.%d -> %d.%d.%d.%d (%d.%d.%d.%d)\n", tun.name,
         tun.fd, tun.ip[0], tun.ip[1], tun.ip[2], tun.ip[3], tun.dst_ip[0],
         tun.dst_ip[1], tun.dst_ip[2], tun.dst_ip[3], tun.netmask[0],
         tun.netmask[1], tun.netmask[2], tun.netmask[3]);
  printf("result: %d\n", ret);
  sleep(100);
  fl_tun_deinit(&tun);
  return 0;
}
