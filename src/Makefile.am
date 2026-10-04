AM_CFLAGS = $(VCACHEAPI_CFLAGS) -Wall -Werror
AM_LDFLAGS = $(VCACHEAPI_LIBS) $(VMOD_LDFLAGS)

vmod_LTLIBRARIES = libvmod_xcounter.la

libvmod_xcounter_la_SOURCES = \
	vmod_xcounter.c

nodist_libvmod_xcounter_la_SOURCES = \
	vcc_xcounter_if.c \
	vcc_xcounter_if.h
vmod_vcs_version.txt: $(nodist_libvmod_xcounter_la_SOURCES)
DISTCLEANFILES = vmod_vcs_version.txt

dist_man_MANS = vmod_xcounter.3

@BUILD_VMOD_XCOUNTER@

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VCACHE_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = vtest -E@VTESTEXT@ -v
AM_VTC_LOG_FLAGS = -Dvmod_xcounter="$(VMOD_XCOUNTER)"

TESTS = @VMOD_TESTS@

EXTRA_DIST = \
	vmod_xcounter.vcc \
	$(VMOD_TESTS)
