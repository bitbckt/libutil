# Autoconf configuration for utils
#
# Copyright (c) 2020 Brandon Mitchell <brandon@thewholedoubt.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

AC_PREREQ(2.60)
AC_INIT([util],
        [0],
        [brandon@thewholedoubt.com],
        [util],
        [http://github.com/bitbckt/libutil])
AC_CONFIG_SRCDIR([src/assert.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_LIBOBJ_DIR([portable])
AM_INIT_AUTOMAKE([check-news foreign 1.11 -Wall -Werror -Wno-portability silent-rules tar-pax no-dist-gzip dist-xz subdir-objects])
AM_MAINTAINER_MODE
AC_PROG_CC_STDC
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])
LT_INIT([disable-static pic-only])
AC_PREFIX_DEFAULT([/usr])

AC_PROG_SED
AC_PROG_MKDIR_P

AC_ARG_VAR([PATH_VALGRIND], [Path to valgrind for the test suite])
AC_PATH_PROG([PATH_VALGRIND], [valgrind])
AM_CONDITIONAL([HAVE_VALGRIND], [test "x$PATH_VALGRIND" != "x"])

AC_ARG_VAR([PATH_CLANG_FORMAT], [Path to clang-format])
AC_PATH_PROG([PATH_CLANG_FORMAT], [clang-format])
AM_CONDITIONAL([HAVE_CLANG_FORMAT], [test "x$PATH_CLANG_FORMAT" != "x"])

AC_ARG_VAR([PATH_CLANG_TIDY], [Path to clang-tidy])
AC_PATH_PROG([PATH_CLANG_TIDY], [clang-tidy])
AM_CONDITIONAL([HAVE_CLANG_TIDY], [test "x$PATH_CLANG_TIDY" != "x"])

AC_TYPE_UINT32_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_ARG_ENABLE([debug],
        AS_HELP_STRING([--enable-debug], [enable debug messages @<:@default=disabled@:>@]),
        [], [enable_debug=no])
AS_IF([test "x$enable_debug" = "xyes"], [
        AC_DEFINE(ENABLE_DEBUG, [1], [Debug messages.])
])

AC_ARG_ENABLE([asserts],
        AS_HELP_STRING([--enable-asserts], [enable asserts @<:@default=disabled@:>@]),
        [], [enable_asserts=no])
AS_IF([test "x$enable_asserts" = "xyes"], [
        AC_DEFINE(ENABLE_ASSERTS, [1], [Asserts.])
])

AC_ARG_ENABLE([panic],
        AS_HELP_STRING([--enable-panic], [panic when assert fails @<:@default=disabled@:>@]),
        [], [enable_panic=no])
AS_IF([test "x$enable_panic" = "xyes"], [
        AC_DEFINE(ASSERT_PANIC, [1], [Assert panics.])
])

my_CFLAGS="-std=c99 -pedantic -Wall -Wextra \
-Wmissing-declarations -Wmissing-prototypes \
-Wnested-externs -Wsign-compare \
-Wstrict-prototypes -Wshadow \
-Wformat-security"
AC_SUBST([my_CFLAGS])

AC_HEADER_STDBOOL
AC_CHECK_HEADERS([errno.h stdarg.h stddef.h stdio.h stdlib.h string.h sys/types.h unistd.h])
AC_CHECK_HEADERS([inttypes.h stdint.h])
AC_CHECK_HEADERS([sys/time.h time.h])
AC_CHECK_HEADERS([execinfo.h], [
        AC_DEFINE(HAVE_BACKTRACE, [1], [backtraces available])], {})
AC_CHECK_DECLS([snprintf vsnprintf])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES([
        Makefile
])

AC_OUTPUT
AC_MSG_RESULT([
        $PACKAGE $VERSION
        =====

        prefix:                 ${prefix}
        sysconfdir:             ${sysconfdir}
        libdir:                 ${libdir}
        includedir:             ${includedir}

        compiler:               ${CC}
        cflags:                 ${CFLAGS}
        ldflags:                ${LDFLAGS}

        valgrind:               ${PATH_VALGRIND}
        clang-format:           ${PATH_CLANG_FORMAT}
        clang-tidy:             ${PATH_CLANG_TIDY}

        debug:                  ${enable_debug}
        asserts:                ${enable_asserts}
        panic:                  ${enable_panic}
])
