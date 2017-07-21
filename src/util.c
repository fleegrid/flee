//
//  util.c
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#include <flee/util.h>

#include <stdio.h>
#include <string.h>

fl_err fl_ip_set(fl_ip ip, char *s) {
  fl_err err = err_ok;
  int ret = 0;
  ret = sscanf(s, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
  require(ret == 4, err = err_ip_invalid, exit);
exit:
  return err;
}

uint32_t fl_ip_to_u(fl_ip ip) {
  uint32_t ret = 0;
  memcpy(&ret, ip, sizeof(ret));
  return ret;
}