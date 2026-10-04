AC_PREREQ([2.69])
AC_INIT([libvmod-xcounter],[0.1],[],[vmod-xcounter])
AC_COPYRIGHT([Copyright (c) 2018 Shohei Tanaka(@xcir)])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR(src/vmod_xcounter.vcc)
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.12 -Wall -Werror foreign parallel-tests])
AM_SILENT_RULES([yes])
AM_PROG_AR

LT_PREREQ([2.2.6])
LT_INIT([dlopen disable-static])

AX_PTHREAD(,[AC_MSG_ERROR([Could not configure pthreads support])])

LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CC="$PTHREAD_CC"

AC_ARG_WITH([rst2man],
	AS_HELP_STRING(
		[--with-rst2man=PATH],
		[Location of rst2man (auto)]),
	[RST2MAN="$withval"],
	[AC_CHECK_PROGS(RST2MAN, [rst2man rst2man.py], [])])

m4_ifndef([VCACHE_REQUIRE], AC_MSG_ERROR([Need varnish.m4 -- see README.rst]))

VCACHE_REQUIRE(
	[[vinyl], [9.1.0]],
	[[varnish], [9.1.0]]
)
VCACHE_VMODS([xcounter])

VMOD_TESTS="$(cd $srcdir/src && echo vtc/*.vtc)"
AC_SUBST(VMOD_TESTS)

PKG_CHECK_VAR([LIBVCACHEAPI_LIBDIR], [varnishapi], [libdir])
AC_SUBST([VCACHE_LIBRARY_PATH],
	[$LIBVCACHEAPI_LIBDIR:$LIBVCACHEAPI_LIBDIR/varnish])


####################
#for trunk(VRT_VSC_Allocv)
save_CFLAGS="$CFLAGS"
CFLAGS="$VCACHEAPI_CFLAGS"
AC_CHECK_DECLS([VRT_VSC_Allocv],[],[],
       [#include <stdarg.h>]
       [#include "vdef.h"]
       [#include "vrt.h"])
CFLAGS="$save_CFLAGS"
####################

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT
