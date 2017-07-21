//
//  main.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee.h>
#include <stdio.h>

int main(int argc, char **argv) {
  fl_tun tun = fl_tun_empty;
  fl_tun_init(&tun);
  printf("hello %s(%d)\n", tun.name, tun.fd);
  fl_tun_deinit(&tun);
  return 0;
}
