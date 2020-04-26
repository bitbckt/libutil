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
