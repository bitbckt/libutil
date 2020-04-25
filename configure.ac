AC_PREREQ(2.60)
AC_INIT([util],
        [0],
        [brandon@thewholedoubt.com],
        [util],
        [http://github.com/bitbckt/libutil])
AC_CONFIG_SRCDIR([src/])
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

AC_ARG_ENABLE([panic],
        AS_HELP_STRING([--enable-panic], [panic when assert fails @<:@default=disabled@:>@]),
        [], [enable_panic=no])
AS_IF([test "x$enable_panic" = "xyes"], [
        AC_DEFINE(ASSERT_PANIC, [1], [Assert panics.])
])

my_CFLAGS="-pedantic -Wall -Wextra \
-Wmissing-declarations -Wmissing-prototypes \
-Wnested-externs -Wsign-compare \
-Wstrict-prototypes -Wshadow \
-Wformat-security"
AC_SUBST([my_CFLAGS])

AC_HEADER_STDBOOL

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

        panic:                  ${enable_panic}
])
