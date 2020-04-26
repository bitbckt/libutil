#pragma once

#include <config.h>

#if HAVE_INTTYPES_H
#    include <inttypes.h>
#endif
#include <stdarg.h>
#include <stddef.h>
#if HAVE_STDINT_H
#    include <stdint.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#if HAVE_UNISTD_H
#    include <unistd.h>
#endif

#ifdef HAVE_BACKTRACE
#    include <execinfo.h>
#endif

#ifndef HAVE_SSIZE_T
typedef ptrdiff_t ssize_t;
#endif

/*
 * POSIX requires that these be defined in <unistd.h>.  If one of them has
 * been defined, all the rest almost certainly have.
 */
#ifndef STDIN_FILENO
#    define STDIN_FILENO  0
#    define STDOUT_FILENO 1
#    define STDERR_FILENO 2
#endif

/*
 * C99 requires va_copy.  Older versions of GCC provide __va_copy.  Per the
 * Autoconf manual, memcpy is a generally portable fallback.
 */
#ifndef va_copy
#    ifdef __va_copy
#        define va_copy(d, s) __va_copy((d), (s))
#    else
#        define va_copy(d, s) memcpy(&(d), &(s), sizeof(va_list))
#    endif
#endif

#if !HAVE_DECL_SNPRINTF
extern int snprintf(char *, size_t, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));
#endif
#if !HAVE_DECL_VSNPRINTF
extern int vsnprintf(char *, size_t, const char *, va_list)
    __attribute__((__format__(printf, 3, 0)));
#endif
