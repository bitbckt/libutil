# Automake makefile for utils
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

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
EXTRA_DIST =
CLEANFILES =

# Work around the GNU Coding Standards, which leave all the Autoconf and
# Automake sutff around after make maintainer-clean, thus making that command
# mostly worthless
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 build-aux/compile \
            build-aux/config.guess build-aux/config.sub build-aux/depcomp \
            build-aux/install-sh build-aux/ltmain.sh build-aux/missing \
            config.h.in config.h.in~ configure m4/libtool.m4 m4/ltoptions.m4 \
            m4/ltsugar.m4 m4/ltversion.m4 m4/lt~obsolete.m4
AM_MAKEFLAGS = --no-print-directory

AM_CPPFLAGS = \
	-include $(top_builddir)/config.h \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DLIBEXECDIR=\""$(libexecdir)"\" \
	-I${top_srcdir}/include \
	-I${top_srcdir}/src

AM_CFLAGS = ${my_CFLAGS} \
	-fvisibility=hidden \
	-ffunction-sections \
	-fdata-sections

AM_LDFLAGS = \
	-Wl,--gc-sections \
	-Wl,--as-needed

SED_PROCESS = \
	$(AM_V_GEN)$(MKDIR_P) $(dir $@) && $(SED) \
	-e 's,@VERSION\@,$(VERSION),g' \
	-e 's,@prefix\@,$(prefix),g' \
	-e 's,@exec_prefix\@,$(exec_prefix),g' \
	-e 's,@libdir\@,$(libdir),g' \
	-e 's,@includedir\@,$(includedir),g' \
	< $< > $@ || rm $@

%.pc: %.pc.in Makefile
	$(SED_PROCESS)

LIBUTIL_CURRENT=0
LIBUTIL_REVISION=0
LIBUTIL_AGE=0

pkginclude_HEADERS = include/util/buffer.h
lib_LTLIBRARIES = src/libutil.la

src_libutil_la_SOURCES =\
	src/assert.h \
	src/assert.c \
	src/buffer.c \
	src/log.h \
	src/log.c \
	src/str.h \
	src/str.c \
	src/util-private.h \
	src/xmalloc.h \
	src/xmalloc.c \
	src/xwrite.h \
	src/xwrite.c

EXTRA_DIST += src/libutil.sym

src_libutil_la_LDFLAGS = $(AM_LDFLAGS) \
	-version-info $(LIBUTIL_CURRENT):$(LIBUTIL_REVISION):$(LIBUTIL_AGE) \
	-Wl,--version-script=$(top_srcdir)/src/libutil.sym
src_libutil_la_DEPENDENCIES = ${top_srcdir}/src/libutil.sym

pkgconfigdir = $(libdir)/pkgconfig
nodist_pkgconfig_DATA = src/libutil.pc
EXTRA_DIST += src/libutil.pc.in
CLEANFILES += src/libutil.pc

# Everything below is for the test suite
check_PROGRAMS =\
	test/runtests \
	test/dbuf-t

test_runtests_CPPFLAGS = -DC_TAP_SOURCE='"$(abs_top_srcdir)/test"' \
	-DC_TAP_BUILD='"$(abs_top_builddir)/test"'
check_LIBRARIES = test/tap/libtap.a
test_tap_libtap_a_CPPFLAGS = -I$(abs_top_srcdir)/test
test_tap_libtap_a_SOURCES =\
	test/tap/basic.c \
	test/tap/basic.h \
	test/tap/macros.h

test_dbuf_t_SOURCES = test/dbuf-t.c
test_dbuf_t_LDADD = test/tap/libtap.a src/libutil.la

check-local: $(check_PROGRAMS)
	cd test && ./runtests -l $(abs_top_srcdir)/test/TESTS

VALGRIND_COMMAND = $(PATH_VALGRIND) --leak-check=full			\
	--trace-children=yes --trace-children-skip=/bin/sh		\
	--log-file=$(abs_top_builddir)/tests/tmp/valgrind/log.%p

check-valgrind: $(check_PROGRAMS)
	rm -rf $(abs_top_builddir)/test/tmp
	mkdir $(abs_top_builddir)/test/tmp
	mkdir $(abs_top_builddir)/test/tmp/valgrind
	C_TAP_VALGRIND="$(VALGRIND_COMMAND)" test/runtests \
	    -l '$(abs_top_srcdir)/test/TESTS'
