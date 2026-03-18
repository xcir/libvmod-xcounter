AM_CFLAGS = $(VINYLAPI_CFLAGS) -Wall -Werror
AM_LDFLAGS = $(VINYLAPI_LIBS) $(VMOD_LDFLAGS)

vmod_LTLIBRARIES = libvmod_xcounter.la

libvmod_xcounter_la_SOURCES = \
	vmod_xcounter.c

nodist_libvmod_xcounter_la_SOURCES = \
	vcc_xcounter_if.c \
	vcc_xcounter_if.h

dist_man_MANS = vmod_xcounter.3

@BUILD_VMOD_XCOUNTER@

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VINYL_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = vinyltest -v
AM_VTC_LOG_FLAGS = -Dvmod_xcounter="$(VMOD_XCOUNTER)"

TESTS = @VMOD_TESTS@

EXTRA_DIST = \
	vmod_xcounter.vcc \
	$(VMOD_TESTS)
