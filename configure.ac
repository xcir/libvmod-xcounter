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

m4_ifndef([VARNISH_PREREQ], AC_MSG_ERROR([Need varnish.m4 -- see README.rst]))

VARNISH_PREREQ([6.0])
VARNISH_VMODS([xcounter])

VMOD_TESTS="$(cd $srcdir/src && echo vtc/*.vtc)"
AC_SUBST(VMOD_TESTS)

PKG_CHECK_VAR([LIBVARNISHAPI_LIBDIR], [varnishapi], [libdir])
AC_SUBST([VARNISH_LIBRARY_PATH],
	[$LIBVARNISHAPI_LIBDIR:$LIBVARNISHAPI_LIBDIR/varnish])


####################
#for trunk(VRT_VSC_Allocv)
PKG_CHECK_VAR([VARNISH_PKGINCLUDEDIR], [varnishapi], [pkgincludedir])

AC_CHECK_DECLS([VRT_VSC_Allocv],[],[],
       [#include <stdarg.h>]
       [#include "$VARNISH_PKGINCLUDEDIR/vdef.h"]
       [#include "$VARNISH_PKGINCLUDEDIR/vrt.h"])
####################

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT
