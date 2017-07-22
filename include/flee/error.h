//
//  error.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_ERROR_H_
#define _FLEE_ERROR_H_

typedef int fl_err;

// no error
#define err_ok 0
// syscall err, check errno for details
#define err_syscall 1
// required function is not implemented in current platform
#define err_platform 2
// ip is invalid
#define err_ip_invalid 3
// unknown cipher
#define err_cipher_unknown 4
// encryption failure, underlaying libsodium failure
#define err_crypto 5

// if ERR is not err_ok, goto LABEL
#define require_ok(ERR, LABEL)                                                 \
  if (ERR != err_ok) {                                                         \
    goto LABEL;                                                                \
  }

// if COND is not satisfied, then ACT and goto LABEL
#define require(COND, ACT, LABEL)                                              \
  if (!(COND)) {                                                               \
    ACT;                                                                       \
    goto LABEL;                                                                \
  }

// if RET is < 0, then assign ERR to err_syscall and goto LABEL
#define require_syscall(RET, ERR, LABEL)                                       \
  require(RET >= 0, ERR = err_syscall, LABEL)

// if RET is not 0, then assign ERR to err_crypto and goto LABEL
#define require_crypto(RET, ERR, LABEL)                                        \
  require(RET == 0, ERR = err_crypto, LABEL)

#endif /* _FLEE_ERROR_H_ */
