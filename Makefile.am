ACLOCAL_AMFLAGS = -I m4 -I ${VARNISHAPI_DATAROOTDIR}/aclocal

SUBDIRS = src

README.rst: src/vmod_xcounter.vcc
	$(MAKE) $(AM_MAKEFLAGS) -C src vmod_xcounter.man.rst
	cp src/vmod_xcounter.man.rst README.rst

dist_doc_DATA = README.rst LICENSE
