//
//  error.h
//  flee
//
//  Created by Unit.2B on 2017/7/21.
//

#ifndef _FLEE_ERROR_H_
#define _FLEE_ERROR_H_

typedef int fl_err;

#define err_ok 0         // no error
#define err_syscall 1    // syscall err, check errno for details
#define err_ip_invalid 2 // ip is invalid

// if ERR is not err_ok, goto LABEL
#define require_ok(ERR, LABEL)                                                 \
  if (ERR != err_ok) {                                                         \
    goto LABEL;                                                                \
  }

// if COND not satisfied, then ACT and goto LABEL
#define require(COND, ACT, LABEL)                                              \
  if (!(COND)) {                                                               \
    ACT;                                                                       \
    goto LABEL;                                                                \
  }

// if E less than 0, assign ERR to CODE
#define check_syscall(E, ERR)                                                  \
  if (E < 0) {                                                                 \
    ERR = err_syscall;                                                         \
  }

#endif /* _FLEE_ERROR_H_ */
