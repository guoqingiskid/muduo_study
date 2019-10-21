//
// Created by root on 19-10-8.
//

#ifndef REPOS_MY_MUDUO_MACRO_H
#define REPOS_MY_MUDUO_MACRO_H

#include <assert.h>

//use this function include assert.h
__BEGIN_DECLS
extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function)
__THROW __attribute__ ((__noreturn__));
__END_DECLS

#define MCHECK(ret, code) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != code, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#define PTHREAD_RETURE_MCHECK(ret) MCHECK(ret, 0)

#endif //REPOS_MY_MUDUO_MACRO_H
